#ifndef INCLUDE_HPP
#define INCLUDE_HPP

#include "bibliotecas.hpp"

#define NUM_PERIFERICOS 5

using namespace std;

extern int idGlobal;
extern int CLOCK;
extern bool verificaIf;

extern unordered_map<int, int> cache;
extern vector<int> principal;
extern vector<vector<int>> disco;

extern bool perifericos[NUM_PERIFERICOS];

enum ProcessState { PRONTO, RODANDO, BLOQUEADO, CONCLUIDO};

// Process Control Block
struct PCB {
    int ID;
    ProcessState estado;
    int prioridade;
    int baseEndereco;
    int limiteEndereco;
    int quantum;
    int timestamp;
    string nome;
    int PC;
    vector<int> registers;
    short int numPipeline;

    PCB () : numPipeline(0) {}

    PCB(int id, ProcessState s, int p, int base, int limit, int quantum, int time, string nome, int PC)
    : ID(id), estado(s), prioridade(p), baseEndereco(base), 
      limiteEndereco(limit), quantum(quantum), timestamp(time), nome(nome), PC(PC), registers(10, 0) {}

};

extern queue<PCB> filaProntos;

extern mutex queueMutex;
extern mutex printMutex;   

extern map<int, int> memoria;
extern map<int, PCB> processosAtuais;
extern std::mutex processosMutex;  

#endif