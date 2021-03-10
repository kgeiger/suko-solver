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
#define PROGVERS "0.2"

/* Size of 3x3 puzzle grid */

#define SUKOSIZE 9
#define GRID3X3  3  
#define GRIDLINE 9

/* handy macros here */

#define between(X, LOW, HIGH) ((LOW) <= (X) && (X) <= (HIGH))

/* 
 * Stucture to hold clues parsed from command line.
 * If a grid satisfies these parameters it solves
 * the suko puzzle.
 */

struct color {
	int sum;
	int box[SUKOSIZE];
};

#define _COLOR_INIT {0, {-1, -1, -1, -1, -1, -1, -1, -1, -1}}

struct clue {
	int gridwidth;        /* output in 3x3 grid or line */
	int matrixonly;       /* when set, ignore color tests */
	int tl, tr, bl, br;   /* top-left thru bottom-right circles */
	struct color a;       /* 1st color, sum of which boxen */
	struct color b;       /* 2nd color, sum of which boxen */
	struct color c;       /* 3rd color, sum of which boxen */
};

#define _CLUES_INIT { \
	GRID3X3, \
	0, \
	0, 0, 0, 0, \
	_COLOR_INIT , \
	_COLOR_INIT , \
	_COLOR_INIT   \
}

#endif  /* suko.h */
