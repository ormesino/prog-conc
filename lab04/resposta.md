### Atividade 3

#### Enunciado

Praticar o uso da concorrência. Dada uma sequência consecutiva de números naturais (inteiros positivos) de 1 a N (N muito grande), identificar todos os números primos dessa sequência e retornar a quantidade total de números primos encontrados.

#### Roteiro

1. Implemente uma versão concorrente para esse problema, dividindo a tarefa entre as _threads_. O número de elementos (N) e o número de _threads_ devem ser informados
   na linha de comando.
   ATENCÃO: os valores da série NÃO devem ser colocados em um vetor, devem ser obtidos incrementalmente em tempo de execução.
2. OBS.: defina a variável N do tipo `long long int` e use a função `atoll()` para converter o valor recebido do usuário (string) para `long long int`.
3. Certifique-se que a sua solução está correta.
4. Inclua tomadas de tempo no código (parte de processamento da sequência).
5. Execute o programa várias vezes, variando o valor de N (experimente $10^3$ e $10^6$) e para cada valor de N, varie a quantidade de _threads_ (1, 2 e 4).
6. Para cada configuração, execute o programa ao menos 5 vezes e registre o tempo médio dessas execuções.
7. Calcule a aceleração e a eficiência alcançada em cada configuração, tomado como base (execução sequencial) a execução com 1 _thread_.

#### Comandos

Compile o arquivo `achaPrimo.c` conforme o exemplo abaixo.

```bash
gcc achaPrimo.c -o <executavel> -Wall -lm
```

Para a utilização do programa basta realizar a chamada do executável informando respectivamente o valor de N e a quantidade de _threads_ desejada.

```bash
./<executavel> <valor_n> <n_threads>
```

Após a execução serão imprimidas no console as informações de tempo de execução de diferentes partes do programa, sendo possível assim realizar a comparação entre as execuções concorrente e sequencial (uso de 1 _thread_).

#### Resultados Obtidos

| Versões | 10^3 elementos | 10^6 elementos  |
|--------|-----------------|---------|
| Sequencial (1 thread) | 0,0002274s  |  0,1679996s  | 
| Concorrente (2 threads) | 0,000256s  |  0,0916866s  | 
| Concorrente (4 threads) | 0,0004264s  |  0,0848864s  | 

| Versões | Aceleração (10^3) | Eficiência (10^3) |
|--------|-----------------|---------|
| Concorrente (2 threads) |  0,88828125 |  0,148046875  | 
| Concorrente (4 threads) |  0,53330206 |  0,088883677  | 

| Versões | Aceleração (10^6) | Eficiência (10^6) |
|--------|-----------------|---------|
| Concorrente (2 threads) |  1,83232446 |  0,30538741  | 
| Concorrente (4 threads) |  1,97911090 |  0,32985182  | 

