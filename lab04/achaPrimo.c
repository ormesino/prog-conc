#define _POSIX_C_SOURCE 199309L // para o clock_gettime

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

// Variáveis globais
long long int n;
int globalIndex = 0;
pthread_mutex_t mutex;
int *arr;

// Função que verifica se um número é primo
int findPrime(long long int n)
{
  if (n <= 1)
    return 0;
  if (n == 2)
    return 1;
  if (n % 2 == 0)
    return 0;
  for (int i = 3; i <= sqrt(n); i += 2)
    if (n % i == 0)
      return 0;
  return 1;
}

// Função que as threads executarão
void *countPrimes(void *arg)
{
  int *partialResult;
  partialResult = (int *)malloc(sizeof(int));
  if (partialResult == NULL)
  {
    pthread_exit(NULL);
  }

  // Inicializa o resultado parcial
  *partialResult = 0;
  while (1)
  {
    // Seção crítica
    pthread_mutex_lock(&mutex);
    if (globalIndex >= n)
    {
      pthread_mutex_unlock(&mutex);
      break;
    }
    int index = globalIndex;
    globalIndex++;
    pthread_mutex_unlock(&mutex);

    if (findPrime(arr[index]))
      (*partialResult)++;
  }

  pthread_exit((void *)partialResult);
}

int main(int argc, char *argv[])
{
  int nThreads, result;
  pthread_t *tid;
  struct timespec start, end;
  double delta;

  // Validação dos parâmetros de entrada
  if (argc != 3)
  {
    fprintf(stderr, "Use: %s <n> <nThreads>\n", argv[0]);
    exit(-1);
  }

  n = atoll(argv[1]);
  nThreads = atoi(argv[2]);

  // Alocação de memória para o vetor de elementos e sua inicialização
  arr = (int *)malloc(n * sizeof(int));
  if (arr == NULL)
  {
    fprintf(stderr, "Memory allocation error\n");
    exit(-1);
  }
  for (int i = 0; i < n; i++)
  {
    arr[i] = i + 1;
  }

  // Alocação de memória para o vetor de threads
  tid = (pthread_t *)malloc(nThreads * sizeof(pthread_t));
  if (tid == NULL)
  {
    fprintf(stderr, "Memory allocation error\n");
    exit(-1);
  }

  clock_gettime(CLOCK_MONOTONIC, &start);
  pthread_mutex_init(&mutex, NULL);

  // Inicialização das threads
  for (int i = 0; i < nThreads; i++)
  {
    if (pthread_create(&tid[i], NULL, countPrimes, NULL))
    {
      fprintf(stderr, "Error creating thread\n");
      exit(-1);
    }
  }

  // Aguarda o término das threads
  result = 0;
  for (int i = 0; i < nThreads; i++)
  {
    int *partialResult;
    if (pthread_join(tid[i], (void **)&partialResult))
    {
      fprintf(stderr, "Error joining thread\n");
      exit(-1);
    }
    result += *partialResult;
    free(partialResult);
  }
  clock_gettime(CLOCK_MONOTONIC, &end);
  delta = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1.0e9;
  fprintf(stdout, "Tempo de processamento (%lld elementos) (%d threads): %lf\n", n, nThreads, delta);

  printf("Total de primos: %d\n", result);

  // Libera memória e encerra
  pthread_mutex_destroy(&mutex);
  free(tid);
  free(arr);

  return 0;
}