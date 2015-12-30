#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

/* POSIX */
#include <sys/wait.h>
#include <sys/types.h>

#include "signals.h"

void execute(char** tokens) {
    pid_t pid;
    char* cmd;
    int status;

    if((pid = fork()) == 0) { /* Child */ 
        cmd = tokens[0];
        if(execvp(cmd, tokens) == -1) {
            fprintf(stderr, "Unknown command: %s\n", tokens[0]);
			exit(EXIT_FAILURE); 
        }
    } else if(pid < 0) { /* Error */
        fprintf(stderr, "fork: forking child process failed.\n");
		exit(EXIT_FAILURE);
    } else { /* Parent */ 
        waitpid(pid, &status, WUNTRACED | WCONTINUED);
    }
}

void execute_background(char** tokens) {

    if(SIGDET) {
        background_sig_setup();
    }

    if(fork() == 0) {
        if(signal(SIGQUIT, SIG_DFL) == SIG_ERR) {
            fprintf(stderr, "signal: failed - %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        if(signal(SIGINT, SIG_IGN) == SIG_ERR) {
            fprintf(stderr, "signal: failed - %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        
        execute(tokens);
        
        if(SIGDET) {
            /*
             * TODO Make this output nicer
            */
            fprintf(stdout,"\n%s has ended.\n", tokens[0]);
        }
        exit(EXIT_SUCCESS);
    }
}
