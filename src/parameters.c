/* parameters.c */

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
#include <libgen.h>
#include <getopt.h>

#include "suko.h" 

/*
 * Expected arguments are parameters that constrain the puzzle's
 * solution.  Positional parameters are, in order,
 *
 * - top left, top right, bottom left, bottom right circles (box)
 *   numbers
 * - color #1 value followed by four box #s, each one 1..9
 * - color #2 value followed by three box #s, each one 1..9
 * - color #3 value, followed by two box #s, each one 1..9
 *
 * Invoking the program example:
 *
 * suko 28 16 24 12  13 2 3 6 9  21 1 4 5  11 6 7   
 *      ^-- box --^  ^--  3 color parm sets   --^
 *
 * for a total of 16 puzzle parameters.
 */

#define PARMCOUNT 16 

#define OPTIONS "ghlv"

#define USAGE_FMT "Usage: %s [flags] tl tr bl br  a a1 a2 a3 a4  b b1 b2 b3  c c1 c2\n"
#define USAGE_KNT "Too few parameters (%d) to solve puzzle (%d needed).\n\n"
#define BADPARM_1 "Bad top-left circle value '%d' in parameter #%d; must be from 10 .. 30.\n\n"
#define BADPARM_2 "Bad top-right circle value '%d' in parameter #%d; must be from 10 .. 30.\n\n"
#define BADPARM_3 "Bad bottom-left circle value '%d' in parameter #%d; must be from 10 .. 30.\n\n"
#define BADPARM_4 "Bad bottom-right circle value '%d' in parameter #%d; must be from 10 .. 30.\n\n"
#define BADCOLR_1 "Bad color 1 sum '%d' in parameter #%d; must be from 10 .. 30.\n\n"
#define BADCOLR_2 "Bad color 2 sum '%d' in parameter #%d; must be from 6 .. 24.\n\n"
#define BADCOLR_3 "Bad color 3 sum '%d' in parameter #%d; must be from 3 .. 17.\n\n"
#define BADBOXNUM "Bad square index '%d' in parameter #%d; must be from 1 .. 9.\n\n"

extern char *optarg;
extern int  optind;
extern int  opterr;
extern int  optopt;

static void show_clue_values (clues_t clue);
static void bad_parameter (const char *msg, const int p, const int where);
static void usage (const char msg[], const char *progname);
static void help ();


void get_parameters (int argc, char *argv[], clues_t *clue)
{
	int  opt;
	int  verbose= 0;

	while ((opt = getopt(argc, argv, OPTIONS)) != EOF) {
		switch(opt) {
			case 'g':
				clue->gridwidth = GRID3X3;
				break;
			case 'l':
				clue->gridwidth = SUKOSIZE;
				break;
			case 'h':
				help();
				break;
			case 'v':
				verbose= 1;
				break;
			case '?':
			default:
				usage(USAGE_FMT, basename(argv[0]));
				break;
				/* never reached */
		}
	}

	/*
	 * We need enough clue parameters to find the answer.
	 */
	if ((argc - optind) < PARMCOUNT)
		bad_parameter(USAGE_KNT, argc - optind, PARMCOUNT);

	/*
	 * Walk along the postional parameter list, keeping them in the clue.
	 * Does it one at a time to afford unique error messages.
	 *
	 * First four are top left/right and bottom left/right circle clues.
	 */
	opt= atoi(argv[optind]);
	if (between(opt, CIRCLEMIN, CIRCLEMAX))
		clue->tl_sum= opt;
	else
		bad_parameter(BADPARM_1, opt, optind);
	optind++;

	opt= atoi(argv[optind]);
	if (between(opt, CIRCLEMIN, CIRCLEMAX))
		clue->tr_sum= opt;
	else
		bad_parameter(BADPARM_2, opt, optind);
	optind++;

	opt= atoi(argv[optind]);
	if (between(opt, CIRCLEMIN, CIRCLEMAX))
		clue->bl_sum= opt;
	else
		bad_parameter(BADPARM_3, opt, optind);
	optind++;

	opt= atoi(argv[optind]);
	if (between(opt, CIRCLEMIN, CIRCLEMAX))
		clue->br_sum= opt;
	else
		bad_parameter(BADPARM_4, opt, optind);
	optind++;

	/*
	 * First color clue followed by 4 indices for squares.
	 * Indices are -1 because array is zero-based.
	 */
	opt= atoi(argv[optind]);
	if (between(opt, COLOR1MIN, COLOR1MAX))
		clue->color1= opt;
	else
		bad_parameter(BADCOLR_1, opt, optind);
	optind++;

	opt= atoi(argv[optind]);
	if (between(opt, BOXIDXMIN, BOXIDXMAX))
		clue->c1box1= opt - 1;
	else
		bad_parameter(BADBOXNUM, opt, optind);
	optind++;

	opt= atoi(argv[optind]);
	if (between(opt, BOXIDXMIN, BOXIDXMAX))
		clue->c1box2= opt - 1;
	else
		bad_parameter(BADBOXNUM, opt, optind);
	optind++;

	opt= atoi(argv[optind]);
	if (between(opt, BOXIDXMIN, BOXIDXMAX))
		clue->c1box3= opt - 1;
	else
		bad_parameter(BADBOXNUM, opt, optind);
	optind++;

	opt= atoi(argv[optind]);
	if (between(opt, BOXIDXMIN, BOXIDXMAX))
		clue->c1box4= opt - 1;
	else
		bad_parameter(BADBOXNUM, opt, optind);
	optind++;

	/*
	 * Second color clue followed by 3 indices for three squares.
	 */
	opt= atoi(argv[optind]);
	if (between(opt, COLOR2MIN, COLOR2MAX))
		clue->color2= opt;
	else
		bad_parameter(BADCOLR_2, opt, optind);
	optind++;

	opt= atoi(argv[optind]);
	if (between(opt, BOXIDXMIN, BOXIDXMAX))
		clue->c2box1= opt - 1;
	else
		bad_parameter(BADBOXNUM, opt, optind);
	optind++;

	opt= atoi(argv[optind]);
	if (between(opt, BOXIDXMIN, BOXIDXMAX))
		clue->c2box2= opt - 1;
	else
		bad_parameter(BADBOXNUM, opt, optind);
	optind++;

	opt= atoi(argv[optind]);
	if (between(opt, BOXIDXMIN, BOXIDXMAX))
		clue->c2box3= opt - 1;
	else
		bad_parameter(BADBOXNUM, opt, optind);
	optind++;

	/*
	 * Third color clue followed by 2 indices for two squares.
	 */
	opt= atoi(argv[optind]);
	if (between(opt, COLOR3MIN, COLOR3MAX))
		clue->color3= opt;
	else
		bad_parameter(BADCOLR_3, opt, optind);
	optind++;

	opt= atoi(argv[optind]);
	if (between(opt, BOXIDXMIN, BOXIDXMAX))
		clue->c3box1= opt - 1;
	else
		bad_parameter(BADBOXNUM, opt, optind);
	optind++;

	opt= atoi(argv[optind]);
	if (between(opt, BOXIDXMIN, BOXIDXMAX))
		clue->c3box2= opt - 1;
	else
		bad_parameter(BADBOXNUM, opt, optind);
	optind++;

	/*
	 * Show the loaded clue parameters
	 */
	if (verbose)
		show_clue_values(*clue);

} /* get_parameters() */


