/* parameters.c - read puzzle clues from command line parameters */

/*
    suko - suko puzzle solver using brute force
 
    Copyright (c) 2021 Karl Geiger
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/> * 
 */

#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <libgen.h>
#include <getopt.h>

#include "suko.h" 

/*
 * Expected arguments are parameters that constrain the puzzle's
 * solution.
 *
 * Invoking the program example with miminum input:
 *
 *   suko -s 28,16,24,12  -a 13,2,3,6,9  -b 21,1,4,5  -c 11,6,7   
 *   
 *         ^- grid circs   ^- 1st color   ^- 2nd color ^- 3rd color
 */

#define OPTIONS ":a:b:c:ghlMs:v"

/*
 * Input data checks/constraints and user error messages.
 */

#define CIRCLEMIN 10  /* min, max for tl_sum .. br_sum */
#define CIRCLEMAX 30  /* min is sum(1..4); max is sum(6..9) */ 

#define COLORMIN   3  /* min value is sum(1..2) -- expect at least 2 boxes */
#define COLORMAX  30  /* max value is sum(6..9) -- expect at least 4 boxes */
#define BOXIDXMIN  1  /* lowest grid index */
#define BOXIDXMAX  9  /* largest grid index */

#define USAGE_FMT "Usage: %s [flags] -s tl,tr,bl,br  -a as,a1,a2,a3,a4  -b bs,b1,b2,b3  -c cs,c1,c2\n"
#define USAGE_KNT "Too few clues %d to solve puzzle, %d needed.\n\n"
#define USAGE_PRM "Bad parameter '-%c' at %d.\n\n"
#define USAGE_BOX "Too few parameters %d for box clues, need at least %d.\n\n"
#define BADCIRC_1 "Too few circle clues %d, total of %d needed.\n\n"
#define BADCIRC_2 "Bad circle clue '%d' at clue #%d; must be from 10..30.\n\n"
#define BADCOLR_1 "Bad color clue; sum '%d' at #%d; must be from 3..30.\n\n"
#define BADBOXNUM "Bad square index '%d' in clue #%d; must be from 1 .. 9.\n\n"

extern char *optarg;
extern int  optind;
extern int  opterr;
extern int  optopt;

static void get_circle_clues(struct clue *p, char *digits);
static void get_color_clues(struct color *c, char *digits);
static void show_clue_values(struct clue p);
static void bad_parameter(const char *msg, const int value, const int where);
static void usage(const char msg[], const char *progname);
static void help();

void get_parameters(int argc, char *argv[], struct clue *p)
{
	int  opt;
	int  verbose= false;

	while ((opt = getopt(argc, argv, OPTIONS)) != EOF) {

		switch(opt) {
		case 'a':
			get_color_clues(&p->a, optarg);
			break;
		case 'b':
			get_color_clues(&p->b, optarg);
			break;
		case 'c':
			get_color_clues(&p->c, optarg);
			break;
		case 'g':
			p->gridwidth = GRID3X3;
			break;
		case 'l':
			p->gridwidth = GRIDLINE;
			break;
		case 'h':
			help();
			/* never reached */
			break;
		case 'M':
			p->matrixonly = true;
			break;
		case 's':
			get_circle_clues(p, optarg);
			break;
		case 'v':
			verbose = 1;
			break;
		case ':':
			bad_parameter(USAGE_PRM, opt, optind);
		case '?':
		default :
			usage(USAGE_FMT, basename(argv[0]));
			/* never reached */
			break;
		} /* switch (opt) .. */

	} /* while (opt = .. */

	if (verbose)
		show_clue_values(*p);

} /* get_parameters() */

static void get_circle_clues(struct clue *p, char *numbers)
{
	int count = 0;

	count = sscanf(numbers, "%d,%d,%d,%d",
			&p->tl, &p->tr, &p->bl, &p->br);

	if (count < 4)
		bad_parameter(BADCIRC_1, count, 4);

	if (!between(p->tl, CIRCLEMIN, CIRCLEMAX))
		bad_parameter(BADCIRC_2, p->tl, 1);

	if (!between(p->tr, CIRCLEMIN, CIRCLEMAX))
		bad_parameter(BADCIRC_2, p->tr, 2);

	if (!between(p->bl, CIRCLEMIN, CIRCLEMAX))
		bad_parameter(BADCIRC_2, p->bl, 3);

	if (!between(p->br, CIRCLEMIN, CIRCLEMAX))
		bad_parameter(BADCIRC_2, p->br, 4);
}

