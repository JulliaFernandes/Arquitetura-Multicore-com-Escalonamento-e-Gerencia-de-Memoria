#include "include.hpp"
#include "escalonador.hpp"
#include "functions.hpp"

namespace fs = std::filesystem;

mutex queueMutex; 
mutex printMutex;
mutex processosMutex;

deque<PCB> filaPCB;
deque<PCB> filaReserva;

queue<PCB> filaProntosReserva;
queue<PCB> filaProntos;

pair<string, int> infoPrimeiroProcesso;
pair<string, int> infoSegundoProcesso;

vector<int> principal;
vector<vector<int>> disco;

map<int, int> memoria;
map<int, PCB> processosAtuais;
map<int, PCB> processosAtuaisRR;

unordered_map<char, int> hashSimbolos;
unordered_map<int, int> cache;

int CLOCK;
int numeroProcessos;

bool verificaIf;
bool verificaFCFS;

void criarProcessoThread(const string &filePath)
{
    criarProcesso(filePath);
}

int main()
{
    std::srand(std::time(nullptr));

    auto inicio = high_resolution_clock::now(); // Tempo inicial

    // Código cuja execução você quer medir
    for (volatile int i = 0; i < 100000000; ++i);

    string dataPath = "./data";
    verificaFCFS = false;
    vector<thread> threads;

    CPU();

    // Cria threads para preparar os processos
    this_thread::sleep_for(chrono::milliseconds(500));
    for (const auto &entry : fs::directory_iterator(dataPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".data")
        {
            string filePath = entry.path().string();
            numeroProcessos++;
            threads.emplace_back(criarProcessoThread, filePath); // Cada arquivo é gerenciado em uma thread
        }
    }

    for (auto &t : threads)
    {
        if (t.joinable())
        {
            t.join();
        }
    }

    // cout << "\nNúmero processos " << numeroProcessos << "\n";

    Escalonador escalonador;
    CPU cpu;

    // Transfere os processos da fila de prontos para o escalonador
    {
        lock_guard<mutex> lock(queueMutex);
        while (!filaProntos.empty())
        {
            escalonador.adicionarProcesso(filaProntos.front());
            filaProntosReserva.push(filaProntos.front());
            filaProntos.pop();
        }
    }
    cpu.reiniciar();

    this_thread::sleep_for(chrono::milliseconds(500));
    sleep(1);
    cout << "\n_________________________________________________________________________________________\n\n";

    auto inicio1 = high_resolution_clock::now(); // Tempo inicial

    verificaFCFS = true;
    escalonador.executarFCFS();

    string fcfs = "data/FCFSoutput.txt";
    salvarProcessosEmArquivo(fcfs);

    auto fim1 = high_resolution_clock::now();
    auto duracao1 = duration_cast<microseconds>(fim1 - inicio1);

    cout << "\nTempo FCFS: " << duracao1.count() / 1e6 << " segundos\n\n";
    // FCFS(escalonador);

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
    cpu.reiniciar();

    this_thread::sleep_for(chrono::milliseconds(500));
    sleep(1);
    verificaFCFS = true;
    cout << "_________________________________________________________________________________________\n\n";

    auto inicio4 = high_resolution_clock::now();

    criaHashSimbolos();
    processarInput();

    escalonador.executarMenorJobPrimeiro();

    string sjf = "data/SJFoutput.txt";
    salvarProcessosEmArquivo(sjf);

    auto fim4 = high_resolution_clock::now();

    auto duracao4 = duration_cast<microseconds>(fim4 - inicio4);
    cout << "\nTempo Menor Job Primeiro " << duracao4.count() / 1e6 << " segundos\n\n";
    // SJF(escalonador);


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
    cpu.reiniciar();
    this_thread::sleep_for(chrono::milliseconds(500));
    sleep(1);
    verificaFCFS = false;
    cout << "_________________________________________________________________________________________\n\n";

//     {
//         lock_guard<mutex> lock(queueMutex);

//         queue<PCB> filaTemporaria = filaProntosReserva;

//         while (!filaTemporaria.empty())
//         {
//             // Adiciona o processo ao escalonador
//             escalonador.adicionarProcesso(filaTemporaria.front());
//             filaTemporaria.pop();
//         }
//     }

    auto inicio2 = high_resolution_clock::now(); // Tempo inicial

    escalonador.executarRoundRobin();

    string rr = "data/RRoutput.txt";
    salvarProcessosEmArquivo(rr);

    auto fim2 = high_resolution_clock::now();
    auto duracao2 = duration_cast<microseconds>(fim2 - inicio2);
    cout << "\nTempo Round Robin: " << duracao2.count() / 1e6 << " segundos\n\n";

    // RoundRobin(escalonador);


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
    cpu.reiniciar();

    this_thread::sleep_for(chrono::milliseconds(500));
    sleep(1);
    cout << "_________________________________________________________________________________________\n\n";
    verificaFCFS = false;

    // {
    //     lock_guard<mutex> lock(queueMutex);

    //     queue<PCB> filaTemporaria = filaProntosReserva;

    //     while (!filaTemporaria.empty())
    //     {
    //         // Adiciona o processo ao escalonador
    //         escalonador.adicionarProcesso(filaTemporaria.front());
    //         filaTemporaria.pop();
    //     }
    // }

    auto inicio3 = high_resolution_clock::now(); // Tempo inicial

    escalonador.executarPrioridade();

    string p = "data/Poutput.txt";
    salvarProcessosEmArquivo(p);

    auto fim3 = high_resolution_clock::now();
    auto duracao3 = duration_cast<microseconds>(fim3 - inicio3);
    cout << "\nTempo Prioridade: " << duracao3.count() / 1e6 << " segundos\n\n";
    // Prioridade(escalonador);

    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<microseconds>(fim - inicio);
    cout << "_________________________________________________________________________________________\n\n";

    cout << "\nTempo de execução total: " << duracao.count() / 1e6 << " segundos\n\n";

    return 0;
}
