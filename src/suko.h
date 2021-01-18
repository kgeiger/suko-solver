/* suko.h -- definitions and constants for suko puzzle solver. */

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
    along with this program.  If not, see <https://www.gnu.org/licenses/>
 */

#ifndef _SUKO_H
#define _SUKO_H

#define PROGNAME "suko"
#define PROGVERS "0.1"

/* Size of 3x3 puzzle grid */

#define SUKOSIZE 9
#define GRID3X3  3  
#define GRIDLINE 9


/* handy macros here */

#define between(X, LOW, HIGH) ((LOW) <= (X) && (X) <= (HIGH))

/*
 * Error-checking constraints on the values of command-line puzzle
 * parameters used as the clues.
 */

#define CIRCLEMIN 10  /* min, max for tl_sum .. br_sum */
#define CIRCLEMAX 30 

#define COLOR1MIN 10  /* sum 1..4 */
#define COLOR1MAX 30  /* sum 6..9 */
#define COLOR2MIN  6  /* sum 1..3 */
#define COLOR2MAX 24  /* sum 7..9 */
#define COLOR3MIN  3  /* sum 1..2 */
#define COLOR3MAX 17  /* sum 8..9 */
#define BOXIDXMIN  1  /* lowest grid index */
#define BOXIDXMAX  9  /* largest grid index */


/* Stucture to hold clues parsed from command line.
 * If a grid satisfies these parameters it solves
 * the suko puzzle.
 */

typedef struct {
	int gridwidth;  /* output in 3x3 grid or line */
	int tl_sum;     /* top-left thru bottom-right circles */
	int tr_sum;
	int bl_sum;
	int br_sum;
	int color1;     /* 1st color, sum of which boxen */
	int c1box1;
	int c1box2;
	int c1box3;
	int c1box4;
	int color2;     /* 2nd color, sum of which boxen */
	int c2box1;
	int c2box2;
	int c2box3;
	int color3;     /* 3rd color, sum of which boxen */
	int c3box1;
	int c3box2;
} clues_t;


#endif  /* suko.h */
