#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "include.hpp"

// Com preempção

void WriteBack(int resultado, PCB &processo);
void MemoryAccess(int resultado, int info1, PCB &processo, string &informacoes);
void Execute(char instrucao, int info1, int info2, int info3, string info4, PCB &processo);
void InstructionDecode(char instrucao, int info1, int info2, int info3, string info4, PCB &processo);
void InstructionFetch(string linha, PCB &processo);

// Sem preempção

void WriteBackFCFS(int resultado, PCB &processo);
void MemoryAccessFCFS(int resultado, int info1, PCB &processo, string &informacoes);
void ExecuteFCFS(char instrucao, int info1, int info2, int info3, string info4, PCB &processo);
void InstructionDecodeFCFS(char instrucao, int info1, int info2, int info3, string info4, PCB &processo);
void InstructionFetchFCFS(string linha, PCB &processo);

#endif