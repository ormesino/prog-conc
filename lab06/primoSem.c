#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

// Variáveis globais
sem_t fullSlot, emptySlot;
sem_t innerMutex, outerMutex;
int arrSize, globalN = 0, bufferSize;
int *arr, *buffer;

typedef struct
{
  int id;
  int score;
} worker;

// Inicializa o vetor de inteiros com os valores do arquivo
void initArr(char *str)
{
  size_t ret;
  FILE *fptr;

  // Abertura do arquivo
  fptr = fopen(str, "rb");
  if (!fptr)
  {
    fprintf(stderr, "ERRO: Abertura do arquivo %s\n", str);
    exit(-2);
  }

  // Leitura da dimensão do vetor
  ret = fread(&arrSize, sizeof(int), 1, fptr);
  if (!ret)
  {
    fprintf(stderr, "ERRO: Leitura da dimensao do vetor\n");
    exit(-3);
  }

  // Alocação de memória para o vetor de inteiros
  arr = (int *)malloc(sizeof(int) * arrSize);
  if (!arr)
  {
    fprintf(stderr, "ERRO: malloc() do vetor\n");
    exit(-4);
  }

  // Carrega os elementos do arquivo no vetor
  ret = fread(arr, sizeof(int), arrSize, fptr);
  if (ret < arrSize)
  {
    fprintf(stderr, "ERRO: Leitura dos elementos do vetor\n");
    exit(-5);
  }

  fclose(fptr);
}

// Função auxiliar para verificar se um número é primo
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
  buffer[in] = elem;
  in = (in + 1) % bufferSize;
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
  item = buffer[out];
  buffer[out] = 0;
  out = (out + 1) % bufferSize;
  sem_post(&innerMutex);
  sem_post(&emptySlot); // Sinaliza um slot vazio para ser consumido
  return item;
}

// Função produtora
void *producer(void *arg)
{
  free(arg);
  for (int i = 0; i < arrSize; i++)
    Put(*(arr + i));

  pthread_exit(NULL);
}

// Função consumidora
void *consumer(void *arg)
{
  int num;
  int *partialResult = (int *)malloc(sizeof(int));
  if (!partialResult)
  {
    fprintf(stderr, "ERRO: malloc() do resultado parcial\n");
    pthread_exit(NULL);
  }
  *partialResult = 0;
  free(arg);

  while (1)
  {
    sem_wait(&outerMutex);
    if (globalN == arrSize)
    {
      sem_post(&outerMutex);
      break;
    }
    num = Remove();
    globalN++;
    sem_post(&outerMutex);

    *partialResult += findPrime(num);
  }

  pthread_exit((void *)partialResult);
}

int main(int argc, char *argv[])
{
  int nThreads, *id, result = 0;
  pthread_t *tidCon, tidProd;
  worker *winner;

  // Validação dos parâmetros de entrada
  if (argc != 4)
  {
    fprintf(stderr, "Use: %s <arquivo_inteiros> <tam_buffer> <qntd_threads>\n", argv[0]);
    exit(-1);
  }

  // Inicialização do buffer
  bufferSize = atoi(argv[2]);
  buffer = (int *)malloc(sizeof(int) * bufferSize);
  if (!buffer)
  {
    fprintf(stderr, "ERRO: malloc() do buffer\n");
    return -3;
  }

  // Inicialização dos semáforos
  sem_init(&innerMutex, 0, 1);
  sem_init(&outerMutex, 0, 1);
  sem_init(&fullSlot, 0, 0);
  sem_init(&emptySlot, 0, bufferSize);

  // Inicialização do vetor de inteiros
  initArr(argv[1]);
  nThreads = atoi(argv[3]);

  // Alocação do vetor de threads consumidoras
  tidCon = (pthread_t *)malloc(sizeof(pthread_t) * nThreads);
  if (!tidCon)
  {
    fprintf(stderr, "ERRO: malloc() do tidCon\n");
    return -4;
  }

  // Criação da thread produtora
  if (pthread_create(&tidProd, NULL, producer, NULL))
  {
    fprintf(stderr, "ERRO: Criacao da thread produtora\n");
    exit(1);
  }

  // Criação das threads consumidoras
  for (int i = 0; i < nThreads; i++)
  {
    id = (int *)malloc(sizeof(int));
    *id = i + 1;
    if (pthread_create(&tidCon[i], NULL, consumer, NULL))
    {
      fprintf(stderr, "ERRO: Criacao das threads consumidoras\n");
      exit(1);
    }
  }

  // Alocação do vencedor
  winner = (worker *)malloc(sizeof(worker));
  if (!winner)
  {
    fprintf(stderr, "ERRO: malloc() do vencedor\n");
    return -5;
  }
  winner->id = 0;
  winner->score = 0;

  // Aguardando a execução da thread produtora
  if (pthread_join(tidProd, NULL))
  {
    fprintf(stderr, "ERRO: join() da thread produtora\n");
    exit(-1);
  }

  // Aguardando a execução das threads consumidores
  for (int i = 0; i < nThreads; i++)
  {
    int *partialResult;
    if (pthread_join(tidCon[i], (void **)&partialResult))
    {
      fprintf(stderr, "ERRO: join() das threads consumidoras\n");
      exit(-1);
    }
    result += *partialResult;
    if (winner->score < *partialResult)
    {
      winner->id = i + 1;
      winner->score = *partialResult;
    }
    free(partialResult);
  }

  // Exibição dos resultados
  printf("Soma dos numeros primos encontrados: %d\n", result);
  printf("Thread %d eh a vencedora tendo achado %d primos\n", winner->id, winner->score);

  // Liberação de memória
  free(tidCon);
  free(id);
  free(arr);
  free(winner);

  return 0;
}