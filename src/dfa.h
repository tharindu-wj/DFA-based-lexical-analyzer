#ifndef DFA_H
#define DFA_H

// Define an enumeration for the DFA states.
typedef enum {
    START,
    DELIMITER,
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    ERROR
} State;

int is_whitespace(int c);
int is_delimiter(int c);
int is_digit(int c);
int is_operator(int c);
int is_letter(int c);
int is_underscore(int c);

State dfa_transition(State current_state, char input);

#endif
