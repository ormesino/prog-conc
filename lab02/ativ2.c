#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

// Constante para chamada do teste
#define TEST

// Ponteiro global para o vetor
float *arr;

// Estrutura para os dados de entrada das threads
typedef struct
{
  long int start, qntyElem;
  float chunkSum;
} tArgs;

// Função de soma dos blocos por cada thread
void *sumElem(void *arg)
{
  tArgs *args = (tArgs *)arg;

  // Soma os valores do bloco que a thread monitora
  for (long int i = args->start; i < args->start + args->qntyElem; i++)
    args->chunkSum += arr[i];

  // Retorno da soma do bloco
  pthread_exit((void *)args);
}

int main(int argc, char *argv[])
{
  int nThreads;              // Número de threads informada pelo usuário
  long int qtyElem;          // Quantidade de elementos do vetor de origem
  long int chunkSize, limit; // Auxiliares na divisão dos blocos para as threads
  float sum = 0;             // Soma dos elementos do vetor

  pthread_t *tid;         // Vetor de identificadores das threads
  tArgs *args, *response; // Entrada da thread e sua resposta

// Definição das variáveis de teste
#ifdef TEST
  float correctSum, sumError;
#endif

  // Valida e recebe os valores de entrada
  if (argc < 2)
  {
    fprintf(stderr, "Use o comando: '%s <numero de threads (maior que 0)>' \n", argv[0]);
    return 1;
  }
  nThreads = atol(argv[1]);
  if (nThreads < 1)
  {
    fprintf(stderr, "ERRO: o numero de threads deve ser maior que 0 \n");
    return 2;
  }

  // Cria o vetor para armazenar os ids das threads
  tid = (pthread_t *)malloc(sizeof(pthread_t) * nThreads);
  if (tid == NULL)
  {
    fprintf(stderr, "ERRO: malloc() do vetor de threads\n");
    return 3;
  }

  // Armazena a quantidade de elementos do vetor
  scanf("%ld", &qtyElem);
  if (qtyElem < nThreads)
  {
    printf("A quantidade de threads eh maior que a quantidade de elementos, a execucao sera sequencial!\n");
    nThreads = qtyElem;
  }

  // Alocação do espaço para o vetor e sua inicialização
  arr = (float *)malloc(sizeof(float) * qtyElem);
  if (arr == NULL)
  {
    fprintf(stderr, "ERRO: malloc() do vetor de elementos\n");
    return 3;
  }
  for (long int i = 0; i < qtyElem; i++)
    scanf("%f", &arr[i]);

// Armazena o valor de teste da soma
#ifdef TEST
  scanf("%f", &correctSum);
#endif

  // Definição da quantidade de elementos por thread e o limite
  chunkSize = qtyElem / nThreads;
  limit = qtyElem % nThreads;

  // Loop de criação das threads
  for (int i = 0, index = 0; i < nThreads; i++)
  {
    args = malloc(sizeof(tArgs));
    if (args == NULL)
    {
      fprintf(stderr, "ERRO: malloc() da entrada das threads\n");
      return 4;
    }

    args->start = index;
    // Se estiver dentro do limite, adiciona mais um elemento
    if (i < limit)
      args->qntyElem = chunkSize + 1;
    else
      args->qntyElem = chunkSize;

    // Criação da thread com o objeto como argumento
    if (pthread_create(&tid[i], NULL, sumElem, (void *)args))
    {
      printf("ERRO: pthread_create()\n");
      return 5;
    }

    // Incremento do índice com base na quantidade de elementos dada para a thread
    index += args->qntyElem;
  }

  // Loop de espera da execução das threads
  for (int i = 0; i < nThreads; i++)
  {
    if (pthread_join(*(tid + i), (void **)&response))
    {
      fprintf(stderr, "ERRO: pthread_join()\n");
      return 6;
    }

    // Soma dos retornos das threads
    sum += response->chunkSum;
  }

  // Exibir o resultado da soma final
  printf("\nSoma final: %.7f\n", sum);

// Comparação com o valor verdadeiro
#ifdef TEST
  sumError = fabs(correctSum - sum);
  printf("Soma correta: %.7f\n", correctSum);
  printf("Erro de soma encontrado: %.7f\n", sumError);
#endif

  return 0;
}