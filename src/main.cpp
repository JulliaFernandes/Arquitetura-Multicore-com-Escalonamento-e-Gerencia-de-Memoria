#include <iostream>
#include <filesystem>
#include <vector>
#include <thread>
#include "include.hpp"
#include "unidadeControle.hpp"

namespace fs = std::filesystem;

int PC = 0;
int CLOCK = 0;

queue<PCB> filaProntos;
queue<PCB> filaBloqueados;

mutex queueMutex;

unordered_map<int, int> cache;
vector<int> principal;
vector<vector<int>> disco;

int main() {
    std::string dataPath = "./data"; // Diretório onde os arquivos estão armazenados
    std::vector<std::thread> threads;

    // Itera pelos arquivos no diretório "data"
    for (const auto &entry : fs::directory_iterator(dataPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".data") {
            // Para cada arquivo, cria uma thread que inicializa uma UnidadeControle
            threads.emplace_back([filePath = entry.path().string()]() {
                std::cout << "Processando arquivo: " << filePath << std::endl;
                UnidadeControle unidade(filePath); // Cada unidade lê e processa o arquivo
            });
        }
    }

    // Aguarda todas as threads concluírem
    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "Todos os arquivos foram processados." << std::endl;
    return 0;
}

 
 
 
 