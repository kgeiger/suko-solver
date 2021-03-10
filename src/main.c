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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "suko.h"
#include "grids.h"
#include "parameters.h"

static const int8_t suko_grids[][SUKOSIZE] = _GRID_PERMUTATIONS; 
static const int    suko_count = sizeof(suko_grids)/sizeof(suko_grids[0]);

static int  is_suko(const struct clue p, const int8_t grid[]);
static void write_answer(const int8_t grid[], int width);

int main (int argc, char *argv[])
{
	int i;
	int found = false;
	
	struct clue parms = _CLUES_INIT;

	get_parameters(argc, argv, &parms);

	for (i = 0;  i < suko_count;  i++) {
		if (is_suko(parms, suko_grids[i])) {
			write_answer(suko_grids[i], parms.gridwidth);
			found = true;
		}
	}

	return found ? EXIT_SUCCESS : EXIT_FAILURE;
}


static int circle_test(const struct clue p, const int8_t grid[])
{
	return p.tl == grid[0] + grid[1] + grid[3] + grid[4] &&
		p.tr == grid[1] + grid[2] + grid[4] + grid[5] &&
		p.bl == grid[3] + grid[4] + grid[6] + grid[7] &&
		p.br == grid[4] + grid[5] + grid[7] + grid[8]; 
}

static int color_test(const struct color c, const int8_t grid[])
{
	int i;
	int sum = 0;
 
	for (i = 0;  i < SUKOSIZE && -1 < c.box[i];  i++)
	      sum += grid[c.box[i]];

	return c.sum == sum;
}

static int is_suko(const struct clue p, const int8_t grid[])
{
	/*
	 * If the user specified flag -M, omit the color{a..c} tests.
	 * The user wants to see all suko grids that match the four
	 * circle values.
	 */
	if (p.matrixonly)
		return circle_test(p, grid);
	else
		return circle_test(p, grid) &&
			color_test(p.a, grid) &&
			color_test(p.b, grid) &&
			color_test(p.c, grid);
}

static void write_answer(const int8_t grid[], int width)
{
	width = (width == GRID3X3 || width == GRIDLINE ? width : GRID3X3);

	for (int i= 0; i < SUKOSIZE; i++)
		printf("%d%c", (int) grid[i], (i+1)%width ? ' ' : '\n');
}
