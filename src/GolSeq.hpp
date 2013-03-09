/* Exercise: [MGS] EPR: Game Of Life; with focus on performance
* Author: David Pertiller
* Date: 21.12.2012
* Version: 2, added OpenMP and OpenCL
*/

#include <vector>
#include "Change.hpp"
using namespace std;

//the computation method will iterate over the data for the given number of generations
static inline void ComputeSeq(vector<vector<int>> &data, const int generations, const int &width, const int height)
{	
	//pre-calculate field indexes
	static const int lastRow = height - 1; //this avoids the computing expensive modulo which should only be called if necessary
	static const int secondLastRow = lastRow - 1; //this would crash if the height of the board would be below 2, but than GOF would not be possible anyway ;)
	static const int lastColumn = width - 1;
	static const int secondLastColumn = lastColumn - 1;
	
	//calculate generation and use result for calculation of next generation and so on
	static int changeLimit = int(height*width / 2.2);
	//per iteration, the next state of a cell is computed.
	//this is where the computed data is stored. It's the starting point for the next iteration
	vector<Change> changeList(changeLimit);

	for(int i = 0; i < changeLimit; i++)
	{
		changeList[i] = Change(NULL, 2);
	}
	
	//calculate generation and use result for calculation of next generation and so on
	for (int gen = 0; gen < generations; ++gen)
	{
		int changeCount = 0;
		/*

		The edges of the matrix are joined, i.e. accessing boardData[-1,0] is actually the last element of the row:
		For this, a modulo operation is needed or if statements for when the index is out of bounds		
		int left = mod(x-1, width); //if x positive then left = x, or if negative then width is added
		int right = mod(x+1, width);
		int top = mod(y-1, height);
		int bottom = mod(y+1, height);

		To avoid this and gain maximum performance, the formerly generic calculation is split into multiple parts:

		x x x x x
		x . . . x
		x . . . x
		x x x x x

		The calculation of the neighbours for a cell in the inner Ring of the board (labelled with .) is straightforward, as no modulo calculation is needed, because no edges need to be joined		
		The calculation of the neighbours for a cell in the outer Ring of the board (labelled with x) needs modulo calculation or out of bounds handling, as the ends of the matrix should appear to be joined together

		For maximum performance with no modulo or if statements, the outer ring is splitted into:
		- calculating first row (without corners)
		- calculating last row (without corners)
		- calculating first column (without corners)
		- calculating last column (without corners)
		- calculating top left corner
		- calculating top right corner
		- calculating bottom left corner
		- calculating bottom right corner

		*/

#pragma region Calculation of the inner ring
		//calculate the next state for every cell in the inner ring of the board
		for (int y = 1; y < lastRow; ++y) //starting at 1 and stopping before the end avoids edges and corners
		{
			int top = y-1;
			int bottom = y+1;
			int left = 0; //in the inner ring, we can be sure that the index isn't out of bounds without checking the borders
			int right = 2; //start position of the right neighbour

			for (int x = 1; x < lastColumn; ++x) //starting at 1 and stopping before the end avoids edges and corners
			{
				//calculate the neighbourhood for each cell:
				//the region that determines the neighbourhood of a cell is a 3x3 Matrix
				//the ends of the boardData are joined (right corner + 1 = left corner, top corner -1 = bottom corner, etc.)


				//what we want to know is how many cells in the neighborhood are alive
				//for every surrounding cell, check if cell is 'alive'
				int alive = data[y][left]
						  + data[y][right]
						  + data[top][left]
						  + data[top][x]
						  + data[top][right]
						  + data[bottom][left]
						  + data[bottom][x]
						  + data[bottom][right];

				++left; //increment left
				++right; //increment right

				int *cell = &(data[y][x]); //get pointer to current cell

				if (*cell == 1 && (alive > 3 || alive < 2)) //a live cell with more than 3 or less than 2 neighbours dies
				{
					changeList[changeCount].pos = cell;
					changeList[changeCount].value = 0;
					++changeCount;
				}
				else if (*cell == 0 && (alive == 3)) //a dead cell with exactly 3 live neighbours becomes alive
				{
					changeList[changeCount].pos = cell;
					changeList[changeCount].value = 1;
					++changeCount;
				}


			}

			//visualisation of this:
			/*
			char neighbour[3][3];

			//data's structure: rows (height), each having a vector with size of width
			neighbour[1][0] = data[y][left];
			neighbour[1][1] = data[y][x];
			neighbour[1][2] = data[y][right];

			neighbour[0][0] = data[top][left];
			neighbour[0][1] = data[top][x];
			neighbour[0][2] = data[top][right];

			neighbour[2][0] = data[bottom][left];
			neighbour[2][1] = data[bottom][x];
			neighbour[2][2] = data[bottom][right];

			int dead = 0;
			int alive = 0;

			for (int cell = 0; cell < 9; cell++)
			{
			if (*neighbour[cell] == 0)
			{
			++dead;
			}
			else
			{
			++alive;
			}
			}
			*/

		}
#pragma endregion

#pragma region calculating first row for y = 0 (without corners)
		{
			for (int x = 1; x < lastColumn; ++x) //calculating row for y = 0 (without corners)
			{
				int left = x-1; //this avoids the relatively expensive modulo computation (or if statement)
				int right = x+1; //this avoids the relatively expensive modulo computation (or if statement)

				int alive = data[0][left]
						  + data[0][right]
						  + data[lastRow][left]
						  + data[lastRow][x]
						  + data[lastRow][right]
						  + data[1][left]
						  + data[1][x]
						  + data[1][right];

				int *cell = &(data[0][x]);

				if (*cell == 1 && (alive > 3 || alive < 2)) //a live cell with more than 3 or less than 2 neighbours dies
				{
					changeList[changeCount].pos = cell;
					changeList[changeCount].value = 0;
					++changeCount;
				}
				else if (*cell == 0 && (alive == 3)) //a dead cell with exactly 3 live neighbours becomes alive
				{
					changeList[changeCount].pos = cell;
					changeList[changeCount].value = 1;
					++changeCount;
				}
			}
		}
#pragma endregion

#pragma region calculating last row (height-1) (without corners)
		{
			for (int x = 1; x < lastColumn; ++x) //calculating row for y = 0 (without corners)
			{
				int left = x-1;
				int right = x+1;
				//int bottom = 0; //we can tell that the lower neighbour of the bottom edge is the first row

				int alive = data[lastRow][left]
						  + data[lastRow][right]
						  + data[secondLastRow][left]
						  + data[secondLastRow][x]
						  + data[secondLastRow][right]
						  + data[0][left]
						  + data[0][x]
						  + data[0][right];

				int *cell = &(data[lastRow][x]); //get pointer to current cell

				if (*cell == 1 && (alive > 3 || alive < 2)) //a live cell with more than 3 or less than 2 neighbours dies
				{
					changeList[changeCount].pos = cell;
					changeList[changeCount].value = 0;
					++changeCount;
				}
				else if (*cell == 0 && (alive == 3)) //a dead cell with exactly 3 live neighbours becomes alive
				{
					changeList[changeCount].pos = cell;
					changeList[changeCount].value = 1;
					++changeCount;
				}
			}
		}
#pragma endregion

#pragma region calculating first column (x = 0) (without corners)
		{		
			for (int y = 1; y < lastRow; ++y) //calculating row for y = 0 (without corners)
			{

				//int right = 1; //we can tell that the right column has the index 1
				int top = y-1; //this avoids the relatively expensive modulo computation (or if statement)
				int bottom = y+1;

				int alive = data[y][lastColumn]
						  + data[y][1]
						  + data[top][lastColumn]
						  + data[top][0]
						  + data[top][1]
						  + data[bottom][lastColumn]
						  + data[bottom][0]
						  + data[bottom][1];

				int *cell = &(data[y][0]); //get pointer to current cell

				if (*cell == 1 && (alive > 3 || alive < 2)) //a live cell with more than 3 or less than 2 neighbours dies
				{
					changeList[changeCount].pos = cell;
					changeList[changeCount].value = 0;
					++changeCount;
				}
				else if (*cell == 0 && (alive == 3)) //a dead cell with exactly 3 live neighbours becomes alive
				{
					changeList[changeCount].pos = cell;
					changeList[changeCount].value = 1;
					++changeCount;
				}
			}
		}
#pragma endregion

#pragma region calculating last column (x = width - 1) (without corners)
		{
			for (int y = 1; y < lastRow; ++y) //calculating row for y = 0 (without corners)
			{
				//int right = 0;
				int top = y-1; //this is the computing expense modulo which should only be called if necessary
				int bottom = y+1;

				int alive = data[y][secondLastColumn]
						  + data[y][0]
						  + data[top][secondLastColumn]
						  + data[top][lastColumn]
						  + data[top][0]
						  + data[bottom][secondLastColumn]
						  + data[bottom][lastColumn]
						  + data[bottom][0];

				int *cell = &(data[y][lastColumn]); //get pointer to current cell

				if (*cell == 1 && (alive > 3 || alive < 2)) //a live cell with more than 3 or less than 2 neighbours dies
				{
					changeList[changeCount].pos = cell;
					changeList[changeCount].value = 0;
					++changeCount;
				}
				else if (*cell == 0 && (alive == 3)) //a dead cell with exactly 3 live neighbours becomes alive
				{
					changeList[changeCount].pos = cell;
					changeList[changeCount].value = 1;
					++changeCount;
				}
			}
		}
#pragma endregion

#pragma region calculating top left corner
		{
			//calculating top left corner
			//int bottom = 1;

			int alive = data[0][lastColumn]
					  + data[0][1]
					  + data[lastRow][lastColumn]
					  + data[lastRow][0]
					  + data[lastRow][1]
					  + data[1][lastColumn]
					  + data[1][0]
					  + data[1][1];

			int *cell = &(data[0][0]); //get pointer to current cell

			if (*cell == 1 && (alive > 3 || alive < 2)) //a live cell with more than 3 or less than 2 neighbours dies
			{
				changeList[changeCount].pos = cell;
				changeList[changeCount].value = 0;
				++changeCount;
			}
			else if (*cell == 0 && (alive == 3)) //a dead cell with exactly 3 live neighbours becomes alive
			{
				changeList[changeCount].pos = cell;
				changeList[changeCount].value = 1;
				++changeCount;
			}
		}
#pragma endregion

#pragma region calculating top right corner
		{
			//int bottom = 1;

			int alive = data[0][secondLastColumn]
			+ data[0][0]
			+ data[lastRow][secondLastColumn]
			+ data[lastRow][lastColumn]
			+ data[lastRow][0]
			+ data[1][secondLastColumn]
			+ data[1][lastColumn]
			+ data[1][0];

			int *cell = &(data[0][lastColumn]); //get pointer to current cell

			if (*cell == 1 && (alive > 3 || alive < 2)) //a live cell with more than 3 or less than 2 neighbours dies
			{
				changeList[changeCount].pos = cell;
				changeList[changeCount].value = 0;
				++changeCount;
			}
			else if (*cell == 0 && (alive == 3)) //a dead cell with exactly 3 live neighbours becomes alive
			{
				changeList[changeCount].pos = cell;
				changeList[changeCount].value = 1;
				++changeCount;
			}
		}
#pragma endregion

#pragma region calculating bottom left corner
		{
			//int bottom = 0;

			int alive = data[lastRow][lastColumn]
			+ data[lastRow][1]
			+ data[secondLastRow][lastColumn]
			+ data[secondLastRow][0]
			+ data[secondLastRow][1]
			+ data[0][lastColumn]
			+ data[0][0]
			+ data[0][1];

			int *cell = &(data[lastRow][0]); //get pointer to current cell

			if (*cell == 1 && (alive > 3 || alive < 2)) //a live cell with more than 3 or less than 2 neighbours dies
			{
				changeList[changeCount].pos = cell;
				changeList[changeCount].value = 0;
				++changeCount;
			}
			else if (*cell == 0 && (alive == 3)) //a dead cell with exactly 3 live neighbours becomes alive
			{
				changeList[changeCount].pos = cell;
				changeList[changeCount].value = 1;
				++changeCount;
			}
		}
#pragma endregion

#pragma region calculating bottom right corner
		{
			int alive = data[lastRow][secondLastColumn]
			+ data[lastRow][0]
			+ data[secondLastRow][secondLastColumn]
			+ data[secondLastRow][lastColumn]
			+ data[secondLastRow][0]
			+ data[0][secondLastColumn]
			+ data[0][lastColumn]
			+ data[0][0];

			int *cell = &(data[lastRow][lastColumn]); //get pointer to current cell

			if (*cell == 1 && (alive > 3 || alive < 2)) //a live cell with more than 3 or less than 2 neighbours dies
			{
				changeList[changeCount].pos = cell;
				changeList[changeCount].value = 0;
				++changeCount;
			}
			else if (*cell == 0 && (alive == 3)) //a dead cell with exactly 3 live neighbours becomes alive
			{
				changeList[changeCount].pos = cell;
				changeList[changeCount].value = 1;
				++changeCount;
			}
		}
#pragma endregion


		//with the changes of the changeList we got the starting point of the next iteration
		for (int i = 0; i < changeCount; i++)
		{
			Change change = changeList[i];
			*(change.pos) = change.value;
		}
	}
}
