#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

int main(int argc, char **argv) {
	char *line;
	char **args;
	int status;

	printf("🇦🇲: ");
	line = armsh_read_line();
	args = armsh_split_line(line);

	// understand the below
	for (int i = 0; args[i] != NULL; i++) {
		printf("Token %d: %s\n", i + 1, args[i]);
	}
	
	free(line);
	free(args);
	return EXIT_SUCCESS;
}
