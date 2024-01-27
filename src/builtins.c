#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "../include/builtins.h"

// List of builtin commands, followed by their corresponding functions.

char *builtins[] = { "echo", "cd", "pwd", "help", "exit" };

// Array of function pointers / array of pointers to functions

int (*builtin_funcs[]) (char **) = {
	&armsh_echo,
	&armsh_cd,
	&armsh_pwd,
	&armsh_help,
	&armsh_exit
};

int armsh_num_builtins() {
	return sizeof(builtins) / sizeof(char *);
}

// builtin functions implementation
int armsh_echo (char **args) {
	if (!args[1]) {
		printf("\n");
	}
	else if (args[1] != NULL) {
		printf("%s\n", args[1]);
	}
	return 1;
}

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

int armsh_pwd (char **args) {
	if (args[1]) {
		fprintf(stderr, "armsh: Too many arguments\n");
		return 1;
	}

	long path_max;
	size_t size;
	char *buf;
	char *cwd;

	path_max = pathconf(".", _PC_PATH_MAX);
	if (path_max == -1)
		size = 1024;
	else if (path_max > 10240)
		size = 10240;
	else
		size = path_max;

	for (buf = cwd = NULL; cwd == NULL; size *= 2) {
		if ((buf = realloc(buf, size)) == NULL)
		{
			fprintf(stderr, "Error reallocating memory: %s\n", strerror(errno));
        	exit(EXIT_FAILURE);
		}

		cwd = getcwd(buf, size);
		if (cwd != NULL) {
			printf("%s\n", cwd);
		}
		if (cwd == NULL && errno != ERANGE) {
			fprintf(stderr, "Error getting current working directory: %s\n", strerror(errno));
        	exit(EXIT_FAILURE);
		}
	}
	free (buf);
	return 1;
}

int armsh_help (char **args) {
	printf("Armenian shell\n");
	printf("Below are the builtins\n");
	int builtins_count = armsh_num_builtins();
	for (int i = 0; i < builtins_count; i++) {
		printf(" %s\n", builtins[i]);
	}
	return 1;
}

int armsh_exit (char **args) {
	return 0;
}

int armsh_launch(char** args) {
	pid_t pid;
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
			waitpid(pid, &status, WUNTRACED);
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
