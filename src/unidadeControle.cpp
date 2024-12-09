#include "unidadeControle.hpp"
#include "pipeline.hpp"
#include <sstream>
#include <iostream>
#include <thread>
#include <vector>
#include <fstream>

// Construtor padrão: inicializa com 10 registradores zerados
UnidadeControle::UnidadeControle() : registers(10, 0) {}

// Construtor que lê o arquivo e envia cada linha para o pipeline
UnidadeControle::UnidadeControle(const std::string &filename) {
    registers.resize(10, 0); // Garantindo a inicialização de registradores

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return;
    }

    std::string line;

    // Ler cada linha e enviar instruções para o pipeline sequencialmente
    while (std::getline(file, line)) {
        std::cout << "UnidadeControle: Enviando instrução para o pipeline: " << line << std::endl;
        processInstruction(line);
    }

    file.close();
}

// Implementação do construtor solicitado
UnidadeControle::UnidadeControle(int *registradores, const std::string &linha) {
    // Copiar os valores dos registradores
    registers.assign(registradores, registradores + 10);
    std::cout << "UnidadeControle: Construtor chamado com dados do registrador e linha: " << linha << std::endl;
}

// Processar cada instrução enviando para o pipeline de forma controlada
void UnidadeControle::processInstruction(const std::string &instruction) {
    cout << "Antes de InstructionFetch" << endl;
    InstructionFetch(registers.data(), instruction);
    cout << "Depois de InstructionFetch" << endl;
}
