#include "thread.hpp"
#include "unidadeControle.hpp"
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>

// Instância global da UnidadeControle
UnidadeControle unidadeControle;

void processInstruction(const std::string &instruction) {
    unidadeControle.processInstruction(instruction); // Delegar à UnidadeControle
}

void executeInstructions(const std::string &filePath) {
    std::ifstream inputFile(filePath);
    std::string line;

    if (!inputFile.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    std::vector<std::thread> threads;

    // Ler cada linha do arquivo e criar uma thread para processá-la
    while (std::getline(inputFile, line)) {
        threads.emplace_back(std::thread(processInstruction, line));
    }

    // Aguardar todas as threads terminarem
    for (auto &t : threads) {
        t.join();
    }

    inputFile.close();
}
