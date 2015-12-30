#ifndef UTILS_H
#define UTILS_H

typedef enum {
    false = 0,
    true  = 1
} bool;

void terminate_reason(pid_t, int);
void free_tokens(char**);
void print_tokens(char**);
void* getCurrentDir(char*);

#endif
