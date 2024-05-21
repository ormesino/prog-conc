#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
  int qty, *ptArray;
  FILE *fptr;
  size_t ret;

  // Leitura dos comandos do terminal
  if (argc < 3)
  {
    fprintf(stderr, "Insira o comando no formato: %s <n_inteiros> <arquivo_saida>", argv[0]);
    return -1;
  }

  // Quantidade de número inteiros N
  qty = atoi(argv[1]);

  // Alocação de espaço para a lista de inteiros
  ptArray = (int *)malloc(sizeof(int) * qty);
  if (!ptArray)
  {
    fprintf(stderr, "ERRO: malloc()");
    return -2;
  }

  // Geração da sequência de números inteiros
  for (int i = 0; i < qty; i++)
  {
    *(ptArray + i) = i;
  }

  // Escrita da lista de inteiros em um arquivo binário de saída
  fptr = fopen(argv[2], "wb");
  if (!fptr)
  {
    fprintf(stderr, "ERRO: Abertura do arquivo\n");
    return -3;
  }
  ret = fwrite(ptArray, sizeof(int), qty, fptr);
  if (ret < qty)
  {
    fprintf(stderr, "ERRO: Escrita no arquivo\n");
    return -4;
  }

  // Liberação de espaço alocado na memória
  fclose(fptr);
  free(ptArray);

  return 0;
}