/*
 * Pretty-print of the puzzle clue's values so the user can
 * inspect entered parameters.
 *
 * Indices (XXboxN) are adjusted +1 for display because they
 * were stored zero-based for C array.
 */
void show_clue_values (const clues_t clue)
{
	fprintf (stderr, "Values for suko puzzle solution.\n\n"
			"Circle values:\n\n"
			"  (%2d)  (%2d)\n"
			"  (%2d)  (%2d)\n\n"
			"Color 1 values: sum= %2d, squares= %2d %2d %2d %2d\n"
			"Color 2 values: sum= %2d, squares= %2d %2d %2d\n"
			"Color 3 values: sum= %2d, squares= %2d %2d\n\n"
			"Suko solution (if any):\n\n",
			clue.tl_sum, clue.tr_sum,
			clue.bl_sum, clue.br_sum,
			clue.color1, clue.c1box1+1, clue.c1box2+1, clue.c1box3+1, clue.c1box4+1,
			clue.color2, clue.c2box1+1, clue.c2box2+1, clue.c2box3+1,
			clue.color3, clue.c3box1+1, clue.c3box2+1
		);
}


static void bad_parameter (const char *msg, const int p, const int where)
{
	fprintf (stderr, msg, p, where);
	usage(USAGE_FMT, PROGNAME);
	/* never reached */
}


static void usage (const char *msg, const char *progname)
{
	fprintf(stderr, msg, progname ? progname : PROGNAME);
	exit(EXIT_FAILURE);
	/* never reached */
}


static void help()
{
	fprintf(stderr, 
			"\nSuko puzzle solver.\n\n"
			"Syntax\n\n"
			"  %s [flags] tl tr bl br  a a1 a2 a3 a4  b b1 b2 b3  c c1 c2\n\n"
			"where tl..br are the four numbers found in the puzzle grid's circles\n"
			"and a though c are the three collections of color constraints.  Each\n"
			"color constraint consists of the summed number and the indexes of the\n"
			"four, three, or two squares that add up to the sum.  Example:\n\n"
			"  suko 28 16 24 12   13 2 3 6 8   21 1 4 5   11 6 7\n\n"
			"In this example, '11' is the sum of the two numbers found the sixth\n"
			"& seventh squares of the 3x3 grid as numbered from the top left corner.\n\n"
			"Flags:\n\n"
			"  -g  write output as a 3x3 grid (default)\n\n"
			"  -l  write output in a line\n\n"
			"  -v  show suko puzzle input parameters before solving\n\n"
			"  -h  show this help.\n\n"
			"Version: %s\n\n",
			PROGNAME, PROGVERS);

	exit(EXIT_FAILURE);
}
