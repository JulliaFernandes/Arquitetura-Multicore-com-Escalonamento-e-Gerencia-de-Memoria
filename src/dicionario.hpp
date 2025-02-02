#ifndef DICIONARIO_HPP
#define DICIONARIO_HPP

#include "bibliotecas.hpp"
#include "include.hpp"

void printCacheAfterInsertLRU();
void printCacheAfterInsertFIFO();

int calcularOperacao(char operacao, int operando1, int operando2);
void carregarDicionario(const std::string &filePath);
void exibirDicionario();
double calcularSimilaridade(const string &instrucao1, const string &instrucao2);

bool verificarCacheComLSH(const std::string &chave, int &resultado);
void transferirLSHParaCache();
void atualizarCache(const std::string &chave, int resultado);
void exibirCache();
void exibirCacheOrder();

#endif
