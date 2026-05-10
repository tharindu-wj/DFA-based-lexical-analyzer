#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dfa.h"
#include "keywords.h"
#include "logger.h"
#include "token.h"

#define LEXEME_MAX 256

int run_lexer(FILE *input) {
    // Initialise current state as START
    State currentState = START;

    char lexeme[LEXEME_MAX];
    char *write = lexeme;
    char *limit = lexeme + LEXEME_MAX - 1;

    int token_line = 1;
    int token_col  = 1;

    int line = 1, col = 1;
    int prev_col = 1;

    // process each character of the input file.
    int current_character;

    while ((current_character = fgetc(input)) != EOF) {
        prev_col = col;
        int just_read_line = line;
        int just_read_col  = prev_col;
        if (current_character == '\n') {
            line++;
            col = 1;
        } else {
            col++;
        }

        State next = dfa_transition(currentState, current_character);

        switch (currentState) {
            case START:
                if (next == DELIMITER) {
                    token_line = just_read_line;
                    token_col  = just_read_col;

                    // delimiter is a single character token
                    // log immediately without accumulation
                    char buf[2] = {(char) current_character, '\0'};
                    logger(buf, TOKEN_DELIMITER, token_line, token_col);
                    next = START;
                } else if (next == IDENTIFIER) {
                    token_line = just_read_line;
                    token_col  = just_read_col;

                    // start a new identifier
                    // reset write pointer and store first char.
                    write = lexeme;
                    *write++ = (char) current_character;
                } else if (next == NUMBER) {
                    token_line = just_read_line;
                    token_col  = just_read_col;

                    // start accumulating digits
                    write = lexeme;
                    *write++ = (char) current_character;
                } else if (next == OPERATOR) {
                    token_line = just_read_line;
                    token_col  = just_read_col;

                    // operator is a single character token
                    // log immediately without accumulation
                    char buf[2] = {(char) current_character, '\0'};
                    logger(buf, TOKEN_OPERATOR, token_line, token_col);
                    next = START;
                } else if (next == ERROR) {
                    token_line = just_read_line;
                    token_col  = just_read_col;

                    // Reset the write pointer and store the malformed character
                    write = lexeme;
                    // accumulate subsequent characters
                    *write++ = (char) current_character;
                }
                break;

            case IDENTIFIER:
                if (next == IDENTIFIER) {
                    *write++ = (char) current_character;
                } else {
                    // identifier ended
                    *write = '\0';
                    size_t length = (size_t) (write - lexeme);

                    char *token = malloc(length + 1);
                    memcpy(token, lexeme, length + 1);

                    TokenType type = classify_identifier(token);
                    logger(token, type, token_line, token_col);
                    free(token);

                    ungetc(current_character, input);

                    next = START;
                    write = lexeme;

                    if (current_character == '\n') {
                        line--;
                    }
                    col = prev_col;
                }
                break;

            case NUMBER:
                if (next == NUMBER) {
                    *write++ = (char) current_character;
                } else if (next == ERROR) {
                    *write++ = (char) current_character;
                } else {
                    // number ended
                    *write = '\0';
                    size_t length = (size_t) (write - lexeme);

                    char *token = malloc(length + 1);
                    memcpy(token, lexeme, length + 1);

                    logger(token, TOKEN_NUMBER, token_line, token_col);
                    free(token);

                    ungetc(current_character, input);

                    next = START;
                    write = lexeme;

                    if (current_character == '\n') {
                        line--;
                    }
                    col = prev_col;
                }
                break;

            case DELIMITER:
                break;

            case OPERATOR:
                break;
            case ERROR:
                if (next == ERROR) {
                    *write++ = (char) current_character;
                } else {
                    *write = '\0';
                    logger(lexeme, TOKEN_ERROR, token_line, token_col);
                    ungetc(current_character, input);
                    write = lexeme;
                    next = START;

                    if (current_character == '\n') {
                        line--;
                    }
                    col = prev_col;
                }
        }

        // transition to the next state based on the input
        currentState = next;
    }

    // flush any identifier still accumulated at end
    if (currentState == IDENTIFIER || currentState == NUMBER) {
        *write = '\0';
        size_t length = (size_t) (write - lexeme);
        char *token = malloc(length + 1);
        memcpy(token, lexeme, length + 1);
        if (currentState == IDENTIFIER) {
            TokenType type = classify_identifier(token);
            logger(token, type, token_line, token_col);
        } else {
            logger(token, TOKEN_NUMBER, token_line, token_col);
        }
        free(token);
    }
    else if (currentState == ERROR) {
        *write = '\0';
        logger(lexeme, TOKEN_ERROR, token_line, token_col);
    }

    return 0;
}