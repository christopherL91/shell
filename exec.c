#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

/* POSIX */
#include <sys/wait.h>
#include <sys/types.h>
#include <glob.h>

#include "signals.h"
#include "constants.h"

void execute(char** tokens) {
    pid_t pid;
    char* cmd;

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
        waitpid(pid, NULL, WUNTRACED | WCONTINUED);
    }
}

void execute_glob(char** tokens) {
    glob_t result;
    int i, status;
    int flags[2];

    flags[0] = GLOB_DOOFFS | GLOB_TILDE | GLOB_MARK |
        GLOB_BRACE | GLOB_NOMAGIC;
    flags[1] = GLOB_DOOFFS | GLOB_APPEND | GLOB_TILDE |
        GLOB_MARK | GLOB_BRACE | GLOB_NOMAGIC;

    result.gl_pathc = 0;
    result.gl_pathv = NULL;
    result.gl_offs = 1; /*  In order to save cmd */

    for(i = 1; i < MAX_NUM_COMMANDS && tokens[i] != NULL; i++) {
        if(i == 1) {
            status = glob(tokens[i], flags[0], NULL, &result);
        } else{
            status = glob(tokens[i], flags[1], NULL, &result);
        }
        if(status == GLOB_NOMATCH) {
            printf("No matches for wildcard '%s'\n", tokens[i]);
            break;
        }
    }
    if(result.gl_pathv != NULL) {
        result.gl_pathv[0] = tokens[0]; /* Set current cmd */
        execute(result.gl_pathv); /* Execute the expanded command */
        globfree(&result); /* free structure */
    }else {
        /*failed globbing*/
        execute(tokens);
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

        if(tokens[1] != NULL) {
            execute_glob(tokens);
        } else {
            execute(tokens);
        }

        if(SIGDET) {
            /*
             * TODO Make this output nicer
            */
            fprintf(stdout,"\n%s has ended.\n", tokens[0]);
        }
        exit(EXIT_SUCCESS);
    }
}
