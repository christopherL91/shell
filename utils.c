#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/wait.h>
#include <sys/types.h>

#include "constants.h"

/*A simple utility to show the reason why a process terminated*/
void terminate_reason(pid_t pid, int status) {
    printf("%c[2K", 27); /* Clear current line */
    printf("\rPID : %d\nReason: ", pid);

    if (WIFEXITED(status)) {
        printf("Exited with status = %d\n" , WEXITSTATUS(status));
    }else if (WIFSIGNALED(status)) {
        printf("Killed by signal %d\n", WTERMSIG(status));
    }else if (WIFSTOPPED(status)) {
        printf("Stopped by signal %d\n", WSTOPSIG(status));
    }else if (WIFCONTINUED(status)) {
        printf("Continued\n");
    }
}

void *getCurrentDir(char *path){
    char *token;
    char *directory;
    size_t length;

    token = strrchr(path, '/');

    if (token == NULL) {
        fprintf(stderr, "Error getting current directory name\n");
        exit(1);
    }

    length = strlen(token);
    directory = malloc(length);
    /*Don't forget null*/
    memcpy(directory, token+1, length);

    return directory;
}

/*Called before shell exit*/
void free_tokens(char** tokens) {
    int i;
    for(i = 0; i < MAX_NUM_COMMANDS; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

/*For debugging purposes*/
void print_tokens(char** tokens) {
    int i;
    for(i = 0; i < MAX_NUM_COMMANDS; i++) {
        if(strcmp(tokens[i], "") != 0) {
            printf("Token %d = %s\n", i, tokens[i]);
        }
    }
}

