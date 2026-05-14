#include <string.h>
#include "keywords.h"

#define KEYWORD_COUNT 8

const char *KEYWORDS[] = {
    "int", "char", "if", "else", "while", "for", "do", "return"
};

TokenType classify_identifier(const char *lexeme) {
    for (int i = 0; i < KEYWORD_COUNT; i++) {
        if (strcmp(lexeme, KEYWORDS[i]) == 0) return TOKEN_KEYWORD;
    }
    return TOKEN_IDENTIFIER;
}