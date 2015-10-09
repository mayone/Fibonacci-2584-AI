#ifndef __FIB2584AI_H__
#define __FIB2584AI_H__

#include <cstdlib>
#include <ctime>
#include <map>
#include "Fib2584/MoveDirection.h"

class Fib2584Ai
{
public:
	Fib2584Ai();
	// initialize ai
	void initialize(int argc, char* argv[]);
	// generate one move
	MoveDirection generateMove(int board[4][4]);
	// do some action when game over
	void gameOver(int board[4][4], int iScore);

	/**********************************
	You can implement any additional functions
	or define any variables you may need.
	**********************************/
	int numberOfVerticalMergables(int board[4][4], int &max_tile_up, int &max_tile_down);
	int numberOfHorizontalMergables(int board[4][4], int &max_tile_left, int &max_tile_right);
	void getLocalDensity(int board[4][4], int local_density[4]);
private:
	std::map<int, int> fib_map;
	int board_prev[4][4];
	int prev_move;
};

enum
{
	VERTICAL, HORIZONTAL
};

enum
{
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
};

void generateFibMap(std::map<int, int> &fib_map);

void displayBoard(int board[4][4]);

bool boardcmp(int a[4][4], int b[4][4]);

#endif