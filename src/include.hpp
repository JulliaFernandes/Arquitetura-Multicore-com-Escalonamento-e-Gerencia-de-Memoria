#ifndef INCLUDE_HPP
#define INCLUDE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <queue>
#include <mutex>
#include <pthread.h>
#include <thread>
#include <unistd.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <thread>

#define NUM_PERIFERICOS 5
#define NUM_THREADS 5

using namespace std;

extern int PC;
extern int CLOCK;

extern unordered_map<int, int> cache;
extern vector<int> principal;
extern vector<vector<int>> disco;

extern bool perifericos[NUM_PERIFERICOS];

// Estados dos processos
enum ProcessState { PRONTO, RODANDO, BLOQUEADO };

// Process Control Block

struct PCB {
    int ID;
    ProcessState estado;
    int prioridade;
    int baseEndereco; // Endereço base de memória
    int limiteEndereco;  // Limite de alocação de memória
    int quantum;
    int timestamp;
    pthread_t threads[NUM_THREADS]; 

    PCB(int id, ProcessState s, int p, int base, int limit, int quantum, int time)
        : ID(id), estado(s), prioridade(p),
          baseEndereco(base), limiteEndereco(limit), quantum(quantum), 
          timestamp(time) {}
};

struct Core {
    int id;                 // ID do núcleo
    PCB* processoAtual; // Processo em execução no núcleo
};


extern queue<PCB> filaProntos;
extern queue<PCB> filaBloqueados;

extern mutex queueMutex;


#endif