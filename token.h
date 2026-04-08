#ifndef TOKEN_H
#define TOKEN_H

typedef struct {
    char tipo[30];
    char lexema[100]; 
    int linha;
    int coluna;
} Token;

#endif 

