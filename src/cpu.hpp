#ifndef CPU_HPP
#define CPU_HPP

#include "include.hpp"
#include "pipeline.hpp" 
#include "unidadeControle.hpp"

#define NUM_CORES 1

using namespace std;

struct Core {
    int id;               
    PCB* processoAtual;     // Processo em execução no núcleo 
};

extern int numeroProcessos;

class CPU {
    private:

        vector<Core> cores;     
        mutex nucleoMutex;      // Mutex para proteger acesso aos núcleos
        condition_variable nucleoDisponivel; // Condição para sincronizar disponibilidade dos núcleos

    public:
     
        CPU();

        void inicializarCores();
        void executarProcesso(Core &core, PCB &processo);
        void atualizarTimestamps(int id_aux);
        void liberarNucleo(Core &core);
        void sincronizarTimestamps();

        bool tentarAtribuirProcesso(PCB &processo);
        bool temNucleoLivre();
        bool todosProcessosFinalizados();
};

#endif




