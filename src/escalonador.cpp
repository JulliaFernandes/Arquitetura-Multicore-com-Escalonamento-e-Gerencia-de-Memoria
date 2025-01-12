#include "escalonador.hpp"
#include "cpu.hpp"
#include "functions.hpp"

void Escalonador::adicionarProcesso(PCB processo)
{
    filaPCB.push_back(processo);
    filaReserva.push_back(processo);
}

void Escalonador::executarFCFS()
{

    while (!filaPCB.empty() || !cpu.todosProcessosFinalizados())
    {
        if (!filaPCB.empty())
        {
            PCB processo = filaPCB.front();

            if (cpu.tentarAtribuirProcesso(processo))
            {
                filaPCB.pop_front();
            }
            else
            {
                this_thread::sleep_for(chrono::milliseconds(100));
            }
        }
    }
}

void Escalonador::executarMenorJobPrimeiro()
{
    while (!filaPCB.empty() || !cpu.todosProcessosFinalizados())
    {
        if (!filaPCB.empty())
        {

            auto it = min_element(filaPCB.begin(), filaPCB.end(),
                                  [](const PCB &a, const PCB &b)
                                  {
                                      return a.tempoTotalExecucao < b.tempoTotalExecucao;
                                  });

            PCB processo = *it;
            filaPCB.erase(it);

            this_thread::sleep_for(chrono::milliseconds(500));

            if (cpu.tentarAtribuirProcesso(processo))
            {
                // cout << "Processo " << processo.nome << " executado com tempo de execução " << processo.quantum << " ms." << endl;
            }
            else
            {

                this_thread::sleep_for(chrono::milliseconds(100)); // Aguarda antes de tentar novamente
            }
        }
    }
}

void Escalonador::executarRoundRobin()
{

    while (!filaPCB.empty() || !cpu.todosProcessosFinalizados())
    {
        if (!filaPCB.empty())
        {
            PCB processo = filaPCB.front();

            if (cpu.Mod1tentarAtribuirProcesso(processo))
            {
                filaPCB.pop_front();
            }
            else
            {

                this_thread::sleep_for(chrono::milliseconds(100)); // Aguarda antes de tentar novamente
            }
        }
    }
}

void Escalonador::executarPrioridade()
{

    while (!filaPCB.empty() || !cpu.todosProcessosFinalizados())
    {
        if (!filaPCB.empty())
        {

            auto it = min_element(filaPCB.begin(), filaPCB.end(),
                                  [](const PCB &a, const PCB &b)
                                  {
                                      return a.prioridade < b.prioridade;
                                  });

            PCB processo = *it;
            filaPCB.erase(it);

            if (cpu.Mod1tentarAtribuirProcesso(processo))
            {
                // cout << "Processo " << processo.nome << " executado com prioridade " << processo.prioridade << endl;
            }
            else
            {
                this_thread::sleep_for(chrono::milliseconds(100));
            }
        }
    }
}
