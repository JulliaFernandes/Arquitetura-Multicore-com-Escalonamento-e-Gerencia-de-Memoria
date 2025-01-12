#include "unidadeControle.hpp"
#include "pipeline.hpp"
#include "functions.hpp"

UnidadeControle::UnidadeControle() {}

UnidadeControle::UnidadeControle(PCB &processo)
{

    processo.registers.resize(10, 0);
    processarArquivo(processo);
}

void UnidadeControle::lerAteLinha(ifstream &file, int linhaAlvo)
{
    string line;
    int linhaAtual = 0;

    while (linhaAtual < linhaAlvo && getline(file, line))
    {

        linhaAtual++;
    }

    if (linhaAtual < linhaAlvo)
    {
        // cerr << "Aviso: Arquivo possui apenas " << linhaAtual << " linhas. Continuando do fim." << endl;
    }
}

void UnidadeControle::processarArquivo(PCB &processo)
{

    if (!verificaFCFS)
    {
        ifstream file(processo.nome);

        if (!file.is_open())
        {
            // cerr << "[ERRO] Não foi possível abrir o arquivo: " << processo.nome << endl;
            return;
        }

        lerAteLinha(file, processo.PC);

        string line;
        while (getline(file, line))
        {

            processo.PC++;
            // cout << "\n\nARQUIVO: " << processo.nome << endl << endl;
            InstructionFetch(line, processo);

            if (verificaIf)
            {

                verificaIf = false;
                processo.estado = BLOQUEADO;
                processosAtuais[processo.ID] = processo;
                break;
            }
        }

        processo.estado = CONCLUIDO;
        processosAtuais[processo.ID] = processo;
        file.close();
    }
    else
    {
        ifstream file(processo.nome);

        if (!file.is_open())
        {
            // cerr << "[ERRO] Não foi possível abrir o arquivo: " << processo.nome << endl;
            return;
        }

        string line;

        while (getline(file, line))
        {
            processo.PC++;
            InstructionFetchFCFS(line, processo);
        }

        processo.estado = CONCLUIDO;
        processosAtuais[processo.ID] = processo;

        file.close();
    }
}
