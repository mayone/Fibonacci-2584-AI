#include "Fib2584Ai.h"
#include <fstream>
#include <limits>

using namespace std;

Fib2584Ai::TDLearning::TDLearning()
{
	numFeatures = (1 << (5 * 4));	// 32 * 32 * 32 * 32
	featureWeight_4TupleInner = new int[numFeatures];
	featureWeight_4TupleOuter = new int[numFeatures];
}

Fib2584Ai::TDLearning::~TDLearning()
{
	if (learningMode) {
		ofstream fout(fileName.c_str(), ifstream::out);
		fout.write((char *)featureWeight_4TupleInner, numFeatures * sizeof(int));
		fout.write((char *)featureWeight_4TupleOuter, numFeatures * sizeof(int));
	}

	delete [] featureWeight_4TupleInner;
	delete [] featureWeight_4TupleOuter;
}

void Fib2584Ai::TDLearning::initialize(bool learningMode, int _alpha, std::string fileName)
{
	this->fileName = fileName;
	this->learningMode = learningMode;
	this->_alpha = _alpha;

	ifstream fin(fileName.c_str(), ifstream::in);
	fin.read(reinterpret_cast<char *>(featureWeight_4TupleInner), numFeatures * sizeof(int));
	fin.read(reinterpret_cast<char *>(featureWeight_4TupleOuter), numFeatures * sizeof(int));
}

MoveDirection Fib2584Ai::TDLearning::generateMove(int board[4][4])
{
	AiBoard currBoard(board);
	MoveDirection optimalMove;
	BoardTuples optimalBoard;
	int reward;
	int value;
	int score;
	int maxScore = numeric_limits<int>::min();

	for(int dir = 0; dir < 4; dir++) {
		AiBoard nextBoard(board);
		reward = nextBoard.move(static_cast<MoveDirection>(dir));
		if (nextBoard == currBoard)
			continue;

		BoardTuples boardTuples(nextBoard, featureWeight_4TupleInner, featureWeight_4TupleOuter, reward);
		value = boardTuples.evaluateBoard();

		score = reward + value;
		if (maxScore < score) {
			maxScore = score;
			optimalMove = static_cast<MoveDirection>(dir);
			optimalBoard = boardTuples;
		}
	}

	if (learningMode) {
		boardHistory.push(optimalBoard);
	}

	return optimalMove;
}

void Fib2584Ai::TDLearning::gameOver(int board[4][4])
{
	int delta;
	BoardTuples nextBoard;

	if (learningMode) {
		AiBoard endBoard(board);
		BoardTuples currBoard(endBoard, featureWeight_4TupleInner, featureWeight_4TupleOuter, 0);
		delta = 0-currBoard.evaluateBoard();

		for(int i = 0; i < 4; i++) {
			featureWeight_4TupleInner[currBoard.inner[i]] += delta / _alpha;
			featureWeight_4TupleInner[inverseTuple(currBoard.inner[i])] += delta / _alpha;
			featureWeight_4TupleOuter[currBoard.outer[i]] += delta / _alpha;
			featureWeight_4TupleOuter[inverseTuple(currBoard.outer[i])] += delta / _alpha;
		}
		nextBoard = currBoard;

		while(!boardHistory.empty()) {
			currBoard = boardHistory.top();
			boardHistory.pop();

			delta = nextBoard.reward +
					nextBoard.evaluateBoard() - currBoard.evaluateBoard();

			for(int i = 0; i < 4; i++) {
				featureWeight_4TupleInner[currBoard.inner[i]] += delta / _alpha;
				featureWeight_4TupleInner[inverseTuple(currBoard.inner[i])] += delta / _alpha;
				featureWeight_4TupleOuter[currBoard.outer[i]] += delta / _alpha;
				featureWeight_4TupleOuter[inverseTuple(currBoard.outer[i])] += delta / _alpha;
			}

			nextBoard = currBoard;
		}
	}
}

BitBoard Fib2584Ai::TDLearning::inverseTuple(BitBoard tuple)
{
	BitBoard _tuple = 0;

	for(int i = 0; i < 4; i++) {
		_tuple = (_tuple << 5) | (tuple & 0x1f);
		tuple >>= 5;
	}
	
	return _tuple;
}

Fib2584Ai::TDLearning::BoardTuples::BoardTuples()
{}

Fib2584Ai::TDLearning::BoardTuples::BoardTuples(AiBoard &board, int *featureWeight_4TupleInner, int *featureWeight_4TupleOuter, int reward)
: featureWeight_4TupleInner(featureWeight_4TupleInner), featureWeight_4TupleOuter(featureWeight_4TupleOuter), reward(reward)
{
	inner[UPPER] = board.getRow(1);
	inner[RIGHT] = board.getColumn(2);
	inner[DOWN] = board.getRow(2);
	inner[LEFT] = board.getColumn(1);

	outer[UPPER] = board.getRow(0);
	outer[RIGHT] = board.getColumn(3);
	outer[DOWN] = board.getRow(3);
	outer[LEFT] = board.getColumn(0);
}

int Fib2584Ai::TDLearning::BoardTuples::evaluateBoard()
{
	int value = 0;

	for(int i = 0; i < 4; i++) {
		value += featureWeight_4TupleInner[inner[i]];
	}
	for(int i = 0; i < 4; i++) {
		value += featureWeight_4TupleOuter[outer[i]];
	}

	return value;
}