#ifndef UNIDADECONTROLE_HPP
#define UNIDADECONTROLE_HPP

#include "include.hpp"
#include "pipeline.hpp" 

class UnidadeControle {

    public:

        UnidadeControle();

        // Construtor para tratar arquivos
        explicit UnidadeControle(PCB &processo);

        void lerAteLinha(ifstream &file, int linhaAlvo);
        void processarArquivo(PCB &processo);

};

#endif
