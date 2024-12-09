#ifndef UNIDADECONTROLE_HPP
#define UNIDADECONTROLE_HPP

#include <string>
#include <vector>
#include <mutex>
#include "include.hpp"

class UnidadeControle {
public:
    UnidadeControle();
    UnidadeControle(int *registradores, const std::string &linha); // Novo construtor
    void processInstruction(const std::string &instruction);
    UnidadeControle(const std::string &filename);

private:
    std::vector<int> registers; // Registradores simulados
    std::mutex regMutex;        // Para sincronização de acesso aos registradores
};



#endif
