<h1 align="center" font-size="200em"><b>Desenvolvimento e Implementação de uma Arquitetura Multicore com Escalonamento e Gerência de Memória</b></h1>

<div align = "center" >
<!-- imagem -->

[![requirement](https://img.shields.io/badge/IDE-Visual%20Studio%20Code-informational)](https://code.visualstudio.com/docs/?dv=linux64_deb)
![Make](https://img.shields.io/badge/Compilacao-Make-orange)
![Linguagem](https://img.shields.io/badge/Linguagem-C%2B%2B-blue)
</div>

## Descrição

Este projeto foca no desenvolvimento de um sistema baseado em uma arquitetura multicore com execução de tarefas preemptivas. O sistema implementa a preempção quando o quantum atinge o valor zero, pausando o processo e retornando-o para a fila, mantendo-o no estado de bloqueado. O uso do mutex é essencial para evitar acessos concorrentes às regiões críticas, como o acesso ao Core disponível. A simulação foi projetada para demonstrar o gerenciamento eficaz de processos concorrenciais até o término do arquivo.

## Instruções do Arquivo .data

É necessário seguir os modelos de instruções descritos abaixo no arquivo .data para garantir o funcionamento correto do código e da arquitetura.

### Atribuição de Valores aos Registradores

#### Operação `=`  
O símbolo `=` é utilizado para atribuir um valor a um registrador. O primeiro termo representa o registrador e o segundo termo o valor atribuído. Por exemplo, `= 0 6` indica que o registrador de número 0 receberá o valor 6.

### Operações Matemáticas

#### Operações Matemáticas  
As operações matemáticas são representadas pelos símbolos `+`, `/`, `*`, e `-`. Elas correspondem, respectivamente, às operações de adição, subtração, multiplicação e divisão. Essas operações são realizadas utilizando registradores como operandos. Por exemplo, `+ 0 1 0` indica que será realizada uma soma, com o resultado sendo salvo no registrador 0, utilizando os valores dos registradores 1 e 0.

### Loop `while` - Operação `@`

#### Operação `@`  
O símbolo `@` corresponde ao loop `while`. Por exemplo, `@ 0 1 4` significa somar os valores dos registros começando pelo índice 0 até o índice 1, repetindo essa operação 4 vezes de forma circular. O resultado final é salvo no primeiro registrador utilizado.

### Operação `if` - Operação `?`

#### Operação `?`  
A operação `if` é representada pelo símbolo `?`. Por exemplo, `? 0 1 <$` avalia se o valor no registrador 0 é menor que o valor no registrador 1. O resultado dessa operação é armazenado na memória principal, retornando 1 para verdadeiro (true) e 0 para falso (false).

## Fase 1: Arquitetura Multicore e Suporte a Preempção

### Objetivo

O presente trabalho tem por objetivo desenvolver um sistema que atua como uma arquitetura multicore, com execução de tarefas preemptivas, proporcionando uma simulação realista do comportamento de um sistema operacional.

### Metodologia

Os arquivos de entrada são denominados input.txt. Cada linha do arquivo é uma instrução a ser processada. Cada input representa um processo, que é gerenciado por uma thread separada. Para armazenar as informações de cada processo, é utilizada uma estrutura denominada PCB (Process Control Block). 

#### Ferramentas e Linguagem  
O trabalho foi implementado na linguagem de programação C++, utilizando o ambiente de desenvolvimento Visual Studio Code. As bibliotecas usadas incluem thread, queue, mutex, entre outras, para gerenciar processos concorrentes e a comunicação entre threads.

#### Estrutura de Dados  
Para a coordenação das informações dos processos, foi criada uma estrutura de dados denominada PCB (Process Control Block). Essa estrutura armazena informações detalhadas de cada processo, como o PC (Program Counter), que indica a instrução a ser executada; o timestamp, que registra a última modificação no estado do processo; o quantum, que representa o tempo de CPU que o processo possui; o estado do processo, identificando se está pronto, rodando, bloqueado ou concluído; o nome do arquivo associado ao processo; o ID do processo; e os valores dos registradores.

#### Fluxo de Processo  
O sistema inicia pela leitura de arquivos .data, que estão armazenados na pasta data do projeto. A leitura é realizada a partir da função main do programa, sendo cada arquivo processado por uma thread separada. Cada linha dos arquivos é interpretada como uma instrução que será processada pelo pipeline do sistema. Durante esse processamento inicial, as informações de cada arquivo são armazenadas em uma instância da estrutura PCB.

Após o processamento inicial, os processos são inseridos em uma fila de processos, que será utilizada para determinar a ordem de execução. Para isso, foi adotada inicialmente a política de escalonamento First-Come-First-Served (FCFS), garantindo que os processos sejam executados na ordem em que foram adicionados à fila.

A execução das tarefas ocorre a partir do escalonador, que verifica se há processos na fila e se existem núcleos disponíveis para processá-los. Caso ambas as condições sejam verdadeiras, o processo é enviado para execução. Durante a execução, as threads processam as instruções contidas no pipeline, chamando a unidade de controle e atualizando as informações do PCB. O quantum e o timestamp do processo atual são alterados, assim como os timestamps dos demais processos que aguardam na fila. 

O processo de execução continua até que todos os processos sejam finalizados. Quando a fila de processos é esvaziada, o trabalho é encerrado e as informações finais de cada processo são exibidas no output. Entre os dados apresentados estão o resultado gravado na memória principal, os valores dos registradores, o ID do processo, o nome do arquivo associado, o quantum final, o estado do processo e seu timestamp.

#### Pólitica de escalonamento  
Essa metodologia foi desenvolvida para garantir uma simulação realista e eficiente do comportamento de um sistema operacional multicore, permitindo a execução de tarefas concorrentes com controle e gerenciamento de processos.

### Resultados

O sistema demonstrou a capacidade de realizar preempção quando o quantum atingia o valor zero, pausando o processo, retornando-o para a fila, mantendo-o no estado de bloqueado. Posteriormente executando-o novamente até o término do arquivo. Além disso, o uso do mutex foi essencial para não permitir que os processos acessassem a região crítica, que nesta etapa era o acesso ao Core disponível.

#### Exemplo de entrada e saída

| Input | Instruções                                     | Descrição                       | Ciclos |
|-------|------------------------------------------------|--------------------------------|--------|
| 1     | `= 0 4`                                        | Atribuir 4 ao registrador 0       | 3      |
|       | `= 1 5`                                        | Atribuir 5 ao registrador 1       | 3      |
|       | `- 0 1 0`                                      | Subtrair dos R 0 e 1 e armazenar em 0 | 5      |
| 2     | `= 0 6`                                        | Atribuir 6 ao registrador 0       | 3      |
|       | `= 1 7`                                        | Atribuir 7 ao registrador 1       | 3      |
|       | `+ 0 1 0`                                      | Somar dos R 0 e 1 e armazenar em 0... |        |











## 👾Compilação e execução
* Especificações da máquina em que o código foi rodado:
  * Processador Intel Core i7, 12th Gen;
  * Sistema Operacional Ubuntu 22.04.5;
  * 16GB de RAM.
* | Comando                |  Função                                                                                           |                     
  | -----------------------| ------------------------------------------------------------------------------------------------- |
  |  `make clean`          | Apaga a última compilação realizada contida na pasta build                                        |
  |  `make`                | Executa a compilação do programa utilizando o gcc, e o resultado vai para a pasta build           |
  |  `make run`            | Executa o programa da pasta build após a realização da compilação                                 |


## Contato
<div>
 <p align="justify"> Anna Laura Moura Santana</p>
 <a href="https://t.me/">
 <img align="center" src="https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white"/> 
 </div>
<a style="color:black" href="mailto:nalauramoura@gmail.com?subject=[GitHub]%20Source%20Dynamic%20Lists">
✉️ <i>nalauramoura@gmail.com</i>
</a>

<
<div>
 <br><p align="justify"> Jullia Fernandes</p>
 <a href="https://t.me/JulliaFernandes">
 <img align="center" src="https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white"/> 
 </div>
<a style="color:black" href="mailto:julliacefet@gmail.com?subject=[GitHub]%20Source%20Dynamic%20Lists">
✉️ <i>julliacefet@gmail.com</i>
</a>

