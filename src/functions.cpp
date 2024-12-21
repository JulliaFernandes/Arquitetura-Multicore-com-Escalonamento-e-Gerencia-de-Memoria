#include "functions.hpp"
#include "escalonador.hpp"

int idGlobal = 1;

void printProcessInfo(const PCB &processo) {

    lock_guard<mutex> lock(printMutex);

    cout << "ID: " << processo.ID << endl;
    cout << "Nome do Arquivo: " << processo.nome << endl;
    cout << "Quantum: " << processo.quantum << endl;
    cout << "Timestamp: " << processo.timestamp << endl;
    cout << "PC: " << processo.PC << endl;
    cout << "--------------------------" << endl;
}


void criarProcesso(const string &filePath) {

    PCB processo; 

    ifstream file(filePath);
    if (file.is_open()) {
       
        processo.ID = idGlobal++;                   
        processo.estado = PRONTO;                  
        processo.prioridade = 1;                   
        processo.baseEndereco = 0;                 
        processo.limiteEndereco = 0;               
       // processo.quantum = 20 + rand() % 31;      
        processo.quantum = 5;     
        processo.timestamp = 0 ;    
        processo.PC = 0;           
        processo.nome = filePath;       
        processo.registers = vector <int> (10,0);  
        processo.numPipeline = 0;         

        // Protege o acesso à fila de prontos
        {
            lock_guard<mutex> lock(queueMutex);
            filaProntos.push(processo); 
            processosAtuais[processo.ID] = processo;
            
        }

        file.close(); 
    }

    //cout << "\n";
    printProcessInfo(processo);
    //cout << "\n";
}


bool verificaQuantum (PCB &processo){
    
    if(processo.quantum == 0){

        //cout << "\n>>>>>>> Processo pausado, quantum = " << processo.quantum << "\n";

        processo.estado = PRONTO;
        processo.quantum = 10;  
       
       // cout << "\n>>>>>>>-Novo quantum = " << processo.quantum << "\n\n";

        processo.PC--;
        lock_guard<mutex> lock(queueMutex);

        filaPCB.push_back(processo);

        //cout << "\n\n\n FILA VERIFICA QUANTUM: " << filaPCB.size() << endl;

        processosAtuais[processo.ID] = processo;

        //  cout << "\n";
        // printProcessInfo(processo);
        // cout << "\n";
        
        return true;
    }

    else return false;
}

void gerarOutputFinal() {
    ofstream file_OutPut("data/output.txt");

    if (!file_OutPut.is_open()) {
        cerr << "[ERRO] Não foi possível criar o arquivo de saída.\n";
        return;
    }

    file_OutPut << "_________________________________________________________________________________________________________________________________________________________________" << endl;
    file_OutPut << "                                                          PROCESSOS       		               		                            " << endl;
    file_OutPut << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    {
        lock_guard<mutex> processosLock(processosMutex);

        for (const auto& [id, processo] : processosAtuais) {
            file_OutPut << "ID: " << id << "\n";
            file_OutPut << "Nome do Arquivo: " << processo.nome << "\n";
            file_OutPut << "Estado: " << (processo.estado == PRONTO ? "PRONTO" : "CONCLUIDO") << "\n";
            file_OutPut << "Quantum: " << processo.quantum << "\n";
            file_OutPut << "TimeStamp: " << processo.timestamp << endl;
            file_OutPut << "PC: " << processo.PC << "\n";
            file_OutPut << "Registros: ";
            for (int reg : processo.registers) {
                file_OutPut << reg << " ";
            }
            file_OutPut << "\n";

            if (memoria.find(id) != memoria.end()) {
                file_OutPut << "Memória Principal (ID: " << id << "): " << memoria[id] << "\n";
            } else {
                file_OutPut << "Memória Principal (ID: " << id << "): Não disponível\n";
            }

            file_OutPut << "====================================================================================================================================================================\n";
        }
    }

    file_OutPut.close();
    cout << "[INFO] Arquivo de saída 'output.txt' gerado com sucesso.\n";
}
