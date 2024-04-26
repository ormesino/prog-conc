#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

long long int n;
int globalIndex = 0;
pthread_mutex_t mutex;
int *arr;

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

void *countPrimes(void *arg)
{
  int *partialResult;
  partialResult = (int *)malloc(sizeof(int));
  if (partialResult == NULL)
  {
    pthread_exit(NULL);
  }

  *partialResult = 0;
  while (1)
  {
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

  if (argc != 3)
  {
    fprintf(stderr, "Use: %s <n> <nThreads>\n", argv[0]);
    exit(-1);
  }

  n = atoll(argv[1]);
  nThreads = atoi(argv[2]);

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

  tid = (pthread_t *)malloc(nThreads * sizeof(pthread_t));
  if (tid == NULL)
  {
    fprintf(stderr, "Memory allocation error\n");
    exit(-1);
  }

  pthread_mutex_init(&mutex, NULL);

  for (int i = 0; i < nThreads; i++)
  {
    if (pthread_create(&tid[i], NULL, countPrimes, NULL))
    {
      fprintf(stderr, "Error creating thread\n");
      exit(-1);
    }
  }

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

  printf("Total de primos: %d\n", result);

  pthread_mutex_destroy(&mutex);
  free(tid);
  free(arr);

  return 0;
}