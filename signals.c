#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>

/*  handler for SIGCHLD and SIGINT signals.*/
void sig_handler(int signo) {
	if (signo == SIGCHLD) {
		waitpid(-1, NULL, WNOHANG);
	} else if (signo == SIGINT) {
		return;
	}
}

/* Only for SIGINT. */
void initial_signal() {
	struct sigaction sa;

	/* Clear signal mask so we can listen on all signals */
	if (sigemptyset(&sa.sa_mask) == -1) {
		fprintf(stderr, "sigemptyset: failed - %s.\n", strerror(errno));
	}
	/* Restart if neccesary */
	sa.sa_flags = SA_RESTART;
	/* Register callback function */
	sa.sa_handler = sig_handler;

	/* Listen for SIGINT */
	if (sigaction(SIGINT, &sa, NULL) == -1) {
		fprintf(stderr, "sigaction: failed - %s.\n", strerror(errno));
	}
}

/* Only for SIGCHLD */
void background_sig_setup() {
	struct sigaction sa;

    /* We want to recieve all signals by clearing the mask. */
	if (sigemptyset(&sa.sa_mask) == -1) {
		fprintf(stderr, "sigemptyset: failed - %s.\n", strerror(errno));
	}

	/* We want the program to continue execution. */
	sa.sa_flags = SA_RESTART;
	/* Callback function. */
	sa.sa_handler = sig_handler;

	/* Catch signal SIGCHLD and use callback function sig_handler.*/
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		fprintf(stderr, "sigaction: failed - %s.\n", strerror(errno));
	}
}
