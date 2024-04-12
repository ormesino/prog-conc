#include <stdio.h>
#include <stdlib.h>

#define TEST

float *firstMatrix, *secondMatrix, *finalMatrix;

int main(int argc, char *argv[])
{
  int firstMatrixRows, firstMatrixCol, secondMatrixRows, secondMatrixCol;
  long long int firstMatrixSize, secondMatrixSize, finalMatrixSize;
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
  firstMatrix = (float *)malloc(sizeof(float) * firstMatrixSize);
  if (!firstMatrix)
  {
    fprintf(stderr, "ERRO: malloc() da primeira matriz\n");
    return -4;
  }

  // carrega a matriz de elementos do tipo float do arquivo
  ret = fread(firstMatrix, sizeof(float), firstMatrixSize, fptr);
  if (ret < firstMatrixSize)
  {
    fprintf(stderr, "ERRO: Leitura dos elementos da matriz\n");
    return -5;
  }

  fclose(fptr);

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
  secondMatrix = (float *)malloc(sizeof(float) * secondMatrixSize);
  if (!secondMatrix)
  {
    fprintf(stderr, "ERRO: malloc() da segunda matriz\n");
    return -4;
  }

  ret = fread(secondMatrix, sizeof(float), secondMatrixSize, fptr);
  if (ret < secondMatrixSize)
  {
    fprintf(stderr, "ERRO: Leitura dos elementos da matriz\n");
    return -5;
  }

  if (firstMatrixCol != secondMatrixRows)
  {
    fprintf(stderr, "ERRO: Numero de colunas da primeira matriz diferente do numero de linhas da segunda matriz.\nInforme matrizes validas");
    return -5;
  }

  finalMatrixSize = firstMatrixRows * secondMatrixCol;
  finalMatrix = (float *)malloc(sizeof(float) * finalMatrixSize);
  if (!finalMatrix)
  {
    fprintf(stderr, "ERRO: malloc() da matriz final\n");
    return -4;
  }

  // Processamento Sequencial
  for (int i = 0; i < firstMatrixRows; i++)
  {
    for (int j = 0; j < secondMatrixCol; j++)
    {
      finalMatrix[i * secondMatrixCol + j] = 0;
      for (int k = 0; k < secondMatrixRows; k++)
      {
        finalMatrix[i * secondMatrixCol + j] += firstMatrix[i * secondMatrixRows + k] * secondMatrix[k * secondMatrixCol + j];
      }
    }
  }

#ifdef TEST
  fprintf(stdout, "Primeira matriz:\n");
  for (int i = 0; i < firstMatrixRows; i++)
  {
    for (int j = 0; j < firstMatrixCol; j++)
      fprintf(stdout, "%.6f ", firstMatrix[i * firstMatrixCol + j]);
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\nSegunda matriz:\n");
  for (int i = 0; i < secondMatrixRows; i++)
  {
    for (int j = 0; j < secondMatrixCol; j++)
      fprintf(stdout, "%.6f ", secondMatrix[i * secondMatrixCol + j]);
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\nMatriz final:\n");
  for (int i = 0; i < firstMatrixRows; i++)
  {
    for (int j = 0; j < secondMatrixCol; j++)
      fprintf(stdout, "%.6f ", finalMatrix[i * secondMatrixCol + j]);
    fprintf(stdout, "\n");
  }
#endif

  fptr = fopen(argv[3], "wb");
  if (!fptr)
  {
    fprintf(stderr, "ERRO: Abertura do arquivo\n");
    return -3;
  }
  ret = fwrite(&firstMatrixRows, sizeof(int), 1, fptr);
  ret = fwrite(&secondMatrixCol, sizeof(int), 1, fptr);
  ret = fwrite(finalMatrix, sizeof(float), finalMatrixSize, fptr);

  if (ret < finalMatrixSize)
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