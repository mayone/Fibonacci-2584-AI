#ifndef __AIBOARD_H__
#define __AIBOARD_H__

#include <iostream>
#include <map>
#include "MoveDirection.h"
#include "MoveTable.h"
#include "BitBoard.h"
#include "Random.h"

using namespace std;

class AiBoard
{
public:
	static const int fibonacci_[32];
	std::map<int, int> inverseFib;
	void createInverseFib();
private:
	static Random random_;
public:
	AiBoard();
	AiBoard(int board[4][4]);
	void initialize();
	int move(MoveDirection moveDirection);
	void addRandomTile();
	bool terminated();
	void getArrayBoard(int board[4][4]);
	int getMaxTile();
	void showBoard();
	bool operator==(AiBoard aiBoard);
	BitBoard getRow(int row);
	BitBoard getColumn(int column);
private:
	BitBoard restoreRow(BitBoard rowBits, int row);
	BitBoard restoreColumn(BitBoard columnBits, int column);
	int countEmptyTile();
	int getTile(int row, int column);
	int getFibonacci(int index);
private:
	BitBoard board_;
};

#endif