/* Argument parser for the cell program */
#ifndef CPARSE
#define CPARSE

typedef struct {
	int height;
	int width;
	unsigned char rule;
	char i_pos;
	char *out;
} CARGS;


#define HEIGHT pcargs->height
#define WIDTH pcargs->width
#define RULE pcargs->rule
#define I_POS pcargs->i_pos
#define OUT pcargs->out

void print_help(void);
int carg_init(CARGS *pcargs);
int carg_parse(CARGS *pcargs, int argc, char **argv);
int carg_check(CARGS *pcargs);



#endif