static void get_color_clues(struct color *c, char *numbers)
{
	int i;
	int count = 0;

	count = sscanf(numbers, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&c->sum,
			&c->box[0], &c->box[1], &c->box[2],
			&c->box[3], &c->box[4], &c->box[5],
			&c->box[6], &c->box[7], &c->box[8]);

	if (count < 3)
		bad_parameter(USAGE_BOX, count, 3);

	if (! between(c->sum, COLORMIN, COLORMAX))
		bad_parameter(BADCOLR_1, c->sum, optind);

	for (i = 0; i < count-1; i++) {

		if (!between(c->box[i], BOXIDXMIN, BOXIDXMAX))
			bad_parameter(BADBOXNUM, c->box[i], i+1);

		c->box[i]--;  /* zero-base index for tests later */
	}
}

/*
 * Pretty-print of the puzzle clue's values so the user can inspect
 * entered parameters.  Invoked when -v (verbose) requested.
 *
 * Indices (c.box[n]) are adjusted +1 for display because we
 * stored 'em zero-based for testing the grid array.
 */
static void show_color_values(const struct color c, char *name)
{
	int i;

	fprintf(stderr, "Color %s values: sum=%2d, squares=", name, c.sum);

	for (i= 0; 0 <= c.box[i];  i++)
		fprintf(stderr, " %2d", c.box[i] + 1);

	fprintf(stderr, "\n");
}

static void show_clue_values(const struct clue p)
{
	fprintf(stderr, "Values for suko puzzle solution.\n\n"
			"Circle values:\n\n"
			"  (%2d)  (%2d)\n"
			"  (%2d)  (%2d)\n\n",
			p.tl, p.tr,
			p.bl, p.br);

	show_color_values(p.a, "a");
	show_color_values(p.b, "b");
	show_color_values(p.c, "c");

	if (p.matrixonly)
		fprintf(stderr, "Will display all 3x3 grids, not solution.\n\n");

	fprintf(stderr, "\nSuko solution (if any):\n\n");
}

/*
 * Emit error msgs and exit.  None of these return to main().
 */
static void bad_parameter(const char *msg, const int value, const int where)
{
	fprintf (stderr, msg, value, where);

	usage(USAGE_FMT, PROGNAME);
}

static void usage(const char *msg, const char *progname)
{
	fprintf(stderr, msg, progname ? progname : PROGNAME);

	exit(EXIT_FAILURE);
}

static void help()
{
	fprintf(stderr, 
		"\nSuko puzzle solver.\n\n"
		"Syntax\n\n"
		"  %s [flags] -s tl,tr,bl,br -a as,a1,a2,a3,a4 -b bs,b1,b2,b3 -c cs,c1,c2\n\n"
		"where tl..br are the four numbers found in the puzzle grid's circles\n"
		"(top left, top right, bottom left, bottom right) and a though c are\n"
	        "three collections of 'color' constraints.\n\n"
	        "Each color constraint consists of the summed number and the indexes of the\n"
		"squares that add up to the sum.  Example:\n\n"
		"  %s -s 28,16,24,12  -a 13,2,3,6,8  -b 21,1,4,5 -c 11,6,7\n\n"
		"In this example, '11' is the sum of the two numbers found the sixth and\n"
		"seventh squares of the 3x3 grid as numbered from the top left corner.\n\n"
		"Flags:\n\n"
		"  -g  write output as a 3x3 grid (default)\n\n"
		"  -l  write output as a line (useful when piping output)\n\n"
		"  -M  show all 3x3 grids that match tl .. br. Ignores -a, -b, and -c clues\n\n"
		"  -v  show suko puzzle input parameters before solving (verbose)\n\n"
		"  -h  show this help and quit.\n\n"
		"Version: %s\n\n",
		PROGNAME, PROGNAME, PROGVERS);

	exit(EXIT_FAILURE);
}
