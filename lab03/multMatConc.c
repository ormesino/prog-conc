#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

float *mat1, *mat2, *result; // Ponteiros para as matrizes
int nThreads;                // Número de threads

// Estrutura de dados para armazenar os argumentos da thread
typedef struct
{
  int id, R1, R2, C2;
} tArgs;

// Função que as threads executarão
void *multMat(void *arg)
{
  tArgs *args = (tArgs *)arg;

  // Multiplicação das matrizes
  for (int i = args->id; i < args->R1; i += nThreads)
    for (int j = 0; j < args->C2; j++)
    {
      result[i * (args->C2) + j] = 0;
      for (int k = 0; k < args->R2; k++)
        result[i * (args->C2) + j] += mat1[i * (args->R2) + k] * mat2[k * (args->C2) + j];
    }

  pthread_exit(NULL);
}

// Inicializa a matriz a partir de um arquivo
void initMat(float **mat, int *rows, int *columns, char *str)
{
  long long int size;
  size_t ret;
  FILE *fptr;

  // Abertura do arquivo contendo a primeira matriz
  fptr = fopen(str, "rb");
  if (!fptr)
  {
    fprintf(stderr, "ERRO: Abertura do arquivo %s\n", str);
    exit(-2);
  }

  // Leitura das dimensões da primeira matriz
  ret = fread(rows, sizeof(int), 1, fptr);
  if (!ret)
  {
    fprintf(stderr, "ERRO: Leitura das dimensoes da matriz \n");
    exit(-3);
  }
  ret = fread(columns, sizeof(int), 1, fptr);
  if (!ret)
  {
    fprintf(stderr, "ERRO: Leitura das dimensoes da matriz \n");
    exit(-3);
  }
  size = *(rows) * *(columns);

  // Alocação de memória para a primeira matriz
  *mat = (float *)malloc(sizeof(float) * size);
  if (!*mat)
  {
    fprintf(stderr, "ERRO: malloc() da matriz\n");
    exit(-4);
  }

  // Carrega os elementos do arquivo na primeira matriz
  ret = fread(*mat, sizeof(float), size, fptr);
  if (ret < size)
  {
    fprintf(stderr, "ERRO: Leitura dos elementos da matriz\n");
    exit(-5);
  }

  fclose(fptr);
}

int main(int argc, char *argv[])
{
  int R1, C1, R2, C2;       // Dimensões das matrizes
  long long int resultSize; // Tamanho das matrizes
  FILE *fptr;               // Ponteiro para o arquivo
  tArgs *args;              // IDs locais das threads e dimensao
  pthread_t *tid;           // IDs das threads
  size_t ret;               // Retorno da leitura
  clock_t inicio, fim;      // Variáveis para medir o tempo
  double delta;

  // Leitura dos comandos do terminal
  inicio = clock();
  if (argc < 5)
  {
    fprintf(stderr, "Insira o comando no formato: \
    %s <arquivo_entrada_1> <arquivo_entrada_2> <arquivo_saida> <qntd_threads>",
            argv[0]);
    return -1;
  }

  // Inicialização das matrizes e da qntd de threads
  initMat(&mat1, &R1, &C1, argv[1]);
  initMat(&mat2, &R2, &C2, argv[2]);
  nThreads = atoi(argv[4]);

  // Verifica se as matrizes são compatíveis
  if (C1 != R2)
  {
    fprintf(stderr, "ERRO: Numero de colunas da primeira matriz diferente do \
                      numero de linhas da segunda matriz.");
    return -6;
  }

  // Alocação de memória para a matriz final
  resultSize = R1 * C2;
  result = (float *)malloc(sizeof(float) * resultSize);
  if (!result)
  {
    fprintf(stderr, "ERRO: malloc() da matriz final\n");
    return -4;
  }
  fim = clock();
  delta = (double)(fim - inicio) / CLOCKS_PER_SEC;
  printf("Tempo inicializacao: %lf\n", delta);

  // Alocação de memória para as estruturas auxiliares das threads
  inicio = clock();
  tid = (pthread_t *)malloc(sizeof(pthread_t) * nThreads);
  if (!tid)
  {
    fprintf(stderr, "ERRO: malloc() do tid\n");
    return -4;
  }
  args = (tArgs *)malloc(sizeof(tArgs) * nThreads);
  if (!args)
  {
    fprintf(stderr, "ERRO: malloc() do args\n");
    return -4;
  }

  // Processamento Concorrente
  for (int i = 0; i < nThreads; i++)
  {
    (args + i)->id = i;
    (args + i)->R1 = R1;
    (args + i)->R2 = R2;
    (args + i)->C2 = C2;
    if (pthread_create(tid + i, NULL, multMat, (void *)(args + i)))
    {
      fprintf(stderr, "ERRO: pthread_create()\n");
      return -4;
    }
  }

  // Aguardando a finalização das threads
  for (int i = 0; i < nThreads; i++)
  {
    pthread_join(*(tid + i), NULL);
  }
  fim = clock();
  delta = (double)(fim - inicio) / CLOCKS_PER_SEC;
  printf("Tempo multiplicacao (%dx%d com %dx%d) (%d threads): %lf\n", R1, C1, R2, C2, nThreads, delta);

  // Escrita da matriz resultante no arquivo de saída
  inicio = clock();
  fptr = fopen(argv[3], "wb");
  if (!fptr)
  {
    fprintf(stderr, "ERRO: Abertura do arquivo\n");
    return -3;
  }
  ret = fwrite(&R1, sizeof(int), 1, fptr);
  ret = fwrite(&C2, sizeof(int), 1, fptr);
  ret = fwrite(result, sizeof(float), resultSize, fptr);
  if (ret < resultSize)
  {
    fprintf(stderr, "ERRO: Escrita no arquivo\n");
    return -7;
  }

  // Liberação de memória
  fclose(fptr);
  free(mat1);
  free(mat2);
  free(result);
  free(args);
  free(tid);
  fim = clock();
  delta = (double)(fim - inicio) / CLOCKS_PER_SEC;
  printf("Tempo finalizacao: %lf\n", delta);

  return 0;
}