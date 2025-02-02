#include "functions.hpp"
#include "escalonador.hpp"

int idGlobal = 1;
mutex hashSimbolosMutex;

void printProcessInfo(const PCB &processo)
{

    lock_guard<mutex> lock(printMutex);

    cout << "ID: " << processo.ID << endl;
    cout << "Prioridade: " << processo.prioridade << endl;
    cout << "Nome do Arquivo: " << processo.nome << endl;
    cout << "Quantum: " << processo.quantum << endl;
    cout << "Timestamp: " << processo.timestamp << endl;
    cout << "Tempo total de execução: " << processo.tempoTotalExecucao << endl;
    cout << "PC: " << processo.PC << endl;
    cout << "--------------------------" << endl;
}

void criaHashSimbolos()
{
    lock_guard<mutex> lock(hashSimbolosMutex);
    hashSimbolos['@'] = 5;
    hashSimbolos['='] = 3;
    hashSimbolos['+'] = 5;
    hashSimbolos['-'] = 5;
    hashSimbolos['*'] = 5;
    hashSimbolos['/'] = 5;
    hashSimbolos['?'] = 3;
}

void processarInput()
{
    if (filaPCB.empty())
    {
        cerr << "Fila PCB está vazia. Não há processos para processar." << endl;
        return;
    }

    for (int i = 1; i < numeroProcessos + 1; i++)
    {

        PCB pcb = filaPCB.front();
        filaPCB.pop_front();

        ifstream inputFile(pcb.nome);
        if (!inputFile.is_open())
        {
            cerr << "Erro ao abrir o arquivo: " << pcb.nome << endl;
            continue;
        }

        string line;
        while (getline(inputFile, line))
        {
            if (!line.empty())
            {
                char simbolo = line[0];
                if (hashSimbolos.find(simbolo) != hashSimbolos.end())
                {
                    pcb.tempoTotalExecucao += hashSimbolos[simbolo];
                }
            }
        }
        inputFile.close();
        lock_guard<mutex> lock(queueMutex);
        filaPCB.push_back(pcb);
    }
}

void criarProcesso(const string &filePath)
{

    PCB processo;

    ifstream file(filePath);
    if (file.is_open())
    {
        this_thread::sleep_for(chrono::milliseconds(500));
        int aux = idGlobal++;
        processo.ID = aux;
        processo.estado = PRONTO;
        processo.prioridade = 1 + rand() % 3;
        processo.baseEndereco = 0;
        processo.limiteEndereco = 0;
        processo.quantum = 20;
        processo.timestamp = 0;
        processo.PC = 0;
        processo.nome = filePath;
        processo.registers = vector<int>(10, 0);
        processo.numPipeline = 0;
        processo.tempoTotalExecucao = 0;

        {
            lock_guard<mutex> lock(queueMutex);
            filaProntos.push(processo);
        }

        file.close();
    }
}

bool verificaQuantum(PCB &processo)
{

    if (processo.quantum <= 0)
    {

        processo.estado = PRONTO;
        processo.quantum = 10;

        processo.PC--;
        lock_guard<mutex> lock(queueMutex);

        filaPCB.push_back(processo);

        return true;
    }

    else
        return false;
}

void salvarProcessosEmArquivo(string &nomeArquivo)
{

    CPU cpu;
    ofstream file_OutPut(nomeArquivo);
    if (!file_OutPut.is_open())
    {
        cerr << "[ERRO] Não foi possível criar o arquivo de saída.\n";
        return;
    }

    // for (auto &[id, p] : processosAtuais)
    // {
    //     if (infoSegundoProcesso.first == p.nome)
    //     {

    //         p.timestamp += infoPrimeiroProcesso.second;
    //         processosAtuais[id] = p;
    //     }
    // }

    if (numeroProcessos == NUM_CORES)
    {
        cpu.sincronizarTimestamps();
    }

    file_OutPut << "_________________________________________________________________________________________________________________________________________________________________\n";
    file_OutPut << "                                                          PROCESSOS       		               		                            \n";
    file_OutPut << "_________________________________________________________________________________________________________________________________________________________________\n";

    for (const auto &[id, processo] : processosAtuais)
    {
        file_OutPut << "ID: " << id << "\n";
        file_OutPut << "Nome do Arquivo: " << processo.nome << "\n";
        file_OutPut << "Estado: " << processo.estado << "\n";
        file_OutPut << "Prioridade: " << processo.prioridade << "\n";
        file_OutPut << "Quantum: " << processo.quantum << "\n";
        file_OutPut << "Timestamp: " << processo.timestamp << "\n";
        file_OutPut << "Peso: " << processo.tempoTotalExecucao << "\n";
        file_OutPut << "PC: " << processo.PC << "\n";

        file_OutPut << "Registros: ";
        for (int reg : processo.registers)
        {
            file_OutPut << reg << " ";
        }
        file_OutPut << "\n";

        if (memoria.find(id) != memoria.end())
        {
            file_OutPut << "Memória Principal (ID: " << id << "): " << memoria[id] << "\n";
        }
        else
        {
            file_OutPut << "Memória Principal (ID: " << id << "): Não disponível\n";
        }

        file_OutPut << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
    }

    file_OutPut << "====================================================================================================================================================================\n";

    file_OutPut.close();

    cout << "\n[INFO] Saída " << nomeArquivo << " gerado com sucesso.\n";
}

