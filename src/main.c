#include <stdio.h>
#include "lexer.h"
#include "logger.h"

// maximum number of characters in a single lexeme
#define LEXEME_MAX 256


// sync points for panic recovery
// marks the end of a malformed token to resume cleanly
int is_sync(int c) {
    // whitespace, delimiter, operator, and EOF are sync points
    return is_whitespace(c) || is_delimiter(c) || is_operator(c);
}

int main(int argc, char **argv) {
    // reads from a file
    if (argc < 2) {
        printf("Usage: %s <input-file>\n", argv[0]);
        return 1;
    }

    FILE *input  = fopen(argv[1], "r");
    if (!input ) {
        printf("Error: could not open file '%s'.\n", argv[1]);
        return 1;
    }

    FILE *output = NULL;
    if (argc >= 3) {
        output = fopen(argv[2], "w");
        if (output == NULL) {
            printf("Error: could not open '%s'\n", argv[2]);
            fclose(input);
            return 1;
        }
        logger_set_output_file(output);
    }

    int result = run_lexer(input);

    if (output) fclose(output);
    fclose(input);

    return 0;
}
