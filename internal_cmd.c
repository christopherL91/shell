#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "utils.h"
#include "constants.h"

/* POSIX */
#include <sys/resource.h>

/* TODO implement the rest of this function */
void internal_cmd_env() {
    int des_p[2];

    /* Programs to run (progp1 | prog2) */
    char* prog1[] = {"printenv", NULL};
    char* prog2[] = {"sort", NULL};

    if(pipe(des_p) == -1) {
        perror("Pipe failed");
        exit(EXIT_FAILURE);
    }

    if(fork() == 0) {     /* first child */
        close(1);           /* closing stdout */
        dup(des_p[1]);      /* replacing stdout with pipe write */
        close(des_p[0]);    /* closing pipe read */
        close(des_p[1]);

        if(execvp(prog1[0], prog1) == -1) {
            perror("Error while executing printenv");
            exit(EXIT_FAILURE);
        }
    }

    if(fork() == 0) {
        close(0);          /*closing stdin*/
        dup(des_p[0]);     /*replacing stdin with pipe read*/
        close(des_p[1]);   /*closing pipe write*/
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
    to = tokens[1];

    if(strcmp(to, "") == 0) {
        home = getenv("HOME");
        if (chdir(home)) {
           perror(NULL); 
        }
    }else {
        if (chdir(to)) {
            perror(NULL);
        }
    }
}

void internal_cmd_help() {
    char help[] = "\
Help!\n\
Hello and welcome to my awesome shell\n\
This shell implements a few of the most common\n\
utilities you will find in most modern shells.\n\n\
exit|quit|bye - Quits the current session\n\
help - Prints help page\n\
env - Prints out environment variables\n\
cd [directory] - Change directory\n";
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
