#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "utils.h"
#include "constants.h"

/* POSIX */
#include <sys/resource.h>
#include <glob.h>

/* TODO implement the rest of this function */
void internal_cmd_env() {
    int des_p[2];

    /* Programs to run progp1 | prog2 */
    char* prog1[] = {"printenv", NULL};
    char* prog2[] = {"sort", NULL};

    if(pipe(des_p) == -1) {
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }

    if(fork() == 0) {       /* First child */
        close(1);           /* Closing stdout */
        dup(des_p[1]);      /* Replacing stdout with pipe write */
        close(des_p[0]);    /* Closing pipe read */
        close(des_p[1]);

        if(execvp(prog1[0], prog1) == -1) {
            perror("Error while executing printenv");
            exit(EXIT_FAILURE);
        }
    }

    if(fork() == 0) {       /*  Second child */
        close(0);           /*  Closing stdin */
        dup(des_p[0]);      /*  Replacing stdin with pipe read */
        close(des_p[1]);    /*  Closing pipe write */
        close(des_p[0]);

        if(execvp(prog2[0], prog2) == -1) {
            perror("Error while executing sort");
            exit(EXIT_FAILURE);
        }
    }

    /* Cleanup */
    close(des_p[0]);
    close(des_p[1]);
    wait(NULL);
    wait(NULL);
}

void internal_cmd_cd(char** tokens) {
    char* home;
    char* to;
    glob_t globbuf;

    to = tokens[1]; /* destination */

    if(to == NULL) {
        if((home = getenv("HOME")) == NULL) {
            fprintf(stderr, "Error while attempting to get env variable HOME\n");
            return;
        }
        if (chdir(home) == -1) {
            perror("Could not change directory");
            return;
        }
    }else {
        if(glob(to, GLOB_TILDE | GLOB_MARK, NULL, &globbuf) != 0) {
            perror("Error while globbing");
            return;
        }
        if (chdir(globbuf.gl_pathv[0])) {
            perror("Error while changing directory");
            globfree(&globbuf);
            return;
        }
        globfree(&globbuf);
    }
}

void internal_cmd_help() {
    static char help[] = "\
Hello and welcome to my awesome shell\n\
This shell implements a few of the most common\n\
utilities you will find in most modern shells.\n\n\
exit|quit|bye - Quits the current session\n\
help - Prints help page\n\
env - Prints out environment variables\n\
cd [directory] - Change directory\n\
license - Show licese notice\n\
version - Show software version\n";
    printf(ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET, help);
}

void internal_cmd_proc() {
    struct rusage r_usage;
    getrusage(RUSAGE_SELF,&r_usage);
    printf("Memory usage: %ld bytes\n", r_usage.ru_maxrss);
}

void internal_cmd_quit() {
    kill(0, SIGQUIT);
    exit(EXIT_SUCCESS);
}

void internal_cmd_version() {
    printf("Version: %.2f\n", SHELL_VERSION);
}
