#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Shell version specified here */
#define SHELL_VERSION 0.6

#ifndef SIGDET
#define SIGDET 0
#endif

/* Specifies the length of a sub command */
#define MAX_COMMAND_LENGTH 80
/* Maximum number of sub commands */
#define MAX_NUM_COMMANDS 10

/* For the lulz */
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define UNDERSCORE         "\033[4m"

#endif
