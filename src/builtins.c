#include <stdio.h>

int armsh_cd(char **args);
int armsh_help(char **args);
int armsh_exit(char **args);

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
