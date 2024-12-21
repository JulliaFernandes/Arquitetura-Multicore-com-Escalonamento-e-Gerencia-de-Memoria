#include "cpu.hpp"
#include "pipeline.hpp"
#include "functions.hpp"
#include "escalonador.hpp"

CPU::CPU() {
    inicializarCores();
}

void CPU::inicializarCores() {
    for (int i = 0; i < NUM_CORES; ++i) {
        cores.push_back({i, nullptr});
    }
}

void CPU::executarProcesso(Core &core, PCB &processo) {

    UnidadeControle unidadeControle;

    cout << "\nExecutando processo no núcleo " << core.id + 1 << " com o arquivo: " << processo.nome << endl;

    unidadeControle.processarArquivo(processo);

    this_thread::sleep_for(chrono::milliseconds(500)); // Simulação de execução
}

void CPU::sincronizarTimestamps() {
    lock_guard<mutex> lock(processosMutex); // Protege o acesso aos processosAtuais

    if (processosAtuais.empty()) {
        return; 
    }

    
    int menorTimestamp = processosAtuais[1].timestamp;
    for (const auto &[id, processo] : processosAtuais) {
        menorTimestamp = min(menorTimestamp, processo.timestamp);
    }

    for (auto &[id, processo] : processosAtuais) {
        processo.timestamp = menorTimestamp;
    }

    // cout << "[INFO] Timestamps sincronizados para " << menorTimestamp << ".\n";
}

bool CPU::tentarAtribuirProcesso(PCB &processo) {

    unique_lock<mutex> lock(nucleoMutex);
    static int id_processo_passado = 0;  // Torna id_processo_passado estático para persistir entre as chamadas
    static PCB processo_passado = PCB();
    static int timestamp_antes = 0;

    // Atualiza os timestamps dos processos na fila global antes de atribuir o processo
    {
        lock_guard<mutex> filaLock(queueMutex);  // Protege o acesso à filaPCB
        size_t filaSize = filaPCB.size();
        deque<PCB> tempQueue;

        for (size_t i = 0; i < filaSize; ++i) {
            PCB processoFila = filaPCB.front();  // Referência ao processo da frente
            filaPCB.pop_front();  // Remove o processo da frente da fila

            if (processoFila.ID != id_processo_passado) {
                processoFila.timestamp += processo_passado.timestamp - timestamp_antes;
                processosAtuais[processoFila.ID] = processoFila;
            }

            // Coloca o processo atualizado de volta à fila temporária
            tempQueue.push_back(processoFila);
        }

        // Restaura os processos atualizados de volta à fila original
        filaPCB = tempQueue;
    }

    // Espera até que um núcleo fique disponível
    nucleoDisponivel.wait(lock, [&]() {
        for (const auto &core : cores) {
            if (core.processoAtual == nullptr) {
                return true;
            }
        }
        return false; 
    });

    // Atribui o processo ao núcleo e cria a thread para execução
    for (auto &core : cores) {
        if (core.processoAtual == nullptr) {
            // Atualiza o timestamp do processo que será atribuído ao núcleo
            if (processo.ID != id_processo_passado) {
                processo.timestamp += processo_passado.timestamp - timestamp_antes;
                processosAtuais[processo.ID] = processo;
            }

            core.processoAtual = &processo;

            // Cria uma thread para executar o processo
            thread([this, &core, processo]() mutable {
                // cout << "\n >>>> ESTOU INDO EXECUTAR O PROCESSO: " << processo.nome << endl;
                // Executa o processo
                timestamp_antes = processo.timestamp;

                executarProcesso(core, processo);
                processosAtuais[processo.ID] = processo;

                // Atualiza o id_processo_passado após a execução
                {
                    lock_guard<mutex> filaLock(queueMutex);  // Protege o acesso à filaPCB
                    id_processo_passado = processo.ID;
                    processo_passado = processo;  // Atualiza o id_processo_passado após a execução
                }

                // Libera o núcleo após a execução
                liberarNucleo(core);
            }).detach();

            return true;
        }
    }

    return false; // Redundante, mas mantém segurança
}


void CPU::liberarNucleo(Core &core) {
    {
        lock_guard<mutex> lock(nucleoMutex);
        core.processoAtual = nullptr;
    }

    // Notifica que um núcleo ficou disponível
    nucleoDisponivel.notify_one();
}


bool CPU::temNucleoLivre() {

    lock_guard<mutex> lock(nucleoMutex);

    for (const auto &core : cores) {
        if (core.processoAtual == nullptr) return true;
    }
    return false;
}

bool CPU::todosProcessosFinalizados() {

    lock_guard<mutex> lock(nucleoMutex);
    for (const auto &core : cores) {
        if (core.processoAtual != nullptr) return false;
    }

    return true;
}
