### Atividade 1

#### Enunciado

Projetar e implementar uma solucão concorrente para o problema de multiplicacão de matrizes, coletar informações sobre o seu tempo de execução, e calcular o ganho de desempenho obtido.

Os seguintes requisitos de implementação deverão ser atendidos:
- As matrizes de entrada e saída serão do tipo _float_, com N linhas e M colunas. 
- As matrizes de entrada devem ser carregadas de arquivos binários previamente gerados, onde os dois primeiros valores (do tipo inteiro) indicam as dimensões da
matriz (N e M), e os demais elementos (do tipo _float_) são a sequência de valores da matriz.
- As matrizes deverão ser representadas internamente como vetores de _float_ (variável do tipo ponteiro, alocada dinamicamente).
- A matriz de saída deverá ser escrita em um arquivo binário, no mesmo formato dos arquivos de entrada.
- O programa deverá receber como entrada, na linha de comando, os nomes dos arquivos de entrada e de saída, e a quantidade de threads de processamento.
- O programa deverá incluir chamadas de tomada de tempo de execução interna do programa, separando as partes de inicialização, processamento e finalização do programa.

#### Comandos

Compile os arquivos `geraMatriz.c`, `multMatSeq.c` e `multMatConc` conforme o exemplo abaixo.
```bash
gcc geraMatriz.c -o <executavel_matriz> - Wall
gcc multMatSeq.c -o <executavel_seq> -Wall
gcc multMatConc.c -o <executavel_conc> -Wall -lpthread
```

Para a utilização dos programas concorrente e sequencial, é necessário primeiro gerar matrizes (uma ou duas) que serão informadas como parâmetro na execução dos programas. 
```bash
./<executavel_matriz> <qntd_linhas> <qntd_colunas> <arquivo_matriz>
```

Possuindo a(s) matrize(s) geradas, é possível realizar a execução da multiplicação entre matrizes, tanto com o programa sequencial, quanto com o programa concorrente da seguinte forma:
```bash
./<executavel_seq> <matriz_1> <matriz_2> <arquivo_matriz_resultante>
./<executavel_conc> <matriz_1> <matriz_2> <arquivo_matriz_resultante> <qntd_threads>
```

Após a execução serão imprimidas no console as informações de tempo de execução de diferentes partes do programa, sendo possível assim realizar a comparação entre as execuções concorrente e sequencial.