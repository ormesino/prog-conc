### Atividade 2

#### Enunciado

Implemente um programa concorrente onde uma _thread_ produtora carrega de um arquivo binário uma sequência de N (N bastante grande) de números inteiros e os deposita em um buffer de tamanho M (M pequeno) — um de cada vez — que será compartilhado com _threads_ consumidoras. As _threads_ consumidoras retiram os números — um de cada vez — e avaliam sua primalidade, usando a seguinte função:
```c
int findPrime(int num)
{
  if (num <= 1)
    return 0;
  if (num == 2)
    return 1;
  if (num % 2 == 0)
    return 0;
  for (int i = 3; i <= sqrt(num); i += 2)
    if (num % i == 0)
      return 0;
  return 1;
}
```
Ao final do programa (depois que os N números foram processados), deverá ser retornado: 
- (i) a quantidade de números primos encontrados (para avaliar a corretude do programa).
- (ii) a _thread_ consumidora vencedora (aquela que encontrou a maior quantidade de primos).

#### Comandos

Compile o arquivo `geraInt.c` e rode o executável para criar o arquivo com a sequência de valores inteiros conforme o exemplo abaixo.

```bash
gcc geraInt.c -o <executavel> -Wall
./<executavel> <qntd_inteiros> <arquivo_temp>
```

Agora é necessário compilar o arquivo `primoSem.c` que será responsável por executar de forma concorrente o algoritmo responsável por detectar números primos.

```bash
gcc primoSem.c -o <executavel> -Wall -lm
./<executavel> <arquivo_temp> <qntd_buffer> <n_threads>
```

Após a execução serão imprimidas no terminal as informações de quantos números primos foram detectados, assim como a thread vencedora e quantos números primos ela detectou.
