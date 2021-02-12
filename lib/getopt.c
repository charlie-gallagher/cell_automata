#include "getopt.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>


char optopt, *optarg;
int optind, opterr;


int getopt(int argc, char **argv, char *format)
{
	extern char optopt;
	extern char *optarg;
	extern int optind;
	extern int opterr;

	/* Check if out of bounds yet */
	if (optind >= argc - 1) {
		return -1;
	}

	char *arg, *narg;
	char *f_loc;
	char c;

	arg = argv[optind + 1];

	/* Set next argument if valid */
	if (optind + 2 < argc)
		narg = argv[optind + 2];
	else
		narg = NULL;


	if (!isflag(arg)) {
		optopt = '#';
		opterr = NOT_FLAG;
		optind++;
		return '#';
	}

	c = arg[1];

	f_loc = strchr(format, c);

	if (f_loc == NULL) {
		optopt = c;
		opterr = INVALID_FLAG;
		optind++;
		return '#';
	}

	/* Set up format parsing variables */
	int f_max = strlen(format) - 1;
	int f_ind = f_loc - format;
	int f_rem = f_max - f_ind; // remaining in string. Should use \0


	if (f_rem > 0 && f_loc[1] == ':') {
		if (narg == NULL || isflag(narg)) {
			if (f_rem > 1 && f_loc[2] == ':')
				optarg = NULL;
			else {
				optopt = c;
				opterr = MISS_ARG;
				optind++;
				return '#';
			}
		}
		else {
			optarg = narg;
			optind++;
		}
	}


	optopt = c;
	optind++;
	return c;
}


int isflag(char *arg) {
	if (strlen(arg) != 2)
		return 0;

	if (arg[0] != '/')
		return 0;

	if (!isalpha(arg[1]))
		return 0;

	return 1;
}
