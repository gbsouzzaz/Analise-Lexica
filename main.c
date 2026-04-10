#include "scanner.c"
#include <stdio.h>
#include <stdlib.h>

int main() {
  Token t;

  // Redireciona a entrada padrão (stdin) para ler diretamente do arquivo
  // teste.pas
  if (freopen("teste.pas", "r", stdin) == NULL) {
    printf("Erro: Não foi possível abrir o arquivo teste.pas\n");
    return 1;
  }

  FILE *saida = fopen("saida.lex", "w");

  if (saida == NULL) {
    printf("Erro ao criar arquivo de saída\n");
    return 1;
  }

  while (1) {
    t = proximoToken();

    if (t.tipo[0] == '\0')
      break;

    printf("<%s, %s> (%d,%d)\n", t.tipo, t.lexema, t.linha, t.coluna);

    fprintf(saida, "<%s, %s> %d %d\n", t.tipo, t.lexema, t.linha, t.coluna);
  }

  fclose(saida);

  printf("\nArquivo saida.lex gerado com sucesso!\n");

  return 0;
}