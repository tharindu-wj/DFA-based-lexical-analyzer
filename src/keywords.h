#ifndef KEYWORDS_H
#define KEYWORDS_H
#include "token.h"

const char *KEYWORDS[] = {
    "int", "char", "if", "else", "while", "for", "do", "return"
};

TokenType classify_identifier(const char *lexeme) {
    for (int i = 0; KEYWORDS[i] != NULL; i++) {
        if (strcmp(lexeme, KEYWORDS[i]) == 0) return TOKEN_KEYWORD;
    }
    return TOKEN_IDENTIFIER;
}

#endif