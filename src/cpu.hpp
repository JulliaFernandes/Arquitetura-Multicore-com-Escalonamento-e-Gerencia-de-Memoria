#ifndef CPU_HPP
#define CPU_HPP

#include "include.hpp"
#include "pipeline.hpp"
#include "unidadeControle.hpp"
#include "dicionario.hpp"

#define NUM_CORES 1

using namespace std;

struct Core
{
    int id;
    PCB *processoAtual;
};

extern int numeroProcessos;

class CPU
{
private:
    vector<Core> cores;
    mutex nucleoMutex;
    condition_variable nucleoDisponivel;

public:
    CPU();

    void inicializarCores();

    void executarProcesso(PCB &processo);
    void liberarNucleo(Core &core);
    void sincronizarTimestamps();

    bool tentarAtribuirProcesso(PCB &processo);
    bool Mod1tentarAtribuirProcesso(PCB &processo);

    bool temNucleoLivre();
    bool todosProcessosFinalizados();
    void reiniciar();
};

#endif
