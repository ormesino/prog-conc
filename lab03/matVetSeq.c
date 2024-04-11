#include <stdio.h>
#include <stdlib.h>

// #define TEST

int main(int argc, char *argv[])
{
  float *firstMatrix, *secondMatrix, *finalMatrix;
  int rows, columns;
  long long int matrixSize;
  FILE *fptr;
  size_t ret;

  // Leitura dos comandos do terminal
  if (argc < 4)
  {
    fprintf(stderr, "Insira o comando no formato: %s <arquivo_entrada> <arquivo_saida> <qntd_threads>", argv[0]);
    return -1;
  }

  //
  fptr = fopen(argv[1], "rb");
  if (!fptr)
  {
    fprintf(stderr, "ERRO: Abertura do arquivo\n");
    return -2;
  }

  // le as dimensoes da matriz
  ret = fread(&rows, sizeof(int), 1, fptr);
  if (!ret)
  {
    fprintf(stderr, "ERRO: Leitura das dimensoes da matriz arquivo \n");
    return -3;
  }
  ret = fread(&columns, sizeof(int), 1, fptr);
  if (!ret)
  {
    fprintf(stderr, "ERRO: Leitura das dimensoes da matriz arquivo \n");
    return -3;
  }
  matrixSize = rows * columns;

  // aloca memoria para a matriz
  firstMatrix = (float *)malloc(sizeof(float) * matrixSize);
  if (!firstMatrix)
  {
    fprintf(stderr, "ERRO: malloc()\n");
    return -4;
  }
  secondMatrix = (float *)malloc(sizeof(float) * matrixSize);
  if (!secondMatrix)
  {
    fprintf(stderr, "ERRO: malloc()\n");
    return -4;
  }
  finalMatrix = (float *)malloc(sizeof(float) * rows * rows);
  if (!finalMatrix)
  {
    fprintf(stderr, "ERRO: malloc()\n");
    return -4;
  }

  // carrega a matriz de elementos do tipo float do arquivo
  ret = fread(firstMatrix, sizeof(float), matrixSize, fptr);
  if (ret < matrixSize)
  {
    fprintf(stderr, "ERRO: Leitura dos elementos da matriz\n");
    return -5;
  }
  ret = fread(secondMatrix, sizeof(float), matrixSize, fptr);
  if (ret < matrixSize)
  {
    fprintf(stderr, "ERRO: Leitura dos elementos da matriz\n");
    return -5;
  }

  // Processamento Sequencial
  if (rows != columns)
  {

    /* fprintf(stderr, "ERRO: Informe matrizes quadradas\n");
    return -6; */
  }

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < columns; j++)
    {
      finalMatrix[i] = 0;
      for (int k = 0; k < columns; k++)
      {
        finalMatrix[i] += firstMatrix[i * rows + k] * secondMatrix[k * rows + j];
      }

      printf("%.6f\t", finalMatrix[i]);
    }

    printf("\n");
  }

#ifdef TEST
  fprintf(stdout, "Primeira matriz:\n");
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < columns; j++)
      fprintf(stdout, "%.6f ", firstMatrix[i * columns + j]);
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\n");
  fprintf(stdout, "Segunda matriz:\n");
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < columns; j++)
      fprintf(stdout, "%.6f ", secondMatrix[i * columns + j]);
    fprintf(stdout, "\n");
  }
#endif

  fptr = fopen(argv[2], "wb");
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

  fclose(fptr);
  free(firstMatrix);
  free(secondMatrix);
  free(finalMatrix);

  return 0;
}