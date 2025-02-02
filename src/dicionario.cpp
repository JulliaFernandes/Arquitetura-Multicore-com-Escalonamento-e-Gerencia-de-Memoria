#include "dicionario.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <list>
#include <mutex>
#include <vector>

using namespace std;

const double SIMILARIDADE_MINIMA = 50.0;

void printCacheAfterInsertLRU()
{
    cout << "\nCache após inserção no modo LRU:\n";
    for (const auto &chave : cacheOrder)
    {
        auto it = cache.find(chave);
        if (it != cache.end())
        {
            cout << "Chave: " << it->first << " -> Valor: " << it->second << endl;
        }
        else
        {
            cout << "Chave: " << chave << " -> [ERRO] Não encontrada na cache!" << endl;
        }
    }
}

void printCacheAfterInsertFIFO()
{
    cout << "\nCache após inserção no modo FIFO:\n";
    for (const auto &chave : cacheOrder)
    {
        auto it = cache.find(chave);
        if (it != cache.end())
        {
            cout << "Chave: " << it->first << " -> Valor: " << it->second << endl;
        }
        else
        {
            cout << "Chave: " << chave << " -> [ERRO] Não encontrada na cache!" << endl;
        }
    }
}

int calcularOperacao(char operacao, int operando1, int operando2)
{
    switch (operacao)
    {
    case '+':
        return operando1 + operando2;
    case '-':
        return operando1 - operando2;
    case '*':
        return operando1 * operando2;
    case '/':
        return operando2 != 0 ? operando1 / operando2 : 0;
    default:
        cerr << "\n[ERRO] Operação inválida: " << operacao << endl;
        return 0;
    }
}

double calcularSimilaridade(const string &instrucao1, const string &instrucao2)
{
    vector<string> partes1, partes2;
    stringstream ss1(instrucao1), ss2(instrucao2);
    string temp;

    while (ss1 >> temp)
        partes1.push_back(temp);
    while (ss2 >> temp)
        partes2.push_back(temp);

    if (partes1.size() != partes2.size())
        return 0.0;

    int iguais = 0;
    for (size_t i = 0; i < partes1.size(); ++i)
    {
        if (partes1[i] == partes2[i])
            ++iguais;
    }

    double similaridade = (double)iguais / partes1.size() * 100.0;

    similaridade = round(similaridade * 1000.0) / 1000.0;

    // cout << "Similaridade arredondada: " << similaridade << endl;

    return similaridade;
}

void carregarDicionario(const string &filePath)
{
    ifstream arquivo(filePath);
    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir o arquivo: " << filePath << endl;
        return;
    }

    string linha;
    while (getline(arquivo, linha))
    {
        stringstream ss(linha);
        char operacao;
        int operando1, operando2;

        ss >> operacao >> operando1 >> operando2;

        string chave = string(1, operacao) + " " + to_string(operando1) + " " + to_string(operando2);
        int resultado = calcularOperacao(operacao, operando1, operando2);
        LSH[chave] = resultado;
    }

    arquivo.close();
}

void exibirDicionario()
{
    for (const auto &par : LSH)
    {
        cout << "Chave: " << par.first << " -> Valor: " << par.second << endl;
    }
}

bool verificarCacheComLSH(const string &chave, int &resultado)
{

    lock_guard<recursive_mutex> lock(cacheMutex);

    if (chavesEmProcessamento.find(chave) != chavesEmProcessamento.end())
    {
        //cout << "\n[ERROR] Chave já em processamento: " << chave << ". Evitando ciclo." << endl;
        return false;
    }

    // Marcar a chave como em processamento
    chavesEmProcessamento.insert(chave);

    auto it = cache.find(chave);
    if (it != cache.end())
    {
        // Atualizar a ordem na cache
        if (verificaFIFO)
        {
            cacheOrder.remove(chave);    // Remover da posição atual
            cacheOrder.push_back(chave); // Adicionar ao final
        }
        else
        {
            cacheOrder.remove(chave);     // Remover da posição atual
            cacheOrder.push_front(chave); // Adicionar ao início
        }

        // Atribuir o resultado encontrado
        resultado = it->second;
        chavesEmProcessamento.erase(chave);
        return true;
    }

    // Buscar chave mais semelhante com maior similaridade
    double maiorSimilaridade = 0.0;
    string chaveMaisSimilar;

    for (const auto &par : cache)
    {
        double similaridade = calcularSimilaridade(chave, par.first);
        if (similaridade > maiorSimilaridade)
        {
            maiorSimilaridade = similaridade;
            chaveMaisSimilar = par.first;
        }
    }

    //exibirCacheOrder();

    // cout << "\nSIMILARIDADE: " << maiorSimilaridade << " | Chave mais similar: " << chaveMaisSimilar << endl;

    if (maiorSimilaridade >= SIMILARIDADE_MINIMA)
    {

        // Caso o valor da chave mais similar não seja o suficiente, recalcule o resultado
        stringstream ss(chave);
        char operacao;
        int operando1, operando2;
        ss >> operacao >> operando1 >> operando2;
        resultado = calcularOperacao(operacao, operando1, operando2);

        // Atualizar a cache com a nova chave e resultado
        atualizarCache(chave, resultado);

        // Remover a marcação de processamento
        chavesEmProcessamento.erase(chave);

        // cout << "\n[INFO] Cache atualizada com nova chave: " << chave << " -> " << resultado << endl;
        return true;
    }

    // Caso nenhuma similaridade seja suficiente, retornar falso
    chavesEmProcessamento.erase(chave);
    // cout << "\n[INFO] Nenhuma chave similar suficiente encontrada." << endl;

    return false;
}

void transferirLSHParaCache()
{

    for (const auto &par : LSH)
    {

        atualizarCache(par.first, par.second);
    }
}

void atualizarCache(const string &chave, int resultado)
{

    lock_guard<recursive_mutex> lock(cacheMutex);

    if (cache.size() >= CACHE_SIZE)
    {

        if (!cacheOrder.empty())
        {
            if (verificaFIFO)
            {
                // cout << "FIFO" << endl;
                string fifo = cacheOrder.front();
                cacheOrder.pop_front();
                cache.erase(fifo);
            }
            else
            {
                // cout << "LRU" << endl;
                string lru = cacheOrder.back();
                cacheOrder.pop_back();
                cache.erase(lru);
            }
        }
    }

    cache[chave] = resultado;
    if (verificaFIFO)
    {

        cacheOrder.push_back(chave);
        // printCacheAfterInsertFIFO();
    }
    else
    {

        cacheOrder.push_front(chave);
        // printCacheAfterInsertLRU();
    }
}

void exibirCache()
{
    cout << "\n\tCACHE\n"
         << endl;
    for (const auto &par : cache)
    {
        cout << "Chave: " << par.first << " -> Valor: " << par.second << endl;
    }
}

void exibirCacheOrder()
{

    cout << "\n\tCACHE \n";
    for (const auto &chave : cacheOrder)
    {
        auto it = cache.find(chave);
        if (it != cache.end())
        {
            cout << "Chave: " << it->first << " -> Valor: " << it->second << endl;
        }
        else
        {
            cout << "Chave: " << chave << " -> [ERROR] Não encontrada na cache!" << endl;
        }
    }
}
