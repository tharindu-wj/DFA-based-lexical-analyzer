#include "dfa.h"

int is_whitespace(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int is_delimiter(int c) {
    return c == ';' || c == ',' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == '.';
}

int is_digit(int c) {
    return c >= '0' && c <= '9';
}

int is_operator(int c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>' || c == '!';
}

int is_letter(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_underscore(int c) {
    return c == '_';
}

// This function embodies the transition function of the DFA.
// It takes the current state and the next input character,
// then returns the next state according to our DFA rules.
State dfa_transition(State currentState, char input) {
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
            if (is_underscore(input) || is_letter(input)) return IDENTIFIER;

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
            if (is_underscore(input) || is_letter(input) || is_digit(input)) return IDENTIFIER;
            // (IN_ID, other) = START
            // token ends
            return START;

        case NUMBER:
            // (NUMBER, digit) = NUMBER
            // continue accumulating
            if (is_digit(input)) return NUMBER;
            // malformed token sequences such as 123abc
            if (is_underscore(input) || is_letter(input)) return ERROR;
            // (NUMBER, other) = START
            // token ends
            return START;

        case OPERATOR:
            return START;

        case ERROR:
            // (ERROR, whitespace | delimiter | operator) = START
            // back up sync character then log
            if (is_whitespace(input) || is_delimiter(input) || is_operator(input)) return START;
            // (ERROR, digit | letter | _) = ERROR
            // keep collecting malformed sequence
            return ERROR;
    }
}
