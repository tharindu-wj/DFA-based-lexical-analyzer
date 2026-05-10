#include <stdio.h>
#include "lexer.h"
#include "logger.h"

int main(int argc, char **argv) {
    // reads from a file
    if (argc < 2) {
        printf("Usage: %s <input-file>\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        printf("Error: could not open file '%s'.\n", argv[1]);
        return 1;
    }

    const char *output_path = (argc >= 3) ? argv[2] : "tokens.out";
    FILE *output = fopen(output_path, "w");
    if (output == NULL) {
        printf("Error: could not open '%s'\n", argv[2]);
        fclose(input);
        return 1;
    }
    logger_set_output_file(output);


    int result = run_lexer(input);

    if (output) fclose(output);
    fclose(input);

    return 0;
}
