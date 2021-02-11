#ifdef LINKED_LIST

#else

#define LINKED_LIST 0

struct cell {
	struct cell *previous;
	struct cell *next;
	char current;
	char next_timestep;
};

typedef struct cell CELL;

#define RAND_LINE 0
#define ONE_LINE 1

// TODO: 
// 	Initialize element
// 	Initialize line
// 	Evaluate element
// 	Evaluate line
// 	Move cell to next timestep
// 	Move line to next timestep

int cell_init(CELL *self, CELL *previous, CELL *next, int type); 
CELL *line_init(int n, int type);
char cell_eval(CELL *self, char rule);
int line_eval(CELL *head, char rule, int n);
int line_update(CELL *head, int n);
int eval_timesteps(CELL *head, char rule, int width, int length); 
void line_print(CELL *head, FILE *fp, int n); 


int line_write_pbm(CELL *head, char *file, char width);
void line_convert(CELL *head, unsigned char *line, int width);
unsigned char bytes_to_bits(char a[], int n);
int make_raw_pbm(CELL *head, char *file, char rule, int width, int length);
#endif
