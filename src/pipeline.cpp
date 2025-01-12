#include "pipeline.hpp"
#include "ula.hpp"
#include "escalonador.hpp"
#include "functions.hpp"

using namespace std;

mutex pipelineMutex;

void WriteBack(int resultado, PCB &processo)
{

    CLOCK++;
    processo.timestamp++;
    processo.quantum--;
    processo.numPipeline = 5;

    // cout << "\n################# Resultado gravado: " << resultado << endl;

    principal.push_back(resultado);
    memoria[processo.ID] = resultado;
}

void MemoryAccess(int resultado, int info1, PCB &processo)
{

    // cout << "\n------------- Resultado ULA: " << resultado << endl;

    lock_guard<mutex> lock(pipelineMutex);

    CLOCK++;
    processo.timestamp++;
    processo.quantum--;
    processo.numPipeline = 4;

    if (verificaQuantum(processo))
    {
        verificaIf = true;
        processo.estado = BLOQUEADO;
        processosAtuais[processo.ID] = processo;
        return;
    }
    processo.registers[info1] = resultado;
    WriteBack(resultado, processo);
}

void Execute(char instrucao, int info1, int info2, int info3, string info4, PCB &processo)
{

    int soma = 0, registradorAtual = info1;

    if (instrucao == '=')
    {

        processo.registers[info1] = info2;

        CLOCK++;
        processo.timestamp++;
        processo.quantum--;
        processo.numPipeline = 3;
    }
    else if (instrucao == '@')
    {

        for (int i = 0; i < info3; i++)
        {

            soma += processo.registers[registradorAtual];
            registradorAtual++;
            if (registradorAtual > info2)
            {
                registradorAtual = info1;
            }

            CLOCK++;
            processo.timestamp++;
            processo.quantum--;
            processo.numPipeline = 3;

            if (verificaQuantum(processo))
            {
                processo.estado = BLOQUEADO;
                processosAtuais[processo.ID] = processo;
                verificaIf = true;
                return;
            }
        }

        MemoryAccess(soma, info1, processo);
    }
    else if ((instrucao != '&') && (instrucao != '?'))
    {

        int resultado = ULA(processo.registers[info2], processo.registers[info3], instrucao);

        CLOCK++;
        processo.timestamp++;
        processo.quantum--;
        processo.numPipeline = 3;

        if (verificaQuantum(processo))
        {
            verificaIf = true;
            processo.estado = BLOQUEADO;
            processosAtuais[processo.ID] = processo;
            return;
        }

        MemoryAccess(resultado, info1, processo);
    }
    else if (instrucao == '?')
    {

        if (info4 == "<")
        {

            memoria[processo.ID] = (processo.registers[info1] < processo.registers[info2] ? 1 : 0);
        }
        else if (info4 == ">")
        {

            memoria[processo.ID] = (processo.registers[info1] > processo.registers[info2] ? 1 : 0);
        }
        else if (info4 == "=")
        {

            memoria[processo.ID] = (processo.registers[info1] == processo.registers[info2] ? 1 : 0);
        }
        else if (info4 == "!")
        {

            memoria[processo.ID] = (processo.registers[info1] != processo.registers[info2] ? 1 : 0);
        }

        processo.timestamp++;
        processo.quantum--;
        CLOCK++;
        processo.numPipeline = 3;

        if (verificaQuantum(processo))
        {
            verificaIf = true;
            processo.estado = BLOQUEADO;
            processosAtuais[processo.ID] = processo;

            return;
        }
    }
}

void InstructionDecode(char instrucao, int info1, int info2, int info3, string info4, PCB &processo)
{

    CLOCK++;
    processo.timestamp++;
    processo.quantum--;
    processo.numPipeline = 2;

    if (verificaQuantum(processo))
    {
        verificaIf = true;
        processo.estado = BLOQUEADO;
        processosAtuais[processo.ID] = processo;
        return;
    }

    Execute(instrucao, info1, info2, info3, info4, processo);
}

void InstructionFetch(string linha, PCB &processo)
{

    char instrucao;
    int info1 = 0, info2 = 0, info3 = 0;
    string info4;

    // cout << endl;

    stringstream ss(linha);
    ss >> instrucao >> info1;

    if (instrucao != '&')
    {
        ss >> info2;
    }
    if ((instrucao != '=') && (instrucao != '?'))
    {
        ss >> info3;
    }
    if (instrucao == '?')
    {
        ss >> info4;
    }

    // cout << "\n 1- Instrução: " << instrucao << ", info1: " << info1 << ", info2: " << info2 << ", info3: " << info3 << ", info4: " << info4 << endl;

    processo.timestamp++;
    processo.quantum--;
    CLOCK++;

    if (verificaQuantum(processo))
    {
        verificaIf = true;
        processo.estado = BLOQUEADO;
        processosAtuais[processo.ID] = processo;
        return;
    }

    processo.numPipeline = 1;
    InstructionDecode(instrucao, info1, info2, info3, info4, processo);
}

