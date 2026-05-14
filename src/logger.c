#include <stdio.h>
#include "logger.h"
#include "token.h"

static FILE *output_file = NULL;

// Prints a token to stdout in the standard output format.
void logger(const char *lexeme, TokenType type, int token_line, int token_col) {
    const char *names[] = {"DELIMITER", "IDENTIFIER", "KEYWORD", "NUMBER", "OPERATOR", "ERROR"};

    static const char *const format = "Line %2d, Column %2d: %-10s \"%s\"\n";

    printf(format, token_line, token_col, names[type], lexeme);
    if (output_file != NULL) {
        fprintf(output_file, format, token_line, token_col, names[type], lexeme);
    }
}

void logger_set_output_file(FILE *file) {
    output_file = file;
}