#include "Fib2584Ai.h"

Fib2584Ai::Fib2584Ai()
{}

void Fib2584Ai::initialize(int argc, char* argv[])
{
	//srand(time(NULL));
	heuristic.initialize();

	return;
}

MoveDirection Fib2584Ai::generateMove(int board[4][4])
{
	return heuristic.generateMove(board);
	//MoveDirection randomMove = static_cast<MoveDirection>(rand() % 4);
	//return randomMove;
}

void Fib2584Ai::gameOver(int board[4][4], int iScore)
{
	return;
}

/**********************************
You can implement any additional functions
you may need.
**********************************/
Fib2584Ai::Heuristic::Heuristic()
{}

void Fib2584Ai::Heuristic::initialize()
{
	createFibMap();
}

MoveDirection Fib2584Ai::Heuristic::generateMove(int board[4][4])
{
	int max_tile;
	int max_tile_pos[2];

	int num_mergables[2];
	int max_tile_gen[4];

	int move_score[4];
	int max_move_score;
	int optimal_move;
	MoveDirection move;

	bool boardHasChanged = boardcmp(board_prev, board);

	// Find optimal move
	if (!boardHasChanged) {
		optimal_move = (prev_move + 1) % 4;
	}
	else {
		// Get number of mergable tiles vertical/horizontal and the max value of generated tile
		num_mergables[VERTICAL] = numberOfVerticalMergables(board, max_tile_gen[MOVE_UP], max_tile_gen[MOVE_DOWN]);
		num_mergables[HORIZONTAL] = numberOfHorizontalMergables(board, max_tile_gen[MOVE_LEFT], max_tile_gen[MOVE_RIGHT]);

		// Find the value of max tile and it's location
		max_tile = valueOfMaxTile(board, max_tile_pos);

		// Evaluate scores of each direction
		move_score[MOVE_UP] = num_mergables[VERTICAL] + fib_map[max_tile_gen[MOVE_UP]]/2;
		move_score[MOVE_RIGHT] = num_mergables[HORIZONTAL] + fib_map[max_tile_gen[MOVE_RIGHT]]/2;
		move_score[MOVE_DOWN] = num_mergables[VERTICAL] + fib_map[max_tile_gen[MOVE_DOWN]]/2;
		move_score[MOVE_LEFT] = num_mergables[HORIZONTAL] + fib_map[max_tile_gen[MOVE_LEFT]]/2;

		// Top-right corner first
		move_score[MOVE_UP] += 1;
		move_score[MOVE_RIGHT] += 1;

		// Adjust the priority according to situation
		if (prev_move == MOVE_DOWN || prev_move == MOVE_RIGHT) {
			move_score[MOVE_UP] += 1;
		}
		if (prev_move == MOVE_LEFT || prev_move == MOVE_UP) {
			move_score[MOVE_RIGHT] += 1;
		}
		if (abs(fib_map[board[0][2]] - fib_map[board[0][1]]) > 2 &&
			abs(fib_map[board[0][2]] - fib_map[board[0][3]]) > 2) {
			move_score[MOVE_DOWN] += 2;
		}
		if (max_tile_pos[1] == 3 && board[0][3] == 0) {
			move_score[MOVE_UP] += 5;
		}
		if (max_tile_pos[0] == 0 && board[0][3] == 0) {
			move_score[MOVE_RIGHT] += 5;
		}

		optimal_move = MOVE_UP;
		max_move_score = move_score[MOVE_UP];
		for(int i = 1; i < 4; i++) {
			if (max_move_score < move_score[i]) {
				max_move_score = move_score[i];
				optimal_move = i;
			}
		}
	}
	prev_move = optimal_move;
	move = static_cast<MoveDirection>(optimal_move);

	if (boardHasChanged) {
		//displayInfo(board, move_score, move);
	}

	return move;
}

void Fib2584Ai::Heuristic::createFibMap()
{
	fib_map[0] = 0;
	fib_map[1] = 1;
	fib_map[2] = 2;
	fib_map[3] = 3;
	fib_map[5] = 4;
	fib_map[8] = 5;
	fib_map[13] = 6;
	fib_map[21] = 7;
	fib_map[34] = 8;
	fib_map[55] = 9;
	fib_map[89] = 10;
	fib_map[144] = 11;
	fib_map[233] = 12;
	fib_map[377] = 13;
	fib_map[610] = 14;
	fib_map[987] = 15;
	fib_map[1597] = 16;
	fib_map[2584] = 17;
	fib_map[4181] = 18;
	fib_map[6765] = 19;
	fib_map[10946] = 20;
	fib_map[17711] = 21;
	fib_map[28657] = 22;
	fib_map[46368] = 23;
	fib_map[75025] = 24;
	fib_map[121393] = 25;
	fib_map[196418] = 26;
	fib_map[317811] = 27;
	fib_map[514229] = 28;
	fib_map[832040] = 29;
	fib_map[1346269] = 30;
	fib_map[2178309] = 31;
}

