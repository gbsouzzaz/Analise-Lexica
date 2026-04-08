#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"

int linha = 1;
int coluna = 0;

int ehPalavraReservada(char *lexema) {
    char *reservadas[] = {
        "program", "var", "integer", "real",
        "begin", "end", "if", "then", "else",
        "while", "do"
    };

    int total = 11;

    for (int i = 0; i < total; i++) {
        if (strcmp(lexema, reservadas[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

Token proximoToken() {
    Token token;
    char c;

    c = getchar();
    coluna++;

    while (c == ' ' || c == '\t' || c == '\n') {
        if (c == '\n') {
            linha++;
            coluna = 0;
        }
        c = getchar();
        coluna++;
    }

    if (c == EOF) {
        token.tipo[0] = '\0';
        return token;
    }

    if (isalpha(c)) {
        char buffer[100];
        int i = 0;

        while (isalnum(c)) {
            buffer[i++] = tolower(c);
            c = getchar();
            coluna++;
        }

        buffer[i] = '\0';
        ungetc(c, stdin);

        if (ehPalavraReservada(buffer))
            strcpy(token.tipo, "KW");
        else
            strcpy(token.tipo, "ID");

        strcpy(token.lexema, buffer);
        token.linha = linha;
        token.coluna = coluna - i;

        return token;
    }

    if (isdigit(c)) {
        char buffer[100];
        int i = 0;

        while (isdigit(c)) {
            buffer[i++] = c;
            c = getchar();
            coluna++;
        }

        buffer[i] = '\0';
        ungetc(c, stdin);

        strcpy(token.tipo, "NUM_INT");
        strcpy(token.lexema, buffer);
        token.linha = linha;
        token.coluna = coluna - i;

        return token;
    }

    if (c == ':') {
        char next = getchar();
        coluna++;

        if (next == '=') {
            strcpy(token.tipo, "OP_ASS");
            strcpy(token.lexema, ":=");
        } else {
            strcpy(token.tipo, "SMB_COL");
            strcpy(token.lexema, ":");
            ungetc(next, stdin);
        }

        token.linha = linha;
        token.coluna = coluna - 1;
        return token;
    }

    if (c == ';') {
        strcpy(token.tipo, "SMB_SEM");
        strcpy(token.lexema, ";");
        token.linha = linha;
        token.coluna = coluna;

        return token;
    }

    // ERRO
    strcpy(token.tipo, "ERRO");
    token.lexema[0] = c;
    token.lexema[1] = '\0';
    token.linha = linha;
    token.coluna = coluna;

    return token;
}