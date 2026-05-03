#include <stdio.h>
#include <string.h>

// Define an enumeration for the DFA's states.
typedef enum {
    START,
} State;

int is_whitespace(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

// This function embodies the transition function δ.
// It takes the current state and the next input character,
// then returns the next state according to our DFA rules.
State transition(State currentState, char input) {
    switch (currentState) {
        case START:
            (void)input;
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

        // log characters without white spaces
        if (!is_whitespace(c)) {
            printf("character: '%c'\n", c);
        }
    }

    fclose(f);

    return 0;
}
