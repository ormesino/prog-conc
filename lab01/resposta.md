### Atividade 5

#### Enunciado

Escreva um programa com M _threads_, para elevar ao quadrado cada elemento de um vetor de N elementos do tipo inteiro (para cada elemento do vetor, calcular o novo valor e escrever o resultado na mesma posição do elemento).

#### Roteiro

1. Comece pensando em como dividir a tarefa de elevar ao quadrado todos os elementos de um vetor entre M _threads_. Importante: todas as _threads_ deverão executar a mesma função. Qual(is) argumento(s) deverá(ão) ser passado(s) para a função executada pelas _threads_?
2. Implemente funções separadas para inicializar e carregar o vetor e verificar se o resultado está correto no final.
3. Na função `main`, chame a função de inicialização do vetor; crie as _threads_; aguarde o término da execução das _threads_ criadas e chame a função para verificar se os
   valores finais do vetor estão corretos.
4. Execute o programa várias vezes e verifique se ele está funcionando corretamente.

#### Resposta

- Implementação do programa está no arquivo `ativ5.c`.