#include <stdio.h>
#include "scanner.h"
#include "ts.h"

int main() {
    FILE *entrada = fopen("teste.pas", "r");
    FILE *saida = fopen("saida.lex", "w");

    if (!entrada || !saida) {
        printf("Erro ao abrir arquivos\n");
        return 1;
    }

    // Redireciona stdin
    freopen("teste.pas", "r", stdin);

    Token t;

    fprintf(saida, "LEXEMA\t\tTOKEN\t\tLINHA\tCOLUNA\n");
    fprintf(saida, "---------------------------------------------\n");

    while (1) {
        t = proximoToken();

        if (t.tipo[0] == '\0') break;

        fprintf(saida, "%-15s %-15s %d\t%d\n",
                t.lexema, t.tipo, t.linha, t.coluna);
    }

    fclose(saida);
    fclose(entrada);

    imprimirTS();

    return 0;
}