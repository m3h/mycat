#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdline.h"

const char *stdinarg = "-";
char *append;

char *changeTab(char *lineptr);

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

	/* for --numer */
	unsigned long noLines = 0;

	/* for squeeze-blank */
	int needsqueeze;

	int i;
	for (i = 0; i < ai.inputs_num || nostdinarg; ++i) {

		if (nostdinarg || strncmp(ai.inputs[i], stdinarg, strlen(stdinarg)) == 0) {
			curFile = stdin;
			nostdinarg = 0;
		}
		else {
			curFile = fopen(ai.inputs[i], "r");
			if (curFile == NULL) {
				printf("%s: %s: No such file or directory\n", argv[0], ai.inputs[i]);
				continue;
			}
		}

		ssize_t read;
		while (	(read = getline(&lineptr, &n, curFile)) != -1) {

			if (ai.squeeze_blank_flag) {
				if (strcmp("\n", lineptr) == 0) {
					if (!needsqueeze)
						needsqueeze = 1;
					else
						continue;
				}
			}
			else {
				needsqueeze = 0;
			}

			if (ai.show_ends_flag) {
				lineptr[read-1] = '\0';	// strip newline
			}

			if (ai.number_flag)
				printf("%6lu\t", ++noLines);

			if (ai.show_tabs_flag) {
				size_t k;
				for(k = 0; lineptr[k] != '\0'; ++k)
					if (lineptr[k] == '\t')
						printf("^I");
					else
						printf("%c", lineptr[k]);
				printf("%s", append);
			}
			else {
				printf("%s%s", lineptr, append);
			}
		}

		fclose(curFile);
	}

	free(lineptr);

	cmdline_parser_free (&ai);

	exit(EXIT_SUCCESS);
}
