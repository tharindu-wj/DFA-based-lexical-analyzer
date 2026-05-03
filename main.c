#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// maximum number of characters in a single lexeme
#define LEXEME_MAX 256

// Define an enumeration for the DFA's states.
typedef enum {
    START,
    IN_DELIMITER,
    IN_IDENTIFIER
} State;

// Define the token types.
typedef enum {
    TOKEN_DELIMITER,
    TOKEN_IDENTIFIER
} TokenType;

int is_whitespace(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int is_delimiter(int c) {
    return c == ';' || c == ',' ||
           c == '(' || c == ')' ||
           c == '{' || c == '}' ||
           c == '[' || c == ']';
}

// if the character can start an identifier (letter or underscore)
int is_identifier_start(int c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

// if the character can continue an identifier (letter, digit, or underscore)
int is_identifier_continue(int c) {
    return is_identifier_start(c) || (c >= '0' && c <= '9');
}

// Prints a token to stdout in the standard output format.
void logger(const char *lexeme, TokenType type) {
    const char *names[] = { "DELIMITER", "IDENTIFIER" };
    printf("%-10s \"%s\"\n", names[type], lexeme);
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

            // (START, delimiter_character) = IN_DELIMITER
            // enter accept state.
            if (is_delimiter(input))  return IN_DELIMITER;

            // (START, letter | _) = IN_IDENTIFIER
            // begin accumulating identifier
            if (is_identifier_start(input)) return IN_IDENTIFIER;

            // (START, other) = START
            return START;

        case IN_DELIMITER:
            return START;

        case IN_IDENTIFIER:
            // (IN_IDENTIFIER, letter | digit | _) = IN_IDENTIFIER
            // continue accumulating
            if (is_identifier_continue(input)) return IN_IDENTIFIER;
            // (IN_ID, other) = START
            // token ends
            return START;
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

    // Initialise current state as START
    State currentState = START;

    char  lexeme[LEXEME_MAX];
    char *write = lexeme;
    char *limit = lexeme + LEXEME_MAX - 1;

    // process each character of the input file.
    char current_character;
    while ((current_character = fgetc(f)) != EOF) {
        State next = transition(currentState, current_character);

        switch (currentState) {
            case START:
                if (next == IN_DELIMITER) {
                    // delimiter is a single character token
                    // log immediately without accumulation.
                    char buf[2] = { (char)current_character, '\0' };
                    logger(buf, TOKEN_DELIMITER);
                    next = START;
                } else if (next == IN_IDENTIFIER) {
                    // start a new identifier
                    // reset write pointer and store first char.
                    write = lexeme;
                    *write++ = (char)current_character;
                }
                break;

            case IN_IDENTIFIER:
                if (next == IN_IDENTIFIER) {
                    *write++ = (char)current_character;
                } else {
                    // identifier ended
                    *write = '\0';
                    size_t length = (size_t)(write - lexeme);

                    char  *token = malloc(length + 1);
                    memcpy(token, lexeme, length + 1);

                    logger(token, TOKEN_IDENTIFIER);
                    free(token);

                    ungetc(current_character, f);

                    next  = START;
                    write = lexeme;
                }
                break;

            case IN_DELIMITER:
                break;
        }

        currentState = next;
    }

    // flush any identifier still accumulated at end
    if (currentState == IN_IDENTIFIER) {
        *write = '\0';
        size_t length = (size_t)(write - lexeme);
        char  *token = malloc(length + 1);
        memcpy(token, lexeme, length + 1);

        logger(token, TOKEN_IDENTIFIER);

        free(token);
    }

    fclose(f);

    return 0;
}
