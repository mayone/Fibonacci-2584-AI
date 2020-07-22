#include "Fib2584Ai.h"

Fib2584Ai::Fib2584Ai()
{}

void Fib2584Ai::initialize(int argc, char* argv[])
{
	//random.initialize();
	//heuristic.initialize();
	tdlearning.initialize(false);

	return;
}

MoveDirection Fib2584Ai::generateMove(int board[4][4])
{
	//return random.generateMove();
	//return heuristic.generateMove(board);
	return tdlearning.generateMove(board);
}

void Fib2584Ai::gameOver(int board[4][4], int iScore)
{
	tdlearning.gameOver(board);
	return;
}

void displayInfo(int board[4][4], int move_score[4], MoveDirection move)
{
	displayBoard(board);
	displayScore(move_score);
	displayMove(move);
	printf("-------------------------\n");
}

void displayBoard(int board[4][4])
{
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			printf("%d\t", board[i][j]);
		}
		printf("\n");
	}
}

void displayScore(int move_score[4])
{
	printf("move_score UP = %d\n", move_score[MOVE_UP]);
	printf("move_score RIGHT = %d\n", move_score[MOVE_RIGHT]);
	printf("move_score DOWN = %d\n", move_score[MOVE_DOWN]);
	printf("move_score LEFT = %d\n", move_score[MOVE_LEFT]);
}

void displayMove(MoveDirection move)
{
	printf("decided move: ");
	switch (move) {
		case MOVE_UP:
			printf("UP\n");
			break;
		case MOVE_RIGHT:
			printf("RIGHT\n");
			break;
		case MOVE_DOWN:
			printf("DOWN\n");
			break;
		case MOVE_LEFT:
			printf("LEFT\n");
			break;
		default:
			break;
	}
}

bool boardcmp(int a[4][4], int b[4][4])
{
	bool notEqual = 0;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if (a[i][j] != b[i][j]) {
				notEqual = 1;
			}
		}
	}

	return notEqual;
}