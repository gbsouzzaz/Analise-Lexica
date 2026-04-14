#include <stdio.h>
#include <string.h>
#include "ts.h"

#define MAX_TS 1000

char tabela[MAX_TS][100];
int topo = 0;

int buscarTS(char *lexema) {
    for (int i = 0; i < topo; i++) {
        if (strcmp(tabela[i], lexema) == 0)
            return i;
    }
    return -1;
}

void inserirTS(char *lexema) {
    if (buscarTS(lexema) == -1) {
        strcpy(tabela[topo++], lexema);
    }
}

void imprimirTS() {
    printf("\n===== TABELA DE SIMBOLOS =====\n");
    for (int i = 0; i < topo; i++) {
        printf("%d -> %s\n", i, tabela[i]);
    }
}