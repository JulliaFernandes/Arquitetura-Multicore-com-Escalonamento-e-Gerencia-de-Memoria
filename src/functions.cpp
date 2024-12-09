#include "unidadeControle.hpp"
#include "functions.hpp"

using namespace std;

// Função para inicializar um processo com quantum e timestamp

PCB criarProcesso(int ID, ProcessState s, int prioridade, int baseEndereco, int limiteEndereco, int quantumInicial, int timestamp) {
    PCB novoProcesso(
        ID,        // ID do processo          
        s, 
        prioridade,        // Prioridade do processo
        baseEndereco,      // Endereço base
        limiteEndereco,    // Limite de memória
        quantumInicial = 20 + rand() % 31,     // Quantum inicial
        timestamp
    );
    CLOCK++;
    return novoProcesso;
}

void LerInstrucoesDoArquivo(const string &nomeArquivo, int *registradores){

    // string nomeArquivo = "input.data";
    ifstream arquivo(nomeArquivo);
    string linha;

    if (!arquivo.is_open())
    {
        cout << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    while (getline(arquivo, linha))
    {
        // stringstream ss(linha);
        UnidadeControle(registradores, linha);
        cout << "Clock: " << CLOCK << endl;
    }

    arquivo.close();
}

// Simula a execução de um processo em um núcleo
void executeProcess(PCB &process) {

    cout << "Executando processo: " << process.ID << endl;
    process.estado = RODANDO;
    
    // Simula execução reduzindo o quantum
    while (process.quantum > 0) {
        this_thread::sleep_for(chrono::milliseconds(500)); // Simula trabalho
        process.quantum--;
    }

    // Processo finaliza o quantum e volta para a fila de prontos
    queueMutex.lock();
    process.estado = PRONTO;
    filaProntos.push(process);
    queueMutex.unlock();
    cout << "Processo " << process.ID << " preemptado.\n";
}


// Interrompe o processo atual no núcleo e o move para a fila de prontos

void preempt_process(Core &core) {
    if (core.processoAtual != nullptr) {
        core.processoAtual->estado = PRONTO;

        // Adiciona o processo interrompido de volta à fila de prontos
        {
            lock_guard<mutex> lock(queueMutex);
            filaProntos.push(*(core.processoAtual));
        }

        core.processoAtual = nullptr;
        cout << "Processo preemptado e movido para a fila de prontos." << endl;
    }
}

// Simula um processo bloqueado por solicitação de recurso

void block_process(Core &core) {
    if (core.processoAtual != nullptr) {
        core.processoAtual->estado = BLOQUEADO;
        {
            lock_guard<mutex> lock(queueMutex);
            filaBloqueados.push(*(core.processoAtual));
        }
        core.processoAtual = nullptr;
        cout << "Processo bloqueado e movido para a fila de bloqueados." << endl;
    }
}

