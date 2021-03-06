#include <stdio.h>
#include <stdlib.h>

/* GNU */
#include <readline/readline.h>
#include <readline/history.h>

#include "utils.h"
#include "io.h"
#include "signals.h"

#define _XOPEN_SOURCE 500

int main() {
    signal(SIGQUIT, SIG_IGN);
    initial_signal();

    while(true) {
        char* line = NULL;
        char** tokens = NULL;
        int num_arguments = 0;

        line = read_input();
        if (line == NULL) {
            break;
        }
        tokens = tokenize(line, &num_arguments);
        eval(tokens, num_arguments);
        free_tokens(tokens);
    }
    return EXIT_SUCCESS;
}
