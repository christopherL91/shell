#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* GNU */
#include <readline/readline.h>
#include <readline/history.h>

/* Posix */
#include <sys/time.h>


#include "constants.h"
#include "utils.h"
#include "exec.h"
#include "internal_cmd.h"

char* read_input() {
    char* line; 
    char prompt[1024] = ANSI_COLOR_MAGENTA "\\ʕ◔ϖ◔ʔ/ : " ANSI_COLOR_RESET
        ANSI_COLOR_BLUE UNDERSCORE;
    char* cwd;
    char* directory_name;
    
    cwd = getcwd(0, 0);
    directory_name = getCurrentDir(cwd);

    strcat(prompt, directory_name);
    strcat(prompt, ANSI_COLOR_RESET);

    free(directory_name);
    
    /* 
     *  Read input from user and render
     *  command prompt
    */
    line = readline(strcat(prompt, ANSI_COLOR_GREEN " >> " ANSI_COLOR_RESET));
    
    if(line != NULL) {
        if(line[0] != 0) {
            add_history(line);
        }
        return line;
    }
    free(line);
    return NULL;
}

char** tokenize(char* line, int* num_arguments) {
    char* token;
    char **tokens; 
    int i;
    
    tokens = calloc(MAX_NUM_COMMANDS * sizeof(char*), sizeof(char*));    

    token = strtok(line, " ");
    for(i = 0; i < MAX_NUM_COMMANDS && token; i++) {
        tokens[i] = calloc(MAX_COMMAND_LENGTH + 1, sizeof(char*)); 
        memcpy(tokens[i], token, MAX_COMMAND_LENGTH);
        token = strtok(NULL, " ");
    }
    *num_arguments = i;
    free(line);
    return tokens;
}

void eval(char** tokens, int num_arguments) {
    char* cmd;
	struct timeval begin, end;
	long elapsed;
    cmd = tokens[0]; /* First command */

    /* User entered a space */
    if(cmd == NULL) {
        return;
    }

    if(strcmp(cmd, "cd") == 0) {
        internal_cmd_cd(tokens);
    } else if(strcmp(cmd, "help") == 0) {
        internal_cmd_help();
    } else if(strcmp(cmd, "bye") == 0) {
        internal_cmd_quit();
    } else if (strcmp(cmd, "quit") == 0) {
        internal_cmd_quit();
    } else if(strcmp(cmd, "exit") == 0) {
        internal_cmd_quit();
    } else if(strcmp(cmd, "env") == 0) {
        internal_cmd_env();
    } else if(strcmp(cmd, "proc") == 0) {
        internal_cmd_proc();
    } else if(strcmp(cmd, "version") == 0) {
        internal_cmd_version();
    } else if(strcmp(tokens[num_arguments - 1], "&") == 0) {
        tokens[num_arguments - 1] = NULL;
        execute_background(tokens);
    } else {
        gettimeofday(&begin, 0);
		execute(tokens);
		gettimeofday(&end, 0);
		elapsed = (end.tv_sec-begin.tv_sec)*1000000 + end.tv_usec-begin.tv_usec;
		printf(ANSI_COLOR_CYAN UNDERSCORE "time: %.3fs\n" ANSI_COLOR_RESET, (double)elapsed/1000000);
    }
}
