#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Constantes
#define RANGE 20
#define NELEMENTS 500
#define MTHREADS 19

// Vetor que será utilizado pelas threads e vetor de checagem final
int testArr[NELEMENTS];

// Estrutura para auxiliar na multiplicação
typedef struct
{
  int indexArr, qntyElements;
  int *arr;
} tArgs;

// Função de inicialização do vetor
int *initArr(void)
{
  int temp;

  // Criação do vetor principal
  int *arr = (int *)malloc(NELEMENTS * sizeof(int));
  if (arr == NULL)
  {
    printf("ERRO: malloc()\n");
    exit(-4);
  }

  // Utilização de inteiros randômicos na inicialização
  srand(time(NULL));
  printf("Vetor inicial: \n");
  for (int i = 0; i < NELEMENTS; i++)
  {
    temp = rand() % RANGE;
    arr[i] = temp;
    testArr[i] = temp;
    printf("%d ", temp);
  }
  printf("\n");

  return arr;
}

// Função de multiplicação informada para cada thread
void *multElement(void *arg)
{
  tArgs args = *(tArgs *)arg;

  // Multiplica desde o índice dado até a qntd de elementos da thread
  for (int i = args.indexArr; i < args.indexArr + args.qntyElements; i++)
    args.arr[i] *= args.arr[i];

  free(arg);
  pthread_exit(NULL);
}

// Função para checar o vetor resultado após as multiplicações
int checkArr(int *arr)
{
  for (int i = 0; i < NELEMENTS; i++)
  {
    if (testArr[i] != 0 && testArr[i] != arr[i] / testArr[i])
    {
      printf("ERRO: Checagem identificou uma multiplicação incorreta.\n");
      exit(-3);
    }
  }
  return 0;
}

int main(void)
{
  pthread_t tid[MTHREADS];
  tArgs *args;

  // Inicializa os vetores
  int *arr = initArr();

  // Checagem entre número de elementos e número de threads
  if (MTHREADS > NELEMENTS)
  {
    printf("ERRO: Informe uma quantidade de elementos maior do que a de threads.");
    exit(1);
  }

  // Definição da quantidade de elementos por thread e o limite
  int elemByThread = fabs(NELEMENTS / MTHREADS);
  int limit = NELEMENTS % MTHREADS;

  // Loop de criação das threads
  for (int i = 0, index = 0; i < MTHREADS; i++)
  {
    args = malloc(sizeof(tArgs));

    args->indexArr = index;
    args->arr = arr;
    // Se estiver dentro do limite, adiciona mais um elemento
    if (i < limit)
      args->qntyElements = elemByThread + 1;
    else
      args->qntyElements = elemByThread;

    // Criação da thread com o objeto como argumento
    if (pthread_create(&tid[i], NULL, multElement, (void *)args))
    {
      printf("ERRO: pthread_create()\n");
      exit(-1);
    }

    // Incremento do índice com base na quantidade de elementos dada para a thread
    index += args->qntyElements;
  }

  // Loop de espera da execução de todas as threads
  for (int i = 0; i < MTHREADS; i++)
  {
    if (pthread_join(tid[i], NULL))
    {
      printf("ERRO: pthread_join()\n");
      exit(-2);
    }
  }

  // Teste se os resultados são condizentes
  checkArr(arr);

  // Impressão do vetor resultado
  printf("\nVetor resultado:\n");
  for (int i = 0; i < NELEMENTS; i++)
    printf("%d ", arr[i]);

  return 0;
}