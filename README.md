<h1 align="center" font-size="200em"><b>Módulo 3: Gerenciamento de Cache e Escalonamento Baseado em Similaridade</b></h1>

<div align = "center" >
<!-- imagem -->

[![requirement](https://img.shields.io/badge/IDE-Visual%20Studio%20Code-informational)](https://code.visualstudio.com/docs/?dv=linux64_deb)
![Make](https://img.shields.io/badge/Compilacao-Make-orange)
![Linguagem](https://img.shields.io/badge/Linguagem-C%2B%2B-blue)
</div>

## ✒️ Descrição
Este é um programa desenvolvido em C++ para a disciplina de Sistemas Operacionais. Este projeto foca no desenvolvimento de um sistema baseado em uma arquitetura multicore com execução de tarefas preemptivas. O sistema implementa a preempção quando o quantum atinge o valor zero, pausando o processo e retornando-o para a fila, mantendo-o no estado de bloqueado. O uso do mutex é essencial para evitar acessos concorrentes às regiões críticas, como o acesso ao Core disponível. A simulação foi projetada para demonstrar o gerenciamento eficaz de processos concorrenciais até o término do arquivo.

## 💻 Objetivo

Investigar como o gerenciamento de cache pode influenciar o desempenho na execução de tarefas, abordando a similaridade de processos, implementando técnicas para reaproveitamento de dados armazenados e analisando sua integração nas políticas de escalonamento implementadas.

O projeto permite simular, testar e comparar o desempenho de duas políticas de cache, LRU e FIFO através do uso da biblioteca LSH, destacando vantagens e limitações.


## 📄 Instruções do Arquivo .data

É necessário seguir os modelos de instruções descritos abaixo no arquivo .data para garantir o funcionamento correto do código e da arquitetura.

### Atribuição de Valores aos Registradores - Operação `=`  
O símbolo `=` é utilizado para atribuir um valor a um registrador. O primeiro termo representa o registrador e o segundo termo o valor atribuído. Por exemplo, `= 0 6` indica que o registrador de número 0 receberá o valor 6.

### 🔢 Operações Matemáticas 
As operações matemáticas são representadas pelos símbolos `+`, `/`, `*`, e `-`. Elas correspondem, respectivamente, às operações de adição, subtração, multiplicação e divisão. Essas operações são realizadas utilizando registradores como operandos. Por exemplo, `+ 0 1 0` indica que será realizada uma soma, com o resultado sendo salvo no registrador 0, utilizando os valores dos registradores 1 e 0.

### Loop `while` - Operação `@`
O símbolo `@` corresponde ao loop `while`. Por exemplo, `@ 0 1 4` significa somar os valores dos registros começando pelo índice 0 até o índice 1, repetindo essa operação 4 vezes de forma circular. O resultado final é salvo no primeiro registrador utilizado.

### Operação `if` - Operação `?`
A operação `if` é representada pelo símbolo `?`. Por exemplo, `? 0 1 <$` avalia se o valor no registrador 0 é menor que o valor no registrador 1. O resultado dessa operação é armazenado na memória principal, retornando 1 para verdadeiro (true) e 0 para falso (false).

### Pólitica de escalonamento  
Essa metodologia foi desenvolvida para garantir uma simulação realista e eficiente do comportamento de um sistema operacional multicore, permitindo a execução de tarefas concorrentes com controle e gerenciamento de processos.

### Diagrama Arquitetura
O fluxo do programa foi modelado de maneira a integrar os conceitos apresentados, estruturando a execução e gerência de processos, conforme ilustrado no diagrama a seguir.

<div align="center">
  <img src="imgs/fluxo.png" width="50%" height="50%">
  <p align="center"><em> Fluxo da arquitetura </em></p>
</div>

### Diagrama dos Escalonadores ###
O fluxo de execução dos escalonadores foi modelado para ilustrar as diferentes estratégias de escalonamento utilizadas. 

<div align="center">
  <img src="imgs/fluxo2.png" width="50%" height="50%">
  <p align="center"><em> Fluxo de escalonadores </em></p>
</div>

### Diagrama do Uso da Memória Cache ###
O diagrama detalha o fluxo operacional dos algoritmos de cache LRU e FIFO, utilizando a biblioteca LSH para gerenciar o armazenamento em cache.

<div align="center">
  <img src="imgs/fluxo3.png" width="70%" height="50%">
  <p align="center"><em> Fluxo memória cache </em></p>
</div>

## 🎯 Resultados

Com a utilização da cache é possível ver que o sistema mostrou uma boa capacidade de funcionamento, observa-se que quando uma operação realizada por um dos processos já existe na cache, há uma redução de tempo facilitando e agilizando os processos.



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
 <a href="https://t.me/annalaurams">
 <img align="center" src="https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white"/> 
 </div>
<a style="color:black" href="mailto:nalauramoura@gmail.com?subject=[GitHub]%20Source%20Dynamic%20Lists">
✉️ <i>nalauramoura@gmail.com</i>
</a>

<div>
 <br><p align="justify"> Jullia Fernandes</p>
 <a href="https://t.me/JulliaFernandes">
 <img align="center" src="https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white"/> 
 </div>
<a style="color:black" href="mailto:julliacefet@gmail.com?subject=[GitHub]%20Source%20Dynamic%20Lists">
✉️ <i>julliacefet@gmail.com</i>
</a>