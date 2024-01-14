#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "utils.h"
#include "builtins.h"

#define NUM_TOKENS 1000
// the shell during its loop should
// read - read from std input
// parse - separate the command string into a program and arguments
// execute - run the parsed command

void armshloop() {
	char *line;
	char **args;
	int status;
	do {
		printf("🇦🇲");
		line = armsh_read_line();
		args = armsh_split_line(line);
		//status = armsh_execute(args);

		free(line);
		free(args);
	} while (status);		
}

#define ARMSH_RL_BUFSIZE 1024

char* armsh_read_line() {
	int position = 0;
	int bufsize = ARMSH_RL_BUFSIZE;
	char* buffer = malloc(sizeof(char) * bufsize);
	int c;

	if (!buffer) {
		fprintf(stderr, "armsh: allocation failed");
		exit(EXIT_FAILURE);
	}

	while (true) {
		// Read a character
		c = getchar();

		// if we encounter an EOF or a newline then replace it with the null character
		// otherwise add the character to the buffer
		if (c == EOF || c == '\n') {
			buffer[position] = '\0';
			return buffer;
		}
		else {
			buffer[position] = c;
		}

		position++;

		// if we exceed the buffer then we need to reallocate
		if (position >= bufsize) {
			bufsize *= 2;
			buffer = realloc(buffer, bufsize);
			if (!buffer) {
				fprintf(stderr, "armsh: reallocation failed");
				exit(EXIT_FAILURE);
			}
		}
	}
}

char** armsh_split_line(char* line) {
	int num_tokens = NUM_TOKENS;
	char** tokens = malloc(num_tokens * sizeof(char*));
	char* token = strtok(line, " ");
	int token_count = 0;

	if (!tokens) {
		fprintf(stderr, "armsh: allocation failed");
		exit(EXIT_FAILURE);
	}

	while (token != NULL) {
		tokens[token_count] = token;
		token_count++;
		token = strtok(NULL, " ");

		if (token_count >= num_tokens) {
			num_tokens *= 2;
			tokens = realloc(tokens, num_tokens);
			if (!tokens) {
				fprintf(stderr, "armsh: reallocation failed");
				exit(EXIT_FAILURE);
			}
		}
	}

	return tokens;
}

void armsh_loop(void) {
	char *line;
	char **args;
	int status;

	do {
		printf("🇦🇲\u00A0\u00A0");
		line = armsh_read_line();
		args = armsh_split_line(line);
		status = armsh_execute(args);

		free(line);
		free(args);
	} while (status);
}
