/***********************************************************************
*
* Program:		CS 4000 Assignment 2 Part 3:
* 				'Parallel For' Circle The Word Solver
*
* Author:		Alexander Bagnall
* Email:		ab667712@ohio.edu
*
* Description:	Implementation file for the CircleTheWordSolver class.
*
* Date:			February 11, 2016
*
***********************************************************************/

#include "CircleTheWordSolver.h"
#include <climits>

#ifdef _OPENMP
#include <omp.h>
#endif

using namespace std;

// PUBLIC

vector<pair<int, int> > CircleTheWordSolver::word_locations(vector<vector<char> > &puzzle, vector<string> &wordList)
{
	// initialize all locations to <INT_MAX, INT_MAX>
	vector<pair<int, int> > locations;
	for (int i = 0; i < wordList.size(); ++i)
	{
		locations.push_back(pair<int, int>(INT_MAX, INT_MAX));
	}
	
	// for each row
	#pragma omp parallel for num_threads(4)
	for (int x = 0; x < puzzle.size(); ++x)
	{
		// for each column
		for (int y = 0; y < puzzle[x].size(); ++y)
		{
			// for each word
			for (int i = 0; i < wordList.size(); ++i)
			{
				// word at location?
				if (word_at_puzzle_location(puzzle, x, y, wordList[i]))
				{
					// update location of word if it is better than existing location
					#pragma omp critical
					if (x < locations[i].first || (x == locations[i].first && y < locations[i].second))
						locations[i] = pair<int, int>(x, y);
				}
			}
		}
	}
	
	// replace any INT_MAX with -1 to conform to the specification
	for (int i = 0; i < locations.size(); ++i)
	{
		if (locations[i].first == INT_MAX)
			locations[i] = pair<int, int>(-1, -1);
	}
	
	return locations;
}

// PRIVATE

bool CircleTheWordSolver::word_at_puzzle_location(const vector<vector<char> > &puzzle, int loc_x, int loc_y, const string &word)
{
	// return false if the puzzle if empty or word is empty string
	if (!puzzle.size() || !word.length())
		return false;
	
	// scan in all directions
	return scan(puzzle, loc_x, loc_y, -1, 0, word)
		|| scan(puzzle, loc_x, loc_y, 1, 0, word)
		|| scan(puzzle, loc_x, loc_y, 0, -1, word)
		|| scan(puzzle, loc_x, loc_y, 0, 1, word)
		|| scan(puzzle, loc_x, loc_y, -1, -1, word)
		|| scan(puzzle, loc_x, loc_y, -1, 1, word)
		|| scan(puzzle, loc_x, loc_y, 1, 1, word)
		|| scan(puzzle, loc_x, loc_y, 1, -1, word);
}

bool CircleTheWordSolver::scan(const vector<vector<char> > &puzzle, int start_x, int start_y, int delta_x, int delta_y, const string &word)
{
	// quit early if we would go off edge of puzzle
	int end_x = start_x + delta_x * word.size();
	int end_y = start_y + delta_y * word.size();
	if (end_x < 0 || end_x > puzzle.size() || end_y < 0 || end_y > puzzle[0].size())
		return false;
	
	// compare each character in the word with the character in the
	// puzzle given by the initial position + delta position * index
	for (int i = 0; i < word.length(); ++i)
	{
		int x = start_x + delta_x * i;
		int y = start_y + delta_y * i;
		
		if (puzzle[x][y] != word[i])
			return false;
	}
	
	// if we made it this far, the word has been found
	return true;
}