///////////////////////////////////////////////////////// SEM PREEMPÇÃO //////////////////////////////////////////////////////////

void WriteBackFCFS(int resultado, PCB &processo)
{

    CLOCK++;
    processo.timestamp++;
    processo.numPipeline = 5;

    // cout << "\n################# Resultado gravado: " << resultado << endl;

    principal.push_back(resultado);
    memoria[processo.ID] = resultado;
}

void MemoryAccessFCFS(int resultado, int info1, PCB &processo)
{

    // cout << "\n------------- Resultado ULA: " << resultado << endl;

    lock_guard<mutex> lock(pipelineMutex);

    processo.registers[info1] = resultado;

    CLOCK++;
    processo.timestamp++;
    WriteBackFCFS(resultado, processo);
}

void ExecuteFCFS(char instrucao, int info1, int info2, int info3, string info4, PCB &processo)
{

    int soma = 0, registradorAtual = info1;
    // cout << "\n 3- Instrução: " << instrucao << ", info1: " << info1 << ", info2: " << info2 << ", info3: " << info3 << ", info4: " << info4 << endl;

    if (instrucao == '=')
    {
        // cout << "\n Atribuindo " << info2 << " a registrador[" << info1 << "]" << endl;
        processo.registers[info1] = info2;

        CLOCK++;
        processo.timestamp++;
    }
    else if (instrucao == '@')
    {

        for (int i = 0; i < info3; i++)
        {
            soma += processo.registers[registradorAtual];
            registradorAtual++;
            if (registradorAtual > info2)
            {
                registradorAtual = info1;
            }
            // cout << "Soma: " << soma << endl;
            CLOCK++;
            processo.timestamp++;
        }

        MemoryAccessFCFS(soma, info1, processo);
    }
    else if ((instrucao != '&') && (instrucao != '?'))
    {

        int resultado = ULA(processo.registers[info2], processo.registers[info3], instrucao);

        CLOCK++;
        processo.timestamp++;
        MemoryAccessFCFS(resultado, info1, processo);
    }
    else if (instrucao == '?')
    {

        // cout << processo.registers[info1] << " " << info4 << " " << processo.registers[info2] << ": ";

        if (info4 == "<")
        {

            memoria[processo.ID] = (processo.registers[info1] < processo.registers[info2] ? 1 : 0);
        }
        else if (info4 == ">")
        {

            memoria[processo.ID] = (processo.registers[info1] > processo.registers[info2] ? 1 : 0);
        }
        else if (info4 == "=")
        {

            memoria[processo.ID] = (processo.registers[info1] == processo.registers[info2] ? 1 : 0);
        }
        else if (info4 == "!")
        {

            memoria[processo.ID] = (processo.registers[info1] != processo.registers[info2] ? 1 : 0);
        }

        processo.timestamp++;
        CLOCK++;
    }
}

void InstructionDecodeFCFS(char instrucao, int info1, int info2, int info3, string info4, PCB &processo)
{
    // cout << "\n 2- Instrução: " << instrucao << ", info1: " << info1 << ", info2: " << info2 << ", info3: " << info3 << ", info4: " << info4 << endl;
    CLOCK++;
    processo.timestamp++;

    ExecuteFCFS(instrucao, info1, info2, info3, info4, processo);
}

void InstructionFetchFCFS(string linha, PCB &processo)
{

    char instrucao;
    int info1 = 0, info2 = 0, info3 = 0;
    string info4;

    // cout << endl;

    stringstream ss(linha);
    ss >> instrucao >> info1;

    if (instrucao != '&')
    {
        ss >> info2;
    }
    if ((instrucao != '=') && (instrucao != '?'))
    {
        ss >> info3;
    }
    if (instrucao == '?')
    {
        ss >> info4;
    }

    // cout << "\n 1- Instrução: " << instrucao << ", info1: " << info1 << ", info2: " << info2 << ", info3: " << info3 << ", info4: " << info4 << endl;

    processo.timestamp++;
    CLOCK++;

    InstructionDecodeFCFS(instrucao, info1, info2, info3, info4, processo);
}
