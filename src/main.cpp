#include "include.hpp"
#include "escalonador.hpp"
#include "functions.hpp"
#include "dicionario.hpp"

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

int CLOCK;
int numeroProcessos;

bool verificaIf;
bool verificaFCFS;
bool verificaFIFO;
bool similaridadeOkay;

unordered_map<string, int> cache;

list<string> cacheOrder;
size_t CACHE_SIZE = 5;
recursive_mutex cacheMutex;

unordered_set<std::string> chavesEmProcessamento;

std::unordered_map<std::string, int> LSH;
std::unordered_map<std::string, int> FIFO;

void criarProcessoThread(const string &filePath)
{
    criarProcesso(filePath);
}

int main()
{
    std::srand(std::time(nullptr));

    auto inicio = high_resolution_clock::now();

    for (volatile int i = 0; i < 100000000; ++i)
        ;

    string dataPath = "./data";
    verificaFCFS = false;
    verificaFIFO = false;
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

    //cout << "\n######################################### NÃO PREEMPTIVOS #########################################\n\n";

    // cout << "\n\n--------------------- EXECUTANDO LRU ---------------------------------\n\n";

    executarEscalonadoresNaoPreemptivosLRU();

    // cout << "\n\n--------------------- EXECUTANDO FIFO ---------------------------------\n\n";

    executarEscalonadoresNaoPreemptivosFIFO();


    // cout << "\n\n\n\n\n######################################### PREEMPTIVOS #########################################\n\n";
    // cout << "\n\n--------------------- EXECUTANDO LRU ---------------------------------\n\n";

    executarEscalonadoresPreemptivosLRU();

    // cout << "\n\n--------------------- EXECUTANDO FIFO ---------------------------------\n\n";
    executarEscalonadoresPreemptivosFIFO();

    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<microseconds>(fim - inicio);
    cout << "_________________________________________________________________________________________\n\n";

    cout << "\nTempo de execução total: " << duracao.count() / 1e6 << " segundos\n\n";

    cout << "_________________________________________________________________________________________\n\n";

    return 0;
}
