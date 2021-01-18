/* main.c */

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

/* 1. includes */

#include <stdio.h>
#include <stdlib.h>

#include "suko.h"
#include "grids.h"
#include "parameters.h"

/* 2. defines */

#define NOTFOUND  0
#define FOUND    -1

/* 3. external declarations */


/* 4. typdefs */


/* 5. globals */


/* 6. local variables */

static int8_t suko_grids[][SUKOSIZE]= _GRID_PERMUTATIONS; 
static int    suko_count= sizeof(suko_grids)/sizeof(suko_grids[0]);

/* 7. function prototypes */

int  find_solution (const clues_t clue);
int  is_suko (const clues_t clue, const int8_t grid[]);
void write_answer (const int8_t grid[], int gridwidth);

/* 8. main */

int main (int argc, char *argv[])
{
	clues_t clue = {GRID3X3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	get_parameters(argc, argv, &clue);

	if (find_solution(clue) == FOUND)
	       	return(EXIT_SUCCESS);

	return (EXIT_FAILURE);
}


/* 9. function definitions */

int find_solution(const clues_t clue)
{
	for (int i= 0; i < suko_count; i++) {
		if (is_suko(clue, suko_grids[i])) {
			write_answer(suko_grids[i], clue.gridwidth);
			return(FOUND);
		}
	}
	return(NOTFOUND);
}


int  is_suko(const clues_t clue, const int8_t grid[])
{
	return (
		clue.tl_sum == grid[0] + grid[1] + grid[3] + grid[4] &&
		clue.tr_sum == grid[1] + grid[2] + grid[4] + grid[5] &&
		clue.bl_sum == grid[3] + grid[4] + grid[6] + grid[7] &&
		clue.br_sum == grid[4] + grid[5] + grid[7] + grid[8] &&

		clue.color1 == grid[clue.c1box1] + grid[clue.c1box2] + grid[clue.c1box3] + grid[clue.c1box4] &&
		clue.color2 == grid[clue.c2box1] + grid[clue.c2box2] + grid[clue.c2box3] &&
		clue.color3 == grid[clue.c3box1] + grid[clue.c3box2] 
		);
}


void write_answer(const int8_t grid[], int width)
{
	width = (width == GRID3X3 || width == GRIDLINE ? width : GRID3X3);

	for (int i= 0; i < SUKOSIZE; i++) {
		/*
		 * Setting 'width' to 3 prints a 3x3 grid.  Setting it
		 * to SUKOSIZE prints results on a single line.
		 */
		printf("%d%c", (int) grid[i], (i+1)%width ? ' ' : '\n');
	}
}
