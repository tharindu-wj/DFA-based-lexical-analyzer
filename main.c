#include <stdio.h>
#include <string.h>

// Define an enumeration for the DFA's states.
typedef enum {
    EVEN,  // State representing an even count of '0's (also the accepting state)
    ODD    // State representing an odd count of '0's
} State;

// This function embodies the transition function δ.
// It takes the current state and the next input character,
// then returns the next state according to our DFA rules.
State transition(State currentState, char input) {
    switch (currentState) {
        case EVEN:
            if (input == '0')
                return ODD;     // Read a '0': toggles to ODD state.
            else
                return EVEN;    // Read a '1': stays in EVEN.
        case ODD:
            if (input == '0')
                return EVEN;    // Read a '0': toggles back to EVEN.
            else
                return ODD;     // Read a '1': stays in ODD.
    }
    // This return serves as a fallback.
    return currentState;
}

int main() {
    // Buffer to store user input (assumes strings of max length 99)
    char input_string[100];
    
    printf("Enter a binary string: ");
    scanf("%99s", input_string);
    
    // Initialise current state as EVEN (starting state where 0 zeros have been seen).
    State currentState = EVEN;
    
    // Process each character of the input string.
    for (int i = 0; input_string[i] != '\0'; i++) {
        char currentChar = input_string[i];
        
        // Validate input: DFA is defined only for '0' and '1'
        if (currentChar != '0' && currentChar != '1') {
            printf("Invalid input detected: '%c'. Only binary characters (0 and 1) are allowed.\n", currentChar);
            return 1;
        }
        // Transition to the next state based on the input.
        currentState = transition(currentState, currentChar);
    }
    
    // Check if the final state is accepting.
    if (currentState == EVEN) {
        printf("The string is accepted by the DFA (even number of 0's).\n");
    } else {
        printf("The string is rejected by the DFA (odd number of 0's).\n");
    }
    
    return 0;
}
