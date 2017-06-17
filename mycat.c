#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdline.h"

const char *stdinarg = "-";
char *append;

int
main (int argc, char **argv)
{
	struct gengetopt_args_info ai;
	if (cmdline_parser(argc, argv, &ai) != 0) {
		exit(EXIT_FAILURE);
	}

	FILE *curFile = NULL;
	char *lineptr = NULL;
	size_t n = 0;

	/* if no FILE argument is given */
	int nostdinarg = 0;
	if (ai.inputs_num == 0)
		nostdinarg = 1;

	/* show-ends */
	if (ai.show_ends_flag)
		append = "$\n";
	else
		append = "";

	int i;
	for (i = 0; i < ai.inputs_num || nostdinarg; ++i) {

		if (nostdinarg || strncmp(ai.inputs[i], stdinarg, strlen(stdinarg)) == 0) {
			curFile = stdin;
			nostdinarg = 0;
		}
		else {
			curFile = fopen(ai.inputs[i], "r");
		}

		ssize_t read;
		while (	(read = getline(&lineptr, &n, curFile)) != -1) {
			if (ai.show_ends_flag) {
				lineptr[read-1] = '\0';	// strip newline
			}

			printf("%s%s", lineptr, append);
		}

		fclose(curFile);
	}

	free(lineptr);

	exit(EXIT_SUCCESS);
}
