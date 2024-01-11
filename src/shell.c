#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void armshloop();
char* armsh_read_line();
char* armsh_split_line(char* line);
bool armsh_execute(char* args);

int main(int argc, char **argv) {
	armshloop();
	return EXIT_SUCCESS;
}

// the shell during its loop should
// read - read from std input
// parse - separate the command string into a program and arguments
// execute - run the parsed command

void armshloop() {
	char *line;
	char **args;
	int status;
	do {
		printf("🇦🇲: ");
		line = armsh_read_line();
		args = armsh_split_line(line);
		status = armsh_execute(args);

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