int Fib2584Ai::Heuristic::numberOfVerticalMergables(
	int board[4][4],
	int &max_tile_gen_up,
	int &max_tile_gen_down)
{
	int num_mergables_vertical = 0;

	max_tile_gen_up = 0;
	max_tile_gen_down = 0;

	// For each columns
	for(int j = 0; j < 4; j++) {
		// Move up and find the number of mergables
		for(int i = 0; i < 3;) {
			if (board[i][j] == 0) {
				i++;
			}
			else {
				int k;
				for(k = i+1; k < 4;) {
					if (board[k][j] == 0) {
						k++;
					}
					else {
						if (abs(fib_map[board[i][j]] - fib_map[board[k][j]]) == 1 ||
							(board[i][j] == 1 && board[k][j] == 1))	// Mergable
						{
							num_mergables_vertical++;
							if (max_tile_gen_up < board[i][j] + board[k][j]) {
								max_tile_gen_up = board[i][j] + board[k][j];
							}
							i = k+1;
							break;
						}
						else {
							i = k;
							break;
						}
					}
				}
				if (k == 4)
					break;
			}
		}
		// Move down
		for(int i = 3; i > 0;) {
			if (board[i][j] == 0) {
				i--;
			}
			else {
				int k;
				for(k = i-1; k >= 0;) {
					if (board[k][j] == 0) {
						k--;
					}
					else {
						if (abs(fib_map[board[i][j]] - fib_map[board[k][j]]) == 1 ||
							(board[i][j] == 1 && board[k][j] == 1))
						{
							if (max_tile_gen_down < board[i][j] + board[k][j]) {
								max_tile_gen_down = board[i][j] + board[k][j];
							}
							i = k-1;
							break;
						}
						else {
							i = k;
							break;
						}
					}
				}
				if (k < 0)
					break;
			}
		}
	}

	return num_mergables_vertical;
}

int Fib2584Ai::Heuristic::numberOfHorizontalMergables(
	int board[4][4],
	int &max_tile_left,
	int &max_tile_right)
{
	int num_mergables_horizontal = 0;

	max_tile_left = 0;
	max_tile_right = 0;

	// For each row
	for(int i = 0; i < 4; i++) {
		// Move left and find the number of mergables
		for(int j = 0; j < 3;) {
			if (board[i][j] == 0) {
				j++;
			}
			else {
				int k;
				for(k = j+1; k < 4;) {
					if (board[i][k] == 0) {
						k++;
					}
					else {
						if (abs(fib_map[board[i][j]] - fib_map[board[i][k]]) == 1 ||
							(board[i][j] == 1 && board[i][k] == 1))	// Mergable
						{
							num_mergables_horizontal++;
							if (max_tile_left < board[i][j] + board[i][k]) {
								max_tile_left = board[i][j] + board[i][k];
							}
							j = k+1;
							break;
						}
						else {
							j = k;
							break;
						}
					}
				}
				if (k == 4)
					break;
			}
		}
		// Move right
		for(int j = 3; j > 0;) {
			if (board[i][j] == 0) {
				j--;
			}
			else {
				int k;
				for(k = j-1; k >= 0;) {
					if (board[i][k] == 0) {
						k--;
					}
					else {
						if (abs(fib_map[board[i][j]] - fib_map[board[i][k]]) == 1 ||
							(board[i][j] == 1 && board[i][k] == 1))
						{
							if (max_tile_right < board[i][j] + board[i][k]) {
								max_tile_right = board[i][j] + board[i][k];
							}
							j = k-1;
							break;
						}
						else {
							j = k;
							break;
						}
					}
				}
				if (k < 0)
					break;
			}
		}
	}

	return num_mergables_horizontal;
}

int Fib2584Ai::Heuristic::valueOfMaxTile(int board[4][4], int max_tile_pos[2])
{
	int max_tile = 0;

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if (board[i][j] > max_tile) {
				max_tile = board[i][j];
				max_tile_pos[0] = i;
				max_tile_pos[1] = j;
			}
		}
	}

	return max_tile;
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
				a[i][j] = b[i][j];
				notEqual = 1;
			}
		}
	}

	return notEqual;
}