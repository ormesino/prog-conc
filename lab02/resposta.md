### Atividade 2

#### Enunciado

Projetar, implementar e avaliar uma versão concorrente para o problema de somar todos os elementos de um vetor de números reais.

#### Roteiro

1. Implemente um programa auxiliar para gerar os vetores de entrada (quantidade de elementos e elementos do vetor) para os testes da aplicação e o resultado esperado (soma de todos os elementos do vetor).
2. Implemente o programa concorrente para somar os elementos de um vetor de _floats_, considerando que a entrada será lida dos arquivos gerados.
3. Verifique se o resultado gerado está correto. Lembre-se de repetir a execução do programa várias vezes, para a mesma entrada de dados.

#### Comandos

Compile os arquivos `ativ2.c` e `geraVetor.c` conforme o exemplo abaixo.
```bash
gcc ativ2.c -o ativ2 -Wall -lpthread
gcc geraVetor.c -o geraVetor -Wall -lpthread
```

Os executáveis gerados devem ser executados na seguinte ordem e da seguinte forma:
```bash
./geraVetor <num_elem> > temp
./ativ2 <num_threads> < temp
```