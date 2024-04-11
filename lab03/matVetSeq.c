#include <stdio.h>
#include <stdlib.h>

// #define TEST

float *firstMatrix, *secondMatrix, *finalMatrix;

int main(int argc, char *argv[])
{
  float *firstMatrix, *secondMatrix, *finalMatrix;
  int firstMatrixRows, firstMatrixCol, secondMatrixRows, secondMatrixCol;
  long long int firstMatrixSize, secondMatrixSize;
  FILE *fptr;
  size_t ret;

  // Leitura dos comandos do terminal
  if (argc < 5)
  {
    fprintf(stderr, "Insira o comando no formato: %s <arquivo_entrada_1> <arquivo_entrada_2> <arquivo_saida> <qntd_threads>", argv[0]);
    return -1;
  }

  // abertura do arquivo da primeira matriz
  fptr = fopen(argv[1], "rb");
  if (!fptr)
  {
    fprintf(stderr, "ERRO: Abertura do arquivo %s\n", argv[1]);
    return -2;
  }

  // le as dimensoes da primeira matriz
  ret = fread(&firstMatrixRows, sizeof(int), 1, fptr);
  if (!ret)
  {
    fprintf(stderr, "ERRO: Leitura das dimensoes da primeira matriz \n");
    return -3;
  }
  ret = fread(&firstMatrixCol, sizeof(int), 1, fptr);
  if (!ret)
  {
    fprintf(stderr, "ERRO: Leitura das dimensoes da primeira matriz \n");
    return -3;
  }
  firstMatrixSize = firstMatrixRows * firstMatrixCol;

  // aloca memoria para a primeira matriz
  firstMatrix = (float *)malloc(sizeof(float) * matrixSize);
  if (!firstMatrix)
  {
    fprintf(stderr, "ERRO: malloc() da primeira matriz\n");
    return -4;
  }

  // carrega a matriz de elementos do tipo float do arquivo
  ret = fread(firstMatrix, sizeof(float), matrixSize, fptr);
  if (ret < matrixSize)
  {
    fprintf(stderr, "ERRO: Leitura dos elementos da matriz\n");
    return -5;
  }

  // abertura do arquivo da segunda matriz
  fptr = fopen(argv[2], "rb");
  if (!fptr)
  {
    fprintf(stderr, "ERRO: Abertura do arquivo %s\n", argv[2]);
    return -2;
  }

  // le as dimensoes da segunda matriz
  ret = fread(&secondMatrixRows, sizeof(int), 1, fptr);
  if (!ret)
  {
    fprintf(stderr, "ERRO: Leitura das dimensoes da segunda matriz \n");
    return -3;
  }
  ret = fread(&secondMatrixCol, sizeof(int), 1, fptr);
  if (!ret)
  {
    fprintf(stderr, "ERRO: Leitura das dimensoes da segunda matriz \n");
    return -3;
  }
  secondMatrixSize = secondMatrixRows * secondMatrixCol;

  secondMatrix = (float *)malloc(sizeof(float) * matrixSize);
  if (!secondMatrix)
  {
    fprintf(stderr, "ERRO: malloc() da segunda matriz\n");
    return -4;
  }

  ret = fread(secondMatrix, sizeof(float), matrixSize, fptr);
  if (ret < matrixSize)
  {
    fprintf(stderr, "ERRO: Leitura dos elementos da matriz\n");
    return -5;
  }

  if (firstMatrixCol != secondMatrixRows) {
    fprintf(stderr, "ERRO: Numero de colunas da primeira matriz diferente do numero de linhas da segunda matriz.\nInforme matrizes validas");
    return -5;
  }

  finalMatrix = (float *)malloc(sizeof(float) * firstMatrixCol * secondMatrixRows);
  if (!finalMatrix)
  {
    fprintf(stderr, "ERRO: malloc() da matriz final\n");
    return -4;
  }

  // Processamento Sequencial
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
  for (int i = 0; i < firstMatrixRows; i++)
  {
    for (int j = 0; j < firstMatrixCol; j++)
      fprintf(stdout, "%.6f ", firstMatrix[i * firstMatrixCol + j]);
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\n");
  fprintf(stdout, "Segunda matriz:\n");
  for (int i = 0; i < secondMatrixRows; i++)
  {
    for (int j = 0; j < secondMatrixCol; j++)
      fprintf(stdout, "%.6f ", secondMatrix[i * secondMatrixCol + j]);
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