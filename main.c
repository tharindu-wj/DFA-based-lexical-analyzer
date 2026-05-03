#include <stdio.h>
#include <string.h>

// Define an enumeration for the DFA's states.
typedef enum {
    START,
    IN_DELIMITER
} State;

// Define the token types.
typedef enum {
    TOKEN_DELIMITER,
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

// Prints a token to stdout in the standard output format.
void log(const char *lexeme, TokenType type) {
    const char *names[] = { "DELIMITER" };
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

            // (START, other) = START
            return START;

        case IN_DELIMITER:
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

    long count = 0; //

    // process each character of the input file.
    int c;
    while ((c = fgetc(f)) != EOF) {
        currentState = transition(currentState, c);

        // if entered IN_DELIMITER -> log the token immediately.
        // no accumulation needed.
        if (currentState == IN_DELIMITER) {
            char buf[2] = { (char)c, '\0' };
            log(buf, TOKEN_DELIMITER);

            // Transition back to START after logging.
            currentState = transition(currentState, c);
        }
    }

    fclose(f);

    return 0;
}
