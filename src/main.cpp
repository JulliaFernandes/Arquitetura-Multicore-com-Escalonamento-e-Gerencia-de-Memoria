#include "include.hpp"
#include "escalonador.hpp"
#include "functions.hpp"

using namespace std;
using namespace chrono;

namespace fs = std::filesystem;

mutex queueMutex;  // Protege o acesso à fila de prontos
queue<PCB> filaProntos;        

unordered_map<int, int> cache;
vector<int> principal;
vector<vector<int>> disco;
int CLOCK;
bool verificaIf;
mutex printMutex;

deque<PCB> filaPCB;

map<int, int> memoria;
map<int, PCB> processosAtuais;
std::mutex processosMutex;  

int numeroProcessos;

void criarProcessoThread(const string &filePath) {
    criarProcesso(filePath); 
}

int main() {

    auto inicio = high_resolution_clock::now(); // Tempo inicial

    // Código cuja execução você quer medir
    for (volatile int i = 0; i < 100000000; ++i);

    string dataPath = "./data";

    vector<thread> threads;

    CPU();

    // Cria threads para preparar os processos
    for (const auto &entry : fs::directory_iterator(dataPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".data") {
            string filePath = entry.path().string();
            numeroProcessos++;
            threads.emplace_back(criarProcessoThread, filePath); // Cada arquivo é gerenciado em uma thread
        }
    }

    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

   // cout << "\nTodos os processos foram preparados e adicionados à fila de prontos.\n";

    Escalonador escalonador;

    // Transfere os processos da fila de prontos para o escalonador
    {
        lock_guard<mutex> lock(queueMutex);
        while (!filaProntos.empty()) {
            escalonador.adicionarProcesso(filaProntos.front());
            filaProntos.pop();
        }
    }

    escalonador.executar();

    gerarOutputFinal();

    auto fim = high_resolution_clock::now(); 

    auto duracao = duration_cast<microseconds>(fim - inicio);
    cout << "\nTempo de execução: " << duracao.count() / 1e6 << " segundos\n\n";

    return 0;
}