/////////////////////////////////////// NÃO PREEMPTIVOS ///////////////////////////////////////

void executarEscalonadoresNaoPreemptivosLRU()
{
    string filePath = "./data/dicionario.txt";
    Escalonador escalonador;
    CPU cpu;

    /////////////////////////////////// FCFS ///////////////////////////////////
    cpu.reiniciar();

    carregarDicionario(filePath);
    cout << "\n\tDICIONÁRIO LSH\n" << endl;
    exibirDicionario();
    transferirLSHParaCache();

    cout << "\n\tTamanho Cache: " << CACHE_SIZE << " bytes" << std::endl;
    {
        lock_guard<mutex> lock(queueMutex);
        while (!filaProntos.empty())
        {
            escalonador.adicionarProcesso(filaProntos.front());
            filaProntosReserva.push(filaProntos.front());
            filaProntos.pop();
        }
    }

    cout << "_________________________________________________________________________________________\n\n";
    verificaFCFS = true;
    sleep(1);

    auto inicio1 = high_resolution_clock::now(); // Tempo inicial

    escalonador.executarFCFS();

    string fcfs = "data/FCFSoutput.txt";
    salvarProcessosEmArquivo(fcfs);

    auto fim1 = high_resolution_clock::now();
    auto duracao1 = duration_cast<microseconds>(fim1 - inicio1);

    cout << "\nTempo FCFS com LRU: " << duracao1.count() / 1e6 << " segundos\n\n";

    exibirCacheOrder();

    /////////////////////////////////// SJF ///////////////////////////////////

    cpu.reiniciar();

    carregarDicionario(filePath);
    transferirLSHParaCache();

    {
        lock_guard<mutex> lock(queueMutex);
        queue<PCB> filaTemporaria = filaProntosReserva;
        while (!filaTemporaria.empty())
        {
            // Adiciona o processo ao escalonador
            escalonador.adicionarProcesso(filaTemporaria.front());
            filaTemporaria.pop();
        }
    }


    cout << "_________________________________________________________________________________________\n\n";
    verificaFCFS = true;
    sleep(1);

    auto inicio4 = high_resolution_clock::now();

    criaHashSimbolos();
    processarInput();

    escalonador.executarMenorJobPrimeiro();

    string sjf = "data/SJFoutput.txt";
    salvarProcessosEmArquivo(sjf);

    auto fim4 = high_resolution_clock::now();

    auto duracao4 = duration_cast<microseconds>(fim4 - inicio4);
    cout << "\nTempo SJF com LRU: " << duracao4.count() / 1e6 << " segundos\n\n";

    exibirCacheOrder();
}

void executarEscalonadoresNaoPreemptivosFIFO()
{
    string filePath = "./data/dicionario.txt";
    Escalonador escalonador;
    CPU cpu;

    cpu.reiniciar();

    {
        lock_guard<mutex> lock(queueMutex);
        queue<PCB> filaTemporaria = filaProntosReserva;
        while (!filaTemporaria.empty())
        {
            // Adiciona o processo ao escalonador
            escalonador.adicionarProcesso(filaTemporaria.front());
            filaTemporaria.pop();
        }
    }

    carregarDicionario(filePath);
    transferirLSHParaCache();

    verificaFCFS = true;
    verificaFIFO=true;
    cout << "_________________________________________________________________________________________\n\n";
    sleep(1);

    auto inicio2 = high_resolution_clock::now(); // Tempo inicial

    escalonador.executarFCFS();

    string fcfs2 = "data/FCFSoutput.txt";
    salvarProcessosEmArquivo(fcfs2);

    auto fim2 = high_resolution_clock::now();
    auto duracao2 = duration_cast<microseconds>(fim2 - inicio2);

    cout << "\nTempo FCFS com FIFO: " << duracao2.count() / 1e6 << " segundos\n\n";

    exibirCacheOrder();

    /////////////////////////////////// SJF ///////////////////////////////////

    cpu.reiniciar();

    carregarDicionario(filePath);
    transferirLSHParaCache();

    {
        lock_guard<mutex> lock(queueMutex);
        queue<PCB> filaTemporaria = filaProntosReserva;
        while (!filaTemporaria.empty())
        {
            // Adiciona o processo ao escalonador
            escalonador.adicionarProcesso(filaTemporaria.front());
            filaTemporaria.pop();
        }
    }

    cout << "_________________________________________________________________________________________\n\n";
    verificaFIFO = true;
    sleep(1);

    auto inicio4 = high_resolution_clock::now();

    criaHashSimbolos();
    processarInput();

    escalonador.executarMenorJobPrimeiro();

    string sjf = "data/SJFoutput.txt";

    salvarProcessosEmArquivo(sjf);

    auto fim4 = high_resolution_clock::now();

    auto duracao4 = duration_cast<microseconds>(fim4 - inicio4);
    cout << "\nTempo SJF com FIFO: " << duracao4.count() / 1e6 << " segundos\n\n";

    exibirCacheOrder();
}

