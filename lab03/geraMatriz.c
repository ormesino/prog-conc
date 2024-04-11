#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TEST

int main(int argc, char *argv[])
{
  int rows, columns;
  long long int matrixSize;
  float *firstMatrix, *secondMatrix;
  FILE *fptr;
  size_t ret;

  // Leitura dos comandos do terminal
  if (argc < 4)
  {
    fprintf(stderr, "Insira o comando no formato: %s <n_linhas> <m_colunas> <arquivo_saida>", argv[0]);
    return -1;
  }

  // Definição das dimensões da matrizes
  rows = atoi(argv[1]);
  columns = atoi(argv[2]);
  matrixSize = rows * columns;

  // Alocação de espaço para as matrizes
  firstMatrix = (float *)malloc(sizeof(float) * matrixSize);
  if (!firstMatrix)
  {
    fprintf(stderr, "ERRO: malloc()");
    return -2;
  }
  secondMatrix = (float *)malloc(sizeof(float) * matrixSize);
  if (!secondMatrix)
  {
    fprintf(stderr, "ERRO: malloc()");
    return -2;
  }

  // Geração dos números aleatórios
  srand(time(NULL));
  for (int i = 0; i < matrixSize; i++)
  {
    //*(firstMatrix + i) = (rand() % 1000) * 0.3;
    //*(secondMatrix + i) = (rand() % 1000) * 0.3;
    *(firstMatrix + i) = i + 1;
    *(secondMatrix + i) = i + matrixSize + 1;
  }

#ifdef TEST
  fprintf(stdout, "Primeira matriz:\n");
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < columns; j++)
      fprintf(stdout, "%.6f ", firstMatrix[i * columns + j]);
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\nSegunda matriz:\n");
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < columns; j++)
      fprintf(stdout, "%.6f ", secondMatrix[i * columns + j]);
    fprintf(stdout, "\n");
  }
#endif

  // Escrita das matrizes em um arquivo binário de saída
  fptr = fopen(argv[3], "wb");
  if (!fptr)
  {
    fprintf(stderr, "ERRO: Abertura do arquivo\n");
    return -3;
  }
  ret = fwrite(&rows, sizeof(int), 1, fptr);
  ret = fwrite(&columns, sizeof(int), 1, fptr);
  ret = fwrite(firstMatrix, sizeof(float), matrixSize, fptr);
  ret = fwrite(secondMatrix, sizeof(float), matrixSize, fptr);
  if (ret < matrixSize)
  {
    fprintf(stderr, "ERRO: Escrita no arquivo\n");
    return -4;
  }

  // Liberação de espaço alocado na memória
  fclose(fptr);
  free(firstMatrix);
  free(secondMatrix);

  return 0;
}