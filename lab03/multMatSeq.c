#include <stdio.h>
#include <stdlib.h>

#define TEST

float *mat1, *mat2, *result; // Ponteiros para as matrizes

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
  size_t ret;               // Retorno da leitura

  // Leitura dos comandos do terminal
  if (argc < 5)
  {
    fprintf(stderr, "Insira o comando no formato: \
    %s <arquivo_entrada_1> <arquivo_entrada_2> <arquivo_saida> <qntd_threads>",
            argv[0]);
    return -1;
  }

  // Inicialização das matrizes
  initMat(&mat1, &R1, &C1, argv[1]);
  initMat(&mat2, &R2, &C2, argv[2]);

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

  // Processamento Sequencial
  for (int i = 0; i < R1; i++)
  {
    for (int j = 0; j < C2; j++)
    {
      result[i * C2 + j] = 0;
      for (int k = 0; k < R2; k++)
      {
        result[i * C2 + j] += mat1[i * R2 + k] * mat2[k * C2 + j];
      }
    }
  }

#ifdef TEST
  fprintf(stdout, "Primeira matriz:\n");
  for (int i = 0; i < R1; i++)
  {
    for (int j = 0; j < C1; j++)
      fprintf(stdout, "%.6f ", mat1[i * C1 + j]);
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\nSegunda matriz:\n");
  for (int i = 0; i < R2; i++)
  {
    for (int j = 0; j < C2; j++)
      fprintf(stdout, "%.6f ", mat2[i * C2 + j]);
    fprintf(stdout, "\n");
  }
  fprintf(stdout, "\nMatriz final:\n");
  for (int i = 0; i < R1; i++)
  {
    for (int j = 0; j < C2; j++)
      fprintf(stdout, "%.6f ", result[i * C2 + j]);
    fprintf(stdout, "\n");
  }
#endif

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

  fclose(fptr);
  free(mat1);
  free(mat2);
  free(result);

  return 0;
}