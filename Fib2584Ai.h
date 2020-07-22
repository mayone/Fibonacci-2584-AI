#ifndef __FIB2584AI_H__
#define __FIB2584AI_H__

#include <cstdlib>
#include <ctime>
#include <string>
#include <stack>
#include <map>
#include "Fib2584/MoveDirection.h"
#include "Fib2584/AiBoard.h"

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
private:
	// Random
	class Random
	{
	public:
		Random() {}
		void initialize()
		{
			srand(time(NULL));
		}
		MoveDirection generateMove()
		{
			MoveDirection randomMove = static_cast<MoveDirection>(rand() % 4);
			return randomMove;
		}
	private:
	};
	Random random;

	// Heuristic
	class Heuristic
	{
	public:
		Heuristic();
		void initialize(bool debugMode = false);
		MoveDirection generateMove(int board[4][4]);
		int numberOfVerticalMergables(int board[4][4], int &max_tile_up, int &max_tile_down);
		int numberOfHorizontalMergables(int board[4][4], int &max_tile_left, int &max_tile_right);
		int valueOfMaxTile(int board[4][4], int max_tile_pos[2]);
		void createFibMap();
	private:
		enum
		{
			VERTICAL,
			HORIZONTAL
		};
		int board_prev[4][4];
		int prev_move;
		bool debugMode;
		std::map<int, int> fib_map;
	};
	Heuristic heuristic;

	// Temporal Difference Learning
	class TDLearning
	{
	public:
		TDLearning();
		~TDLearning();
		void initialize(bool learningMode = true, int _alpha = 4096, std::string fileName = std::string("weight.dat"));
		MoveDirection generateMove(int board[4][4]);
		void gameOver(int board[4][4]);

		BitBoard inverseTuple(BitBoard tuple);
		int _alpha;	// reciprocal of alpha
	private:
		class BoardTuples {
		public:
			BoardTuples();
			BoardTuples(AiBoard &board, int *featureWeight_4TupleInner, int *featureWeight_4TupleOuter, int reward);
			int evaluateBoard();
			int *featureWeight_4TupleInner;
			int *featureWeight_4TupleOuter;
			BitBoard inner[4];
			BitBoard outer[4];
			int reward;
			enum {
				UPPER,
				RIGHT,
				DOWN,
				LEFT
			};
		};
		int *featureWeight_4TupleInner;
		int *featureWeight_4TupleOuter;
		std::string fileName;
		bool learningMode;
		int numFeatures;
		std::stack<BoardTuples> boardHistory;
	};
	TDLearning tdlearning;
};

void displayInfo(int board[4][4], int move_score[4], MoveDirection move);
void displayBoard(int board[4][4]);
void displayScore(int move_score[4]);
void displayMove(MoveDirection move);

bool boardcmp(int a[4][4], int b[4][4]);

#endif