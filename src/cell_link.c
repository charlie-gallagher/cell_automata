#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cell_link.h"

/*
 * This file contains methods for the individual cells and the linked list as a
 * whole. For a specification of the linked list, see `cell_link.h`.
 *
 * The structure has elements:
 * 	- previous (pointer to CELL)
 * 	- next (pointer to CELL)
 * 	- current (char)
 * 	- next_timestep (char)
 *
 * The structure is typedef'd as CELL.
 */

/* Initialize cell
 *
 * Inputs three cell pointers, returns 1 for success, 0 for failure
 * Third pointer is usually NULL unless it's the last element, in which
 * case it gets set to the first element's address.
 *
 * type is one of RAND_LINE or ONE_LINE, defined in this file's header
 */
int cell_init(CELL *self, CELL *previous, CELL *next, int type)
{
	self->previous = previous;
	self->next = next;

	previous->next = self;

	switch(type) {
		case RAND_LINE: {
				self->current = (char)(rand() % 2);
				break;
			}
		case ONE_LINE: {
				self->current = 0;
				break;
			}
		default: {
				fprintf(stderr, "Error: invalid type\n");
				return -1;
			 }
	}
	self->next_timestep = 0;

	return 1;
}


/* Initialize first line
 *
 * Inputs head pointer and a number of elements to initialize. Memory is
 * allocated all at once for every pointer, with `head` being the first link
 *
 * type is either RAND_LINE (random) or ONE_LINE (single)
 */
CELL *line_init(int n, int type)
{
	CELL *head;
	CELL *new_elem;
	int i;

	srand(time(NULL)); // Initialize random seed for cell initialization

	head = calloc(n, sizeof(CELL));
	new_elem = head;
	if (head == NULL) {
		fprintf(stderr, "Memory allocation: Error allocating memory for cells.\n");
		return NULL;
	}

	/* Initialize first element */
	cell_init(new_elem, head, head, type);
	new_elem++;

	for (i = 1; i < n; i++) {
		if (cell_init(new_elem, new_elem - 1, NULL, type) != 1) {
			fprintf(stderr, "Error initializing line.\n");
			return NULL;
		}
		new_elem++;
	}

	/* Initialize 'next' of last element to 'first' */
	new_elem--;  // Iterated again at end, so set it back to last element
	new_elem->next = head;
	head->previous = new_elem;

	if (type == ONE_LINE) {
		new_elem = head + (n >> 1);
		new_elem->current = 1;
	}


	return head;
}

char cell_eval(CELL *self, char rule)
{
	char value = 2 * self->current +
		1 * (self->previous)->current +
		4 * (self->next)->current;

	return (rule >> value) & 1;
}



int line_eval(CELL *head, char rule, int n)
{
	int i;
	CELL *cell = head;

	for (i = 0; i < n; i++) {
		cell->next_timestep = cell_eval(cell, rule);
		cell++;
	}

	return 0;
}


int line_update(CELL *head, int n)
{
	int i;
	CELL *cell = head;

	for (i = 0; i < n; i++) {
		cell->current = cell->next_timestep;
		cell++;
	}
	return 0;
}


void line_print(CELL *head, FILE *fp, int n)
{
	int i;
	CELL *cell = head;

	for (i = 0; i < n; i++) {
		fprintf(fp, "%c", (cell->current == 1) ? '1' : '0');
		cell++;
	}
	putchar('\n');
}

/* Evaluates and prints lines
 *
 * This is the key function of the program. It takes the initialized data and
 * runs the `length` iterations and prints them to the screen.
 */
int eval_timesteps(CELL *head, char rule, int width, int length)
{
	int i;

	for (i = 0; i < length; i++) {
		printf("%03d: ", i);
		line_print(head, stdout, width);
		line_eval(head, rule, width);
		line_update(head, width);
	}
	return 0;
}


/* Writes a 'plain' PBM image, whic consists of ASCII 1s and 0s in an array
 *
 *	This is now defunct in favor of `make_raw_pbm`
 */
int make_plain_pbm(CELL *head, char *file, char rule, int width, int length)
{
	int i;
	FILE *fp;

	fp = fopen(file, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Error opening output file for writing.\n");
		return -1;
	}

	fprintf(fp, "P1\n%d %d\n", (int)width, (int)length);

	for (i = 0; i < length; i++) {
		line_print(head, fp, width);
		line_eval(head, rule, width);
		line_update(head, width);
	}

	fclose(fp);
	return 0;
}


/* Convert to raw PBM format
 *
 *	Raw PBM format is a magick number (P4) followed by width and height and
 *	an array of bits. If a bit is on, the square is black; otherwise, it's
 *	white. Efficient, but simple.
 */
int make_raw_pbm(CELL *head, char *file, char rule, int width, int length)
{
	int i;
	FILE *fp;
	unsigned char *line;

	fp = fopen(file, "wb");
	if (fp == NULL) {
		fprintf(stderr, "Error opening output file for writing.\n");
		return -1;
	}

	/* Line width is number of bytes per line when converted to pbm */
	int line_width = width / 8;
	if (width % 8 > 0) line_width++;


	/* Allocate memory for bit-line to be written to file */
	line = malloc(line_width);
	if (line == NULL) {
		fprintf(stderr, "Error allocating memory for line\n");
		return -1;
	}

	/* Write PBM header */
	fprintf(fp, "P4\n%d %d\n", (int)width, (int)length);


	for (i = 0; i < length; i++) {
		line_convert(head, line, width);
		fwrite(line, 1, line_width, fp);
		line_eval(head, rule, width);
		line_update(head, width);
	}

	free(line);
	fclose(fp);
	return 0;
}



void line_convert(CELL *head, unsigned char *line, int width)
{
	char byte[8];
	int i, j, n;
	CELL *cell = head;
	/* Get length of bits from bytes */
	int line_width = width / 8;
	if (width % 8 > 0) line_width++;



	/* Convert line to bytes 8 at a time */
	for (i = 0; i < line_width; i++) {
		/* Ensure don't overread line */
		n = 8;
		if ((i+1) * 8 > width)
			n = width - 8 * i;

		/* Fill array with N bits */
		for (j = 0; j < n; j++) {
			byte[j] = cell->current;
			cell++;
		}

		line[i] = bytes_to_bits(byte, n);
	}
}


unsigned char bytes_to_bits(char a[], int n)
{
	unsigned char out = 0;
	int i;

	/* Pad right-side of array with zeros if incomplete byte */
	if (n < 8) {
		for (i = n; i < 8; i++)
			a[i] = 0;
	}

	for (i = 0; i < 8; i++) {
		out += (a[i] << (7 - i));
	}

	return out;
}
