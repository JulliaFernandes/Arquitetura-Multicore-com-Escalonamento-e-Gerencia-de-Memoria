#include "escalonador.hpp"
#include "cpu.hpp"

void Escalonador::adicionarProcesso(PCB processo) {
    filaPCB.push_back(processo);
   // cout << "Processo " << processo.nome << " adicionado à fila de prontos.\n";
}

void Escalonador::executar() {
   // cout << "\n\n   Entrou no executar: " << filaPCB.size() << endl;

    while (!filaPCB.empty() || !cpu.todosProcessosFinalizados()) {
        if (!filaPCB.empty()) {
            PCB processo = filaPCB.front(); 

            if (cpu.tentarAtribuirProcesso(processo)) { 
                filaPCB.pop_front();
               
               // cout << "Processo " << processo.nome << " foi atribuído e removido da fila.\n";
            } 
            else {
                //cout << "Falha ao atribuir o processo, tentando novamente...\n";
                this_thread::sleep_for(chrono::milliseconds(100)); // Aguarda antes de tentar novamente
            }
        }
    }

    if(numeroProcessos == NUM_CORES){
        cpu.sincronizarTimestamps();
    }

    //cout << "Todos os processos foram executados.\n";
}

