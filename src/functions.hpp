#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include "include.hpp"

void printProcessInfo(const PCB &processo);
void criarProcesso(const string &filePath);
void processarInput();
bool verificaQuantum(PCB &processo);
void criaHashSimbolos();

void salvarProcessosEmArquivo(string &nomeArquivo);
// void FCFS(Escalonador &escalonador);
// void SJF(Escalonador &escalonador);
// void RoundRobin(Escalonador &escalonador);
// void Prioridade(Escalonador &escalonador);

#endif
