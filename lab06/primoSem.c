#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define M 5

sem_t fullSlot, emptySlot;
sem_t innerMutex, outerMutex;
int n;
int *arr;
int Buffer[M];

typedef struct
{
  int id;
  int score;
} worker;

void initArr(char *str)
{
  size_t ret;
  FILE *fptr;

  fptr = fopen(str, "rb");
  if (!fptr)
  {
    fprintf(stderr, "ERRO: Abertura do arquivo %s\n", str);
    exit(-2);
  }

  ret = fread(&n, sizeof(int), 1, fptr);
  if (!ret)
  {
    fprintf(stderr, "ERRO: Leitura da dimensao do vetor\n");
    exit(-3);
  }

  // Alocação de memória para o vetor de inteiros
  arr = (int *)malloc(sizeof(int) * n);
  if (!arr)
  {
    fprintf(stderr, "ERRO: malloc() do vetor\n");
    exit(-4);
  }

  // Carrega os elementos do arquivo no vetor
  ret = fread(arr, sizeof(int), n, fptr);
  if (ret < n)
  {
    fprintf(stderr, "ERRO: Leitura dos elementos do vetor\n");
    exit(-5);
  }

  fclose(fptr);
}

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

// Função para inserir um elemento no buffer
void Put(int elem)
{
  static int in = 0;
  sem_wait(&emptySlot);  // Aguarda slot vazio para inserir
  sem_wait(&innerMutex); // Exclusão mútua entre produtores
  Buffer[in] = elem;
  in = (in + 1) % M;
  sem_post(&innerMutex);
  sem_post(&fullSlot); // Sinaliza um slot cheio para ser consumido
}

// Função para retirar um elemento no buffer
int Remove()
{
  int item;
  static int out = 0;
  sem_wait(&fullSlot);   // Aguarda slot cheio para retirar
  sem_wait(&innerMutex); // Exclusão mútua entre consumidores
  item = Buffer[out];
  Buffer[out] = 0;
  out = (out + 1) % M;
  sem_post(&innerMutex);
  sem_post(&emptySlot); // Sinaliza um slot vazio para ser consumido
  return item;
}

void *producer(void *arg)
{
  free(arg);
  for (int i = 0; i < n; i++)
    Put(*(arr + i));

  pthread_exit(NULL);
}

void *consumer(void *arg)
{
  static int localN;
  int elem, result = 0;
  free(arg);
  while (localN < n)
  {
    elem = Remove();
    sem_wait(&outerMutex);
    localN += 1;
    sem_post(&outerMutex);
    result += findPrime(elem);
  }
  pthread_exit((void *)result);
}

int main(int argc, char *argv[])
{
  int nThreads, result = 0;
  pthread_t *tid;
  worker *winner;

  // Inicialização dos semáforos
  sem_init(&innerMutex, 0, 1);
  sem_init(&outerMutex, 0, 1);
  sem_init(&fullSlot, 0, 0);
  sem_init(&emptySlot, 0, M);

  // Validação dos parâmetros de entrada
  if (argc != 3)
  {
    fprintf(stderr, "Use: %s <arquivo_inteiros> <qntd_threads>\n", argv[0]);
    exit(-1);
  }

  initArr(argv[1]);
  nThreads = atoi(argv[2]) + 1;

  tid = (pthread_t *)malloc(sizeof(pthread_t) * nThreads);
  if (!tid)
  {
    fprintf(stderr, "ERRO: malloc() do tid\n");
    return -4;
  }

  if (pthread_create(&tid[0], NULL, producer, NULL))
  {
    fprintf(stderr, "ERRO: Criacao da thread produtora\n");
    exit(1);
  }

  for (int i = 1; i < nThreads; i++)
  {
    if (pthread_create(&tid[1 + i], NULL, consumer, NULL))
    {
      fprintf(stderr, "ERRO: Criacao das threads consumidoras\n");
      exit(1);
    }
  }

  winner = (worker *)malloc(sizeof(worker) * nThreads);
  if (!winner)
  {
    fprintf(stderr, "ERRO: malloc() do vencedor\n");
    return -4;
  }

  // Aguardando a execução da thread produtora
  if (pthread_join(tid[0], NULL))
  {
    fprintf(stderr, "ERRO: join() da thread produtora\n");
    exit(-1);
  }

  printf("thread produtora parou");

  // Aguardando a execução das threads consumidores
  for (int i = 1; i < nThreads; i++)
  {
    int *partialResult;
    if (pthread_join(tid[i], (void **)&partialResult))
    {
      fprintf(stderr, "ERRO: join() das threads consumidoras\n");
      exit(-1);
    }
    printf("thread %d parou", i);
    result += *partialResult;
    if (winner->score < *partialResult)
    {
      winner->id = i;
      winner->score = *partialResult;
    }
    free(partialResult);
  }

  free(tid);
  free(arr);
  free(winner);

  return 0;
}