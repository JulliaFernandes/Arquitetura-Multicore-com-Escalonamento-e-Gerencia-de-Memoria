#include "pipeline.hpp"
#include "ula.hpp"
#include <iostream>
#include <sstream>
#include <mutex>

// Adicionando controle de exclusão mútua global para evitar conflitos
std::mutex pipelineMutex;

void WriteBack(int resultado) {
    std::cout << "Entrando na WriteBack" << std::endl;
    std::cout << "Resultado a ser gravado: " << resultado << std::endl;
    
    principal.push_back(resultado);
    CLOCK++;
    
    std::cout << "Saindo da WriteBack, CLOCK: " << CLOCK << std::endl;
}

void MemoryAccess(int resultado, int *registradores, int info1) {
    std::cout << "Entrando na MemoryAccess" << std::endl;
    std::cout << "Resultado da ULA: " << resultado << std::endl;
    std::cout << "Registradores antes de atualização: " << registradores[info1] << std::endl;
    
    std::lock_guard<std::mutex> lock(pipelineMutex); // Bloqueio no nível mais alto
    std::cout << "Mutex bloqueado na MemoryAccess" << std::endl;

    registradores[info1] = resultado;
    std::cout << "Registradores após atualização: " << registradores[info1] << std::endl;
    std::cout << "Antes de WriteBack" << std::endl;
    
    WriteBack(resultado);

    std::cout << "Depois de WriteBack" << std::endl;
    CLOCK++;
    std::cout << "Saindo da MemoryAccess, CLOCK: " << CLOCK << std::endl;
}


void Execute(char instrucao, int info1, int info2, int info3, std::string info4, int *registradores) {
    std::cout << "Entrando na Execute" << std::endl;
    std::cout << "Instrução: " << instrucao << ", info1: " << info1 << ", info2: " << info2 << ", info3: " << info3 << ", info4: " << info4 << std::endl;

    int soma = 0, registradorAtual = info1;

    if (instrucao == '=') {
        std::cout << "Atribuindo " << info2 << " a registrador[" << info1 << "]" << std::endl;
        registradores[info1] = info2;
    } else if (instrucao == '@') {
        std::cout << "Somando registradores de " << info1 << " até " << info2 << " por " << info3 << " vezes." << std::endl;
        for (int i = 0; i < info3; i++) {
            soma += registradores[registradorAtual];
            registradorAtual++;
            if (registradorAtual > info2) {
                registradorAtual = info1;
            }
            CLOCK++;
        }
        MemoryAccess(soma, registradores, info1);
    } else if ((instrucao != '&') && (instrucao != '?')) {
        std::cout << "Calculando ULA com instrução: " << instrucao << std::endl;
        int resultado = ULA(registradores[info2], registradores[info3], instrucao);
        MemoryAccess(resultado, registradores, info1);
        CLOCK++;
    } else if (instrucao == '?') {
        std::cout << "Comparando registradores[" << info1 << "] e registradores[" << info2 << "] com operação: " << info4 << std::endl;
        std::cout << registradores[info1] << " " << info4 << " " << registradores[info2] << ": ";
        if (info4 == "<") {
            std::cout << (registradores[info1] < registradores[info2] ? "True" : "False") << std::endl;
        } else if (info4 == ">") {
            std::cout << (registradores[info1] > registradores[info2] ? "True" : "False") << std::endl;
        } else if (info4 == "=") {
            std::cout << (registradores[info1] == registradores[info2] ? "True" : "False") << std::endl;
        } else if (info4 == "!") {
            std::cout << (registradores[info1] != registradores[info2] ? "True" : "False") << std::endl;
        }
        CLOCK++;
    }

    std::cout << "Saindo da Execute" << std::endl;
    std::cout << "\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa\n";
}

void InstructionDecode(char instrucao, int info1, int info2, int info3, std::string info4, int *registradores) {
    std::cout << "Entrando na InstructionDecode" << std::endl;
    std::cout << "Instrução: " << instrucao << ", info1: " << info1 << ", info2: " << info2 << ", info3: " << info3 << ", info4: " << info4 << std::endl;
    
    Execute(instrucao, info1, info2, info3, info4, registradores);
    
    std::cout << "Saindo da InstructionDecode" << std::endl;
    CLOCK++;
}


void InstructionFetch(int *registradores, std::string linha) {
    char instrucao;
    int info1 = 0, info2 = 0, info3 = 0;
    std::string info4;

    std::stringstream ss(linha);
    ss >> instrucao >> info1;

    if (instrucao != '&') {
        ss >> info2;
    }
    if ((instrucao != '=') && (instrucao != '?')) {
        ss >> info3;
    }
    if (instrucao == '?') {
        ss >> info4;
    }

    std::cout << "Entrando na InstructionFetch" << std::endl;
    std::cout << "Processando linha: " << linha << std::endl;
    std::cout << "Instrução: " << instrucao << ", info1: " << info1 << ", info2: " << info2 << ", info3: " << info3 << ", info4: " << info4 << std::endl;
    
    InstructionDecode(instrucao, info1, info2, info3, info4, registradores);

    std::cout << "Saindo da InstructionFetch" << std::endl;
    PC++;
    CLOCK++;
}