/////////////////////////////////////// PREEMPTIVOS ///////////////////////////////////////

void executarEscalonadoresPreemptivosLRU()
{
    string filePath = "./data/dicionario.txt";
    Escalonador escalonador;
    CPU cpu;

    /////////////////////////////////// ROUNDIN ROBIN ///////////////////////////////////
    cpu.reiniciar();

    carregarDicionario(filePath);
    transferirLSHParaCache();

    {
        lock_guard<mutex> lock(queueMutex);
        queue<PCB> filaTemporaria = filaProntosReserva;
        while (!filaTemporaria.empty())
        {
            // Adiciona o processo ao escalonador
            escalonador.adicionarProcesso(filaTemporaria.front());
            filaTemporaria.pop();
        }
    }

    cout << "_________________________________________________________________________________________\n\n";
    verificaFCFS = false;
    verificaFIFO = false;
    sleep(1);

    auto inicio2 = high_resolution_clock::now(); // Tempo inicial

    escalonador.executarRoundRobin();

    string rr = "data/RRoutput.txt";
    salvarProcessosEmArquivo(rr);

    auto fim2 = high_resolution_clock::now();
    auto duracao2 = duration_cast<microseconds>(fim2 - inicio2);
    cout << "\nTempo Round Robin com LRU: " << duracao2.count() / 1e6 << " segundos\n\n";

    exibirCacheOrder();

    /////////////////////////////////// PRIORIDADE ///////////////////////////////////

    cpu.reiniciar();

    carregarDicionario(filePath);
    transferirLSHParaCache();
 
    {
        lock_guard<mutex> lock(queueMutex);
        queue<PCB> filaTemporaria = filaProntosReserva;
        while (!filaTemporaria.empty())
        {
            // Adiciona o processo ao escalonador
            escalonador.adicionarProcesso(filaTemporaria.front());
            filaTemporaria.pop();
        }
    }

    cout << "_________________________________________________________________________________________\n\n";
    verificaFCFS = false;
    verificaFIFO = false;
    sleep(1);

    auto inicio3 = high_resolution_clock::now(); // Tempo inicial

    escalonador.executarPrioridade();

    string p = "data/Poutput.txt";
    salvarProcessosEmArquivo(p);

    auto fim3 = high_resolution_clock::now();
    auto duracao3 = duration_cast<microseconds>(fim3 - inicio3);
    cout << "\nTempo Prioridade com LRU: " << duracao3.count() / 1e6 << " segundos\n\n";

    exibirCacheOrder();
}

void executarEscalonadoresPreemptivosFIFO()
{
    string filePath = "./data/dicionario.txt";
    Escalonador escalonador;
    CPU cpu;

    /////////////////////////////////// ROUNDIN ROBIN ///////////////////////////////////
    cpu.reiniciar();

    carregarDicionario(filePath);
    transferirLSHParaCache();

    {
        lock_guard<mutex> lock(queueMutex);
        queue<PCB> filaTemporaria = filaProntosReserva;
        while (!filaTemporaria.empty())
        {
            // Adiciona o processo ao escalonador
            escalonador.adicionarProcesso(filaTemporaria.front());
            filaTemporaria.pop();
        }
    }
  
    cout << "_________________________________________________________________________________________\n\n";
    verificaFCFS = false;
    verificaFIFO = true;
    sleep(1);

    auto inicio2 = high_resolution_clock::now(); // Tempo inicial

    escalonador.executarRoundRobin();

    string rr = "data/RRoutput.txt";
    salvarProcessosEmArquivo(rr);

    auto fim2 = high_resolution_clock::now();
    auto duracao2 = duration_cast<microseconds>(fim2 - inicio2);
    cout << "\nTempo Round Robin com FIFO: " << duracao2.count() / 1e6 << " segundos\n\n";

    exibirCacheOrder();

    /////////////////////////////////// PRIORIDADE ///////////////////////////////////
 
    cpu.reiniciar();

    carregarDicionario(filePath);
    transferirLSHParaCache();

    {
        lock_guard<mutex> lock(queueMutex);
        queue<PCB> filaTemporaria = filaProntosReserva;
        while (!filaTemporaria.empty())
        {
            // Adiciona o processo ao escalonador
            escalonador.adicionarProcesso(filaTemporaria.front());
            filaTemporaria.pop();
        }
    }

    cout << "_________________________________________________________________________________________\n\n";
    verificaFCFS = false;
    verificaFIFO = true;
    sleep(1);

    auto inicio3 = high_resolution_clock::now(); // Tempo inicial

    escalonador.executarPrioridade();

    string p = "data/Poutput.txt";
    salvarProcessosEmArquivo(p);

    auto fim3 = high_resolution_clock::now();
    auto duracao3 = duration_cast<microseconds>(fim3 - inicio3);
    cout << "\nTempo Prioridade com FIFO: " << duracao3.count() / 1e6 << " segundos\n\n";

   exibirCacheOrder();
}