#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include "include.hpp"
#include "thread.hpp"

PCB criarProcesso(int ID, ProcessState s, int prioridade, int baseEndereco, int limiteEndereco, int quantumInicial, int timestamp);
void LerInstrucoesDoArquivo(const string &nomeArquivo, int *registradores);
void executeProcess(PCB &process);
void preempt_process(Core &core);
void block_process(Core &core, int resource_id);

#endif
