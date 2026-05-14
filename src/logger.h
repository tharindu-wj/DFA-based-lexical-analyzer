#ifndef LOGGER_H
#define LOGGER_H
#include <stdio.h>
#include "token.h"

void logger_set_output_file(FILE *file);

void logger(const char *lexeme, TokenType type, int token_line, int token_col);

#endif
