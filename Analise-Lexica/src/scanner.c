#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
#include "ts.h"

int linha = 1;
int coluna = 0;

void classificaIdentificador(char *lexema, char *tipo) {
    char *reservadas[] = {
        "program", "var", "integer", "real",
        "begin", "end", "if", "then", "else",
        "while", "do"
    };

    

    char *tipos_reservadas[] = {
        "KW_PROGRAM", "KW_VAR", "KW_INTEGER", "KW_REAL",
        "KW_BEGIN", "KW_END", "KW_IF", "KW_THEN", "KW_ELSE",
        "KW_WHILE", "KW_DO"
    };

    int total = 11;

    for (int i = 0; i < total; i++) {
        if (strcmp(lexema, reservadas[i]) == 0) {
            strcpy(tipo, tipos_reservadas[i]);
            return;
        }
    }

    strcpy(tipo, "ID");
}

typedef enum {
    q0, q1, q2, q3, q4, q5, q6, q7, q8, q9, q_err,
    q_maior, q_maior_igual, q_maior_aceita,
    q_menor, q_menor_igual, q_menor_difr, q_menor_aceita,
    q_real_ponto, q_real_aceita
} Estado;

Token proximoToken() {
    Token token;
    Estado estado = q0;
    int c;
    char buffer[100];
    int buf_idx = 0;
    int linhaInicioComment = 0;
    int colunaInicioComment = 0;

    while (1) {
        switch (estado) {
            case q0:
                c = getchar();
                coluna++;
                
                if (c == ' ' || c == '\t' || c == '\r') {
                    estado = q0;
                } else if (c == '\n') {
                    linha++;
                    coluna = 0;
                    estado = q0;
                } else if (c == '{') {
                    linhaInicioComment = linha;
                    colunaInicioComment = coluna;
                    while (c != '}' && c != EOF) {
                        c = getchar();
                        coluna++;
                        if (c == '\n') {
                            linha++;
                            coluna = 0;
                        }
                    }
                    if (c == EOF) {
                        strcpy(token.tipo, "ERR_UNCLOSED_COMMENT");
                        strcpy(token.lexema, "{");
                        token.linha = linhaInicioComment;
                        token.coluna = colunaInicioComment;
                        return token;
                    } else {
                        estado = q0;
                    }
                } else if (c == EOF) {
                    estado = q9;
                } else if (isalpha(c)) {
                    buf_idx = 0;
                    buffer[buf_idx++] = tolower(c);
                    estado = q1;
                } else if (isdigit(c)) {
                    buf_idx = 0;
                    buffer[buf_idx++] = c;
                    estado = q3;
                } else if (c == ':') {
                    estado = q5;
                } else if (c == ';') {
                    estado = q8;
                } else if (c == '>') {
                    estado = q_maior;
                } else if (c == '<') {
                    estado = q_menor;
                } else {
                    buffer[0] = c;
                    buffer[1] = '\0';
                    token.linha = linha;
                    token.coluna = coluna;
                    strcpy(token.lexema, buffer);
                    
                    switch(c) {
                        case '+': strcpy(token.tipo, "OP_AD"); return token;
                        case '-': strcpy(token.tipo, "OP_MIN"); return token;
                        case '*': strcpy(token.tipo, "OP_MUL"); return token;
                        case '/': strcpy(token.tipo, "OP_DIV"); return token;
                        case '=': strcpy(token.tipo, "OP_EQ"); return token;
                        case '.': strcpy(token.tipo, "SMB_DOT"); return token;
                        case ',': strcpy(token.tipo, "SMB_COM"); return token;
                        case '(': strcpy(token.tipo, "SMB_OPA"); return token;
                        case ')': strcpy(token.tipo, "SMB_CPA"); return token;
                        default: 
                            strcpy(token.tipo, "ERR_INVALID_CHAR");
                            estado = q_err;
                            goto end_dfa;
                    }
                }
                break;

            case q1:
                c = getchar();
                coluna++;
                if (isalnum(c) && buf_idx < 99) {
                    buffer[buf_idx++] = tolower(c);
                    estado = q1;
                } else {
                    estado = q2;
                }
                break;

            case q2:
                buffer[buf_idx] = '\0';
                if (c != EOF) {
                    ungetc(c, stdin);
                    coluna--;
                }
                classificaIdentificador(buffer, token.tipo);
                strcpy(token.lexema, buffer);
                token.linha = linha;
                token.coluna = coluna - buf_idx + 1;
                return token;
                if (strcmp(token.tipo, "ID") == 0) {
                 inserirTS(buffer);
}

            case q3:
                c = getchar();
                coluna++;
                if (isdigit(c) && buf_idx < 99) {
                    buffer[buf_idx++] = c;
                    estado = q3;
                } else if (c == '.' && buf_idx < 99) {
                    buffer[buf_idx++] = c;
                    estado = q_real_ponto;
                } else {
                    estado = q4;
                }
                break;

            case q4:
                buffer[buf_idx] = '\0';
                if (c != EOF) {
                    ungetc(c, stdin);
                    coluna--;
                }
                strcpy(token.tipo, "NUM_INT");
                strcpy(token.lexema, buffer);
                token.linha = linha;
                token.coluna = coluna - buf_idx + 1;
                return token;

            case q_real_ponto:
                c = getchar();
                coluna++;
                if (isdigit(c) && buf_idx < 99) {
                    buffer[buf_idx++] = c;
                    estado = q_real_aceita;
                } else {
                    buffer[buf_idx] = '\0';
                    if (c != EOF) { ungetc(c, stdin); coluna--; }
                    strcpy(token.tipo, "ERR_INVALID_REAL");
                    strcpy(token.lexema, buffer);
                    token.linha = linha;
                    token.coluna = coluna - buf_idx + 1;
                    return token;
                }
                break;

            case q_real_aceita:
                c = getchar();
                coluna++;
                if (isdigit(c) && buf_idx < 99) {
                    buffer[buf_idx++] = c;
                    estado = q_real_aceita;
                } else {
                    buffer[buf_idx] = '\0';
                    if (c != EOF) { ungetc(c, stdin); coluna--; }
                    strcpy(token.tipo, "NUM_REAL");
                    strcpy(token.lexema, buffer);
                    token.linha = linha;
                    token.coluna = coluna - buf_idx + 1;
                    return token;
                }
                break;

            case q5:
                c = getchar();
                coluna++;
                if (c == '=') {
                    estado = q6;
                } else {
                    estado = q7;
                }
                break;

            case q6:
                strcpy(token.tipo, "OP_ASS");
                strcpy(token.lexema, ":=");
                token.linha = linha;
                token.coluna = coluna - 1;
                return token;

            case q7:
                if (c != EOF) {
                    ungetc(c, stdin);
                    coluna--;
                }
                strcpy(token.tipo, "SMB_COL");
                strcpy(token.lexema, ":");
                token.linha = linha;
                token.coluna = coluna;
                return token;

            case q8:
                strcpy(token.tipo, "SMB_SEM");
                strcpy(token.lexema, ";");
                token.linha = linha;
                token.coluna = coluna;
                return token;

            case q_maior:
                c = getchar();
                coluna++;
                if (c == '=') estado = q_maior_igual;
                else estado = q_maior_aceita;
                break;

            case q_maior_igual:
                strcpy(token.tipo, "OP_GE");
                strcpy(token.lexema, ">=");
                token.linha = linha;
                token.coluna = coluna - 1;
                return token;

            case q_maior_aceita:
                if (c != EOF) { ungetc(c, stdin); coluna--; }
                strcpy(token.tipo, "OP_GT");
                strcpy(token.lexema, ">");
                token.linha = linha;
                token.coluna = coluna;
                return token;

            case q_menor:
                c = getchar();
                coluna++;
                if (c == '=') estado = q_menor_igual;
                else if (c == '>') estado = q_menor_difr;
                else estado = q_menor_aceita;
                break;

            case q_menor_igual:
                strcpy(token.tipo, "OP_LE");
                strcpy(token.lexema, "<=");
                token.linha = linha;
                token.coluna = coluna - 1;
                return token;

            case q_menor_difr:
                strcpy(token.tipo, "OP_NE");
                strcpy(token.lexema, "<>");
                token.linha = linha;
                token.coluna = coluna - 1;
                return token;

            case q_menor_aceita:
                if (c != EOF) { ungetc(c, stdin); coluna--; }
                strcpy(token.tipo, "OP_LT");
                strcpy(token.lexema, "<");
                token.linha = linha;
                token.coluna = coluna;
                return token;

            case q9:
                token.tipo[0] = '\0';
                return token;

            case q_err:
            end_dfa:
                token.linha = linha;
                token.coluna = coluna;
                return token;
        }
    }
}