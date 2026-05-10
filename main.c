#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// maximum number of characters in a single lexeme
#define LEXEME_MAX 256

// Define an enumeration for the DFA's states.
typedef enum {
    START,
    DELIMITER,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    ERROR
} State;

// Define the token types.
typedef enum {
    TOKEN_DELIMITER,
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_ERROR
} TokenType;

const char *KEYWORDS[] = {
    "int", "char", "if", "else", "while", "for", "do", "return"
};

int is_whitespace(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int is_delimiter(int c) {
    return c == ';' || c == ',' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']';
}

int is_digit(int c) {
    return c >= '0' && c <= '9';
}

int is_operator(int c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

// if the character can start an identifier (letter or underscore)
int is_identifier_start(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

// if the character can continue an identifier (letter, digit, or underscore)
int is_identifier_continue(int c) {
    return is_identifier_start(c) || (c >= '0' && c <= '9');
}

// sync points for panic recovery
// marks the end of a malformed token to resume cleanly
int is_sync(int c) {
    // whitespace, delimiter, operator, and EOF are sync points
    return is_whitespace(c) || is_delimiter(c) || is_operator(c);
}

TokenType classify_identifier(const char *lexeme) {
    for (int i = 0; KEYWORDS[i] != NULL; i++) {
        if (strcmp(lexeme, KEYWORDS[i]) == 0) return TOKEN_KEYWORD;
    }
    return TOKEN_IDENTIFIER;
}

// Prints a token to stdout in the standard output format.
void logger(FILE *out, const char *lexeme, TokenType type, int token_line, int token_col) {
    const char *names[] = {"DELIMITER", "IDENTIFIER", "KEYWORD", "NUMBER", "OPERATOR", "ERROR"};
    if (type == TOKEN_ERROR) {
        printf("Line %2d, Column %2d: %-10s malformed token \"%s\"\n", token_line, token_col, names[type], lexeme);
    }else {
        printf("Line %2d, Column %2d: %-10s \"%s\"\n", token_line, token_col, names[type], lexeme);
    }

    if (out != NULL) {
        if (type == TOKEN_ERROR) {
            fprintf(out, "Line %2d, Column %2d: %-10s malformed token \"%s\"\n", token_line, token_col, names[type], lexeme);
        }else {
            fprintf(out, "Line %2d, Column %2d: %-10s \"%s\"\n", token_line, token_col, names[type], lexeme);
        }
    }

}

// This function embodies the transition function δ.
// It takes the current state and the next input character,
// then returns the next state according to our DFA rules.
State transition(State currentState, char input) {
    switch (currentState) {
        case START:
            // (START, whitespace) = START
            // stay in START.
            if (is_whitespace(input)) return START;

            // (START, delimiter_character) = DELIMITER
            // enter accept state.
            if (is_delimiter(input)) return DELIMITER;

            // (START, letter | _) = IDENTIFIER
            // begin accumulating identifier
            if (is_identifier_start(input)) return IDENTIFIER;

            // (START, digit) = NUMBER
            if (is_digit(input)) return NUMBER;

            // (START, operator_char) = OPERATOR
            if (is_operator(input)) return OPERATOR;

            // (START, other) = ERROR
            // unrecognised character
            return ERROR;

        case DELIMITER:
            return START;

        case IDENTIFIER:
            // (IDENTIFIER, letter | digit | _) = IDENTIFIER
            // continue accumulating
            if (is_identifier_continue(input)) return IDENTIFIER;
            // (IN_ID, other) = START
            // token ends
            return START;

        case NUMBER:
            // (NUMBER, digit) = NUMBER
            // continue accumulating
            if (is_digit(input)) return NUMBER;
            // malformed token sequences such as 123abc
            if (is_identifier_start(input)) return ERROR;
            // (NUMBER, other) = START
            // token ends
            return START;

        case OPERATOR:
            return START;

        case ERROR:
            // (ERROR, whitespace | delimiter | operator) = START
            // back up sync character then log
            if (is_sync(input)) return START;
            // (ERROR, digit | letter | _) = ERROR
            // keep collecting malformed sequence
            return ERROR;
    }

    // This return serves as a fallback.
    return currentState;
}

int main(int argc, char **argv) {
    // reads from a file
    if (argc < 2) {
        printf("Usage: %s <input-file>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("Error: could not open file '%s'.\n", argv[1]);
        return 1;
    }

    FILE *out = fopen("tokens.out", "w");
    if (!out) {
        printf("could not open tokens.out for writing; stdout only");
    }

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

    while ((current_character = fgetc(f)) != EOF) {
        prev_col = col;
        int just_read_line = line;
        int just_read_col  = prev_col;
        if (current_character == '\n') {
            line++;
            col = 1;
        } else {
            col++;
        }

        State next = transition(currentState, current_character);

        switch (currentState) {
            case START:
                if (next == DELIMITER) {
                    token_line = just_read_line;
                    token_col  = just_read_col;

                    // delimiter is a single character token
                    // log immediately without accumulation
                    char buf[2] = {(char) current_character, '\0'};
                    logger(out, buf, TOKEN_DELIMITER, token_line, token_col);
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
                    logger(out, buf, TOKEN_OPERATOR, token_line, token_col);
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
                    logger(out, token, type, token_line, token_col);
                    free(token);

                    ungetc(current_character, f);

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

                    logger(out, token, TOKEN_NUMBER, token_line, token_col);
                    free(token);

                    ungetc(current_character, f);

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
                    logger(out, lexeme, TOKEN_ERROR, token_line, token_col);
                    ungetc(current_character, f);
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
            logger(out, token, type, token_line, token_col);
        } else {
            logger(out, token, TOKEN_NUMBER, token_line, token_col);
        }
        free(token);
    }
    else if (currentState == ERROR) {
        *write = '\0';
        logger(out, lexeme, TOKEN_ERROR, token_line, token_col);
    }

    if (out) fclose(out);
    fclose(f);

    return 0;
}
