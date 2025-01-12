#include "cpu.hpp"
// #include "pipeline.hpp"
// #include "functions.hpp"
#include "escalonador.hpp"

int id_processo_passado = 0;
PCB processo_passado;
int timestamp_antes = 0;
int entraPrimeiraVez;

void CPU::reiniciar()
{
    id_processo_passado = 0;
    timestamp_antes = 0;
    processosAtuais.clear();
}

CPU::CPU()
{
    inicializarCores();
}

void CPU::inicializarCores()
{
    for (int i = 0; i < NUM_CORES; ++i)
    {
        cores.push_back({i, nullptr});
    }
}

void CPU::executarProcesso(PCB &processo)
{
    UnidadeControle unidadeControle;

    // cout << "\nExecutando processo no núcleo " << core.id + 1 << " com o arquivo: " << processo.nome << endl;

    unidadeControle.processarArquivo(processo);

    this_thread::sleep_for(chrono::milliseconds(500));
}

void CPU::sincronizarTimestamps()
{

    lock_guard<mutex> lock(processosMutex);

    if (processosAtuais.empty())
    {
        return;
    }

    int menorTimestamp = processosAtuais[1].timestamp;
    for (const auto &[id, processo] : processosAtuais)
    {
        menorTimestamp = min(menorTimestamp, processo.timestamp);
    }

    for (auto &[id, processo] : processosAtuais)
    {
        processo.timestamp = menorTimestamp;
    }
}

// sem preempção

bool CPU::tentarAtribuirProcesso(PCB &processo)
{

    unique_lock<mutex> lock(nucleoMutex);

    processo_passado = PCB();

    // Atualiza os timestamps dos processos na fila global antes de atribuir o processo
    {
        lock_guard<mutex> filaLock(queueMutex);

        size_t filaSize = filaPCB.size();
        deque<PCB> tempQueue;

        for (size_t i = 0; i < filaSize; ++i)
        {
            PCB processoFila = filaPCB.front();
            filaPCB.pop_front();
            // cout << "\n\n\nANTES IF: " << processoFila.timestamp << endl << endl;

            if (processoFila.ID != id_processo_passado)
            {
                processoFila.timestamp += abs(processo_passado.timestamp - timestamp_antes);
                // cout << "\n\nTimeStamp: " << processoFila.timestamp << endl;
                // cout << "\n\nProcesso Passado " << processo_passado.timestamp << endl << endl;
                // processosAtuais[processoFila.ID] = processoFila;
            }

            tempQueue.push_back(processoFila);
        }

        // Restaura os processos atualizados de volta à fila original
        filaPCB = tempQueue;
    }

    nucleoDisponivel.wait(lock, [&]()
                          {
        for (const auto &core : cores) {
            if (core.processoAtual == nullptr) {
                return true;
            }
        }
        return false; });

    for (auto &core : cores)
    {
        if (core.processoAtual == nullptr)
        {

            if (processo.ID != id_processo_passado)
            {
                processo.timestamp += abs(processo_passado.timestamp - timestamp_antes);
                // processosAtuais[processo.ID] = processo;
            }

            core.processoAtual = &processo;

            // Cria uma thread para executar o processo
            thread([this, &core, processo]() mutable
                   {
                // cout << "\n >>>> ESTOU INDO EXECUTAR O PROCESSO: " << processo.nome << endl;
                // Executa o processo
                // timestamp_antes = processo.timestamp;

                // executarProcesso(core, processo);
                // processosAtuais[processo.ID] = processo;


                /////////
                auto timestamp_antes_execucao = processo.timestamp;

               // executarProcesso(core, processo);
               executarProcesso(processo);

                auto timestamp_depois_execucao = processo.timestamp;

                auto timestamp_gasto = timestamp_depois_execucao - timestamp_antes_execucao;

                if(entraPrimeiraVez==0){
                    
                    infoPrimeiroProcesso.first = processo.nome;
                    infoPrimeiroProcesso.second = timestamp_gasto;
                    //cout << "\n\nENTROU NO VALORPRIMEIROTIMESTAMP: " << "Nome: "<< infoPrimeiroProcesso.first << " ------ " << infoPrimeiroProcesso.second << endl;
                    entraPrimeiraVez++;
                    
                }
                else if(entraPrimeiraVez==1){
                    infoSegundoProcesso.first = processo.nome;
                    infoSegundoProcesso.second = timestamp_gasto;
                    //cout << "\n\nENTROU NO VALORSEGUNDOTIMESTAMP: " << "Nome: "<< infoSegundoProcesso.first << " ------ " << infoSegundoProcesso.second << endl;
                    entraPrimeiraVez++;
                    
                }

                {
                    lock_guard<mutex> filaLock(queueMutex);  
                    id_processo_passado = processo.ID;
                    processo_passado = processo;  
                }

                // Libera o núcleo após a execução
                liberarNucleo(core); })
                .detach();

            return true;
        }
    }
    return false;
}

// preemptivos

bool CPU::Mod1tentarAtribuirProcesso(PCB &processo)
{

    unique_lock<mutex> lock(nucleoMutex);
    static int id_processo_passado = 0;
    static PCB processo_passado = PCB();
    static int timestamp_antes = 0;

    // Atualiza os timestamps dos processos na fila global antes de atribuir o processo
    {
        lock_guard<mutex> filaLock(queueMutex);
        size_t filaSize = filaPCB.size();
        deque<PCB> tempQueue;

        for (size_t i = 0; i < filaSize; ++i)
        {
            PCB processoFila = filaPCB.front();
            filaPCB.pop_front();

            if (processoFila.ID != id_processo_passado)
            {

                processoFila.timestamp += processo_passado.timestamp - timestamp_antes;

                processosAtuais[processoFila.ID] = processoFila;
            }

            tempQueue.push_back(processoFila);
        }

        filaPCB = tempQueue;
    }

    // Espera até que um núcleo fique disponível
    nucleoDisponivel.wait(lock, [&]()
                          {
        for (const auto &core : cores) {
            if (core.processoAtual == nullptr) {
                return true;
            }
        }
        return false; });

    // Atribui o processo ao núcleo e cria a thread para execução
    for (auto &core : cores)
    {
        if (core.processoAtual == nullptr)
        {

            if (processo.ID != id_processo_passado)
            {
                processo.timestamp += processo_passado.timestamp - timestamp_antes;
                processosAtuais[processo.ID] = processo;
            }

            core.processoAtual = &processo;

            // Cria uma thread para executar o processo
            thread([this, &core, processo]() mutable
                   {
                timestamp_antes = processo.timestamp;

                executarProcesso(processo);
                {
                    lock_guard<mutex> filaLock(queueMutex);  // Protege o acesso à filaPCB
                    id_processo_passado = processo.ID;
                    processo_passado = processo;  
                }

         
                liberarNucleo(core); })
                .detach();

            return true;
        }
    }

    return false;
}

void CPU::liberarNucleo(Core &core)
{
    {
        lock_guard<mutex> lock(nucleoMutex);
        core.processoAtual = nullptr;
    }

    // Notifica que um núcleo ficou disponível
    nucleoDisponivel.notify_one();
}

bool CPU::temNucleoLivre()
{

    lock_guard<mutex> lock(nucleoMutex);

    for (const auto &core : cores)
    {
        if (core.processoAtual == nullptr)
            return true;
    }
    return false;
}

bool CPU::todosProcessosFinalizados()
{

    lock_guard<mutex> lock(nucleoMutex);
    for (const auto &core : cores)
    {
        if (core.processoAtual != nullptr)
            return false;
    }

    return true;
}
