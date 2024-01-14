#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "builtins.h"

// List of builtin commands, followed by their corresponding functions.

char *builtins[] = { "cd", "help", "exit" };

// Array of function pointers / array of pointers to functions

int (*builtin_funcs[]) (char **) = {
	&armsh_cd,
	&armsh_help,
	&armsh_exit
};

int armsh_num_builtins() {
	return sizeof(builtins) / sizeof(char *);
}

// builtin functions implementation

int armsh_cd (char **args) {
	if (!args[1]) {
		fprintf(stderr, "armsh: Expecting an argument to cd\n");
	}
	else {
		if (chdir(args[1]) != 0) {
			perror("armsh");
		}
	}
	return 1;
}

int armsh_help (char **args) {
	printf("Armenian shell\n");
	printf("Below are the builtins\n");
	for (int i = 0; i < armsh_num_builtins(); i++) {
		printf(" %s\n", builtins[i]);
	}
	return 1;
}

int armsh_exit (char **args) {
	return 0;
}

int armsh_launch(char** args) {
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0) {
		// Child process
		if (execvp(args[0], args) == -1) {
			perror("armsh");
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0) {
		// Error forking
		perror("armsh");
	}
	else {
		// Parent Process
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}

int armsh_execute (char **args) {
	if (!args[0]) {
		// An emtpy command was entered
		return 1;
	}

	for (int i = 0; i < armsh_num_builtins(); i++) {
		if (strcmp(args[0], builtins[i]) == 0) {
			return builtin_funcs[i](args);
		}
	}
	return armsh_launch(args);
}
