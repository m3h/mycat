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

			if (ai.show_tabs_flag) {
				char *temp = changeTab(lineptr);
				free(lineptr);
				lineptr = temp;
				n = strlen(lineptr) + 1;
			}

			if (ai.show_ends_flag) {
				lineptr[read-1] = '\0';	// strip newline
			}

			if (ai.number_flag)
				printf("%6lu\t", ++noLines);

			printf("%s%s", lineptr, append);
		}

		fclose(curFile);
	}

	free(lineptr);

	exit(EXIT_SUCCESS);
}

char *
changeTab(char *lineptr)
{
	int i;
	int n = strlen(lineptr);

	unsigned int tabCount = 0;
	for(i = 0; i < n; ++i)
		if (lineptr[i] == '\t')
			++tabCount;

	char *newString = malloc(n + tabCount + 1);	// 1 for '\0'

	int j;
	for(i = 0, j = 0; i < n; ++i)
		if (lineptr[i] == '\t') {
			newString[j++] = '^';
			newString[j++] = 'I';
		}
		else {
			newString[j++] = lineptr[i];
		}

	return newString;
}
			
