#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include "include.hpp"

void printProcessInfo(const PCB &processo);
void criarProcesso(const string &filePath);
void processarInput();
bool verificaQuantum(PCB &processo);
void criaHashSimbolos();

void salvarProcessosEmArquivo(string &nomeArquivo);

void executarEscalonadoresNaoPreemptivosLRU();
void executarEscalonadoresNaoPreemptivosFIFO();

void executarEscalonadoresPreemptivosLRU();
void executarEscalonadoresPreemptivosFIFO();

#endif
