#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

#include "cpu.hpp"
#include "include.hpp"

extern deque<PCB> filaPCB;
class Escalonador
{

private:
    CPU cpu;

public:
    void adicionarProcesso(PCB processo);

    void executarFCFS();
    void executarMenorJobPrimeiro();

    void executarRoundRobin();
    void executarPrioridade();
};

#endif
