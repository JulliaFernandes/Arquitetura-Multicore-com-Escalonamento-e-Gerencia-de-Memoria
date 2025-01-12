#ifndef UNIDADECONTROLE_HPP
#define UNIDADECONTROLE_HPP

#include "include.hpp"
#include "pipeline.hpp"

class UnidadeControle
{

    public:
        UnidadeControle();
        explicit UnidadeControle(PCB &processo);

        void lerAteLinha(ifstream &file, int linhaAlvo);
        void processarArquivo(PCB &processo);
        void processarArquivoFCFS(PCB &processo);
};

#endif
