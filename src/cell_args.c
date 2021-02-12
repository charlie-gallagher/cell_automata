#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <cell_args.h>


#if defined(_MSC_VER)
#include <getopt.h>
#else
#include <unistd.h>
#endif


void print_help(void) {
	char buf[90];
	FILE *help;

	if ((help = fopen("./doc/cell_help.txt", "r")) == NULL)
		fprintf(stderr, "Error opening help file.\n");

	else {
		while(fgets(buf, 89, help) != NULL) {
			fputs(buf, stdout);
		}
	}
}



/* Inputs a pointer to an initialized CARGS structure
 * and outputs an integer indicating success or fairlure.
 *
 * The function sets defaults for the CARGS struct.
 *
 * Uses macros defined in cell_args.h
 */
int carg_init(CARGS *pcargs)
{
	HEIGHT = 25;
	WIDTH = 50;
	RULE = 26;
	I_POS = 1;
	OUT = NULL;

	return 0;
}

int carg_parse(CARGS *pcargs, int argc, char **argv)
{
	char c;
	carg_init(pcargs);

	while ((c = getopt(argc, argv, "w:h:o:r:i:")) != -1) {
		switch (c)
		{
			case 'w': {
					WIDTH = atoi(optarg);
					break;
				  }
			case 'h': {
					HEIGHT = atoi(optarg);
					break;
				  }
			case 'r': {
					RULE = atoi(optarg);
					break;
				  }
			case 'i': {
					I_POS = (char)atoi(optarg);
					break;
				  }
			case 'o': {
					OUT = optarg;
					break;
				  }
			#ifdef _MSC_VER
			case '#':
			#else
			case '?':
			#endif
			{
				if (optopt == 'w' ||
				    optopt == 'h' ||
				    optopt == 'r' ||
				    optopt == 'i' ||
			            optopt == 'o') {
				    fprintf(stderr,
					    "%c requires an argument.\n", optopt);
					    return -1;
				    }
				    else {
					  fprintf(stderr, "Unknown argument.\n");
					  return -1;
				    }
				  }
			default: {
					 fprintf(stderr, "Unknown outcome.\n");
					 return -1;
				 }
		}
	}
	return 0;
}


int carg_check(CARGS *pcargs)
{
	/* Check ranges */
	if (HEIGHT < 1 || HEIGHT > 20000) {
		fprintf(stderr, "Invalid height [%d]\n", HEIGHT);
		return -1;
	}
	else if (WIDTH < 1 || WIDTH > 20000) {
		fprintf(stderr, "Invalid width [%d]\n", WIDTH);
		return -1;
	}
	else if (I_POS != 1 && I_POS != 0) {
		fprintf(stderr, "Invalid initial position indicator [%d]\n", (int)I_POS);
		return -1;
	}
	else if (RULE < 0 || RULE > 255) {
		fprintf(stderr, "Invalid rule [%d]\n", RULE);
		return -1;
	}


	return 0;
}
