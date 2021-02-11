#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cell_link.h>
#include <cell_args.h>

int main(int argc, char **argv) {

	/* Check for help */
	if (argc == 2 && strcmp(argv[1], "--help") == 0) {
		print_help();
		return 0;
	}


	/* Commandline arguments */
	CARGS cargs;
	CARGS *pcargs;
	pcargs = &cargs;

	if (carg_parse(pcargs, argc, argv) != 0) {
		fprintf(stderr, "Error parsing arguments\n");
		return -1;	
	}
	if (carg_check(pcargs) != 0) {
		fprintf(stderr, "Error in arguments\n");
		return -1;
	}

	printf("height: %d\nwidth: %d\ninitial: %d\nrule: %d\noutput: %s\n",
			HEIGHT, WIDTH, I_POS, RULE, OUT);


	/* Evaluation */
	CELL *head;
	head = line_init(WIDTH, I_POS);

	if (OUT != NULL) {
		make_raw_pbm(head, OUT, RULE, WIDTH, HEIGHT); 
	}
	else {
		eval_timesteps(head, RULE, WIDTH, HEIGHT);
	}


	free(head);

	return 0;
}



