#ifndef LOGGER_H
#define LOGGER_H
#include <stdio.h>
#include "token.h"

static FILE *output_file = NULL;

// Prints a token to stdout in the standard output format.
void logger(const char *lexeme, TokenType type, int token_line, int token_col) {
    const char *names[] = {"DELIMITER", "IDENTIFIER", "KEYWORD", "NUMBER", "OPERATOR", "ERROR"};
    if (type == TOKEN_ERROR) {
        printf("Line %2d, Column %2d: %-10s malformed token \"%s\"\n", token_line, token_col, names[type], lexeme);
    }else {
        printf("Line %2d, Column %2d: %-10s \"%s\"\n", token_line, token_col, names[type], lexeme);
    }

    if (output_file != NULL) {
        if (type == TOKEN_ERROR) {
            fprintf(output_file, "Line %2d, Column %2d: %-10s malformed token \"%s\"\n", token_line, token_col, names[type], lexeme);
        }else {
            fprintf(output_file, "Line %2d, Column %2d: %-10s \"%s\"\n", token_line, token_col, names[type], lexeme);
        }
    }

}

void logger_set_output_file(FILE *file) {
    output_file = file;
}

#endif
