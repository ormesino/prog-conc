#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Constante para limitar a seed de números randômicos
#define MAX 1000

int main(int argc, char *argv[])
{
  long int qtyElem;   // Quantidade de elementos do vetor que serão gerados
  int factor = 1;      // Fator multiplicador para gerar números negativos
  float elem, sum;    // Valor gerado para incluir no vetor e soma total dos valores

  // Valida e recebe os valores de entrada
  if (argc < 2)
  {
    printf("Use o comando: '%s <numero de elementos (maior que 0)>' \n", argv[0]);
    return 1;
  }
  qtyElem = atol(argv[1]);
  if (qtyElem < 1)
  {
    printf("ERRO: o numero de elementos deve ser maior que 0 \n");
    return 2;
  }

  // Imprime a quantidade de elementos do vetor
  printf("%ld\n", qtyElem);

  // Seed para gerar números randômicos
  srand(time(NULL));

  // Gera e imprime os elementos separado por espaço em branco
  for (int i = 0; i < qtyElem; i++)
  {
    elem = (rand() % MAX) / 12.0 * factor;
    printf("%f ", elem);
    factor *= -1;
    sum += elem;
  }

  // Imprime a soma no final
  printf("\n%f\n", sum);

  return 0;
}