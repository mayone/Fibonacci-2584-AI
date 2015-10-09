#include "Fib2584Ai.h"

Fib2584Ai::Fib2584Ai()
{
}

void Fib2584Ai::initialize(int argc, char* argv[])
{
	generateFibMap(fib_map);

	//srand(time(NULL));
	return;
}

MoveDirection Fib2584Ai::generateMove(int board[4][4])
{
	int max_num = 0;
	int max_loc[2];

	int num_mergables[2];
	int max_tile[4];
	int max_num_mergables = 0;

	int local_density[4];

	int move_score[4];
	int max_move_score;
	int optimal_move = -1;
	MoveDirection move;

	bool boardChanged = boardcmp(board_prev, board);

	//displayBoard(board);

	max_tile[MOVE_UP] = 0;
	max_tile[MOVE_RIGHT] = 0;
	max_tile[MOVE_DOWN] = 0;
	max_tile[MOVE_LEFT] = 0;
	num_mergables[VERTICAL] = numberOfVerticalMergables(board, max_tile[MOVE_UP], max_tile[MOVE_DOWN]);
	num_mergables[HORIZONTAL] = numberOfHorizontalMergables(board, max_tile[MOVE_LEFT], max_tile[MOVE_RIGHT]);

	//printf("num_mergables_vertical = %d\n", num_mergables[VERTICAL]);
	//printf("num_mergables_horizontal = %d\n", num_mergables[HORIZONTAL]);

	// Find max number and it's location
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if (board[i][j] > max_num) {
				max_num = board[i][j];
				max_loc[0] = i;
				max_loc[1] = j;
			}
		}
	}

	//getLocalDensity(board, local_density);

	// Evaluate score for each direction
	move_score[MOVE_UP] = num_mergables[VERTICAL] + fib_map[max_tile[MOVE_UP]]/2;
	move_score[MOVE_RIGHT] = num_mergables[HORIZONTAL] + fib_map[max_tile[MOVE_RIGHT]]/2;
	move_score[MOVE_DOWN] = num_mergables[VERTICAL] + fib_map[max_tile[MOVE_DOWN]]/2;
	move_score[MOVE_LEFT] = num_mergables[HORIZONTAL] + fib_map[max_tile[MOVE_LEFT]]/2;

	if (prev_move == MOVE_DOWN || prev_move == MOVE_RIGHT) {
		move_score[MOVE_UP] += 1;
	}
	if (prev_move == MOVE_LEFT || prev_move == MOVE_UP) {
		move_score[MOVE_RIGHT] += 1;
	}
	if (abs(fib_map[board[0][2]] - fib_map[board[0][1]]) > 2 && abs(fib_map[board[0][2]] - fib_map[board[0][3]]) > 2) {
		move_score[MOVE_DOWN] += 2;
	}
	if (max_loc[1] == 3 && board[0][3] == 0) {
		move_score[MOVE_UP] += 5;
	}
	if (max_loc[0] == 0 && board[0][3] == 0) {
		move_score[MOVE_RIGHT] += 5;
	}


	move_score[MOVE_UP] += 1;
	move_score[MOVE_RIGHT] += 1;

	//printf("move_score UP = %d\n", move_score[MOVE_UP]);
	//printf("move_score RIGHT = %d\n", move_score[MOVE_RIGHT]);
	//printf("move_score DOWN = %d\n", move_score[MOVE_DOWN]);
	//printf("move_score LEFT = %d\n", move_score[MOVE_LEFT]);

	optimal_move = MOVE_UP;
	max_move_score = move_score[MOVE_UP];
	for(int i = 1; i < 4; i++) {
		if (max_move_score < move_score[i]) {
			optimal_move = i;
			max_move_score = move_score[i];
		}
	}

	if (!boardChanged) {
		//printf("Not changed\n");
		optimal_move = (prev_move + 1) % 4;
	}

	prev_move = optimal_move;
/*
	printf("decided move: ");
	switch(optimal_move) {
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
	printf("-------------------------\n");
*/
	move = static_cast<MoveDirection>(optimal_move);	
	return move;
/*
	// Find max number and it's location
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if (board[i][j] > max_num) {
				max_num = board[i][j];
				max_loc[0] = i;
				max_loc[1] = j;
			}
		}
	}
*/
/*
	MoveDirection randomMove = static_cast<MoveDirection>(rand() % 4);
	return randomMove;
*/
}

void Fib2584Ai::gameOver(int board[4][4], int iScore)
{
	return;
}

/**********************************
You can implement any additional functions
you may need.
**********************************/
int Fib2584Ai::numberOfVerticalMergables(int board[4][4], int &max_tile_up, int &max_tile_down)
{
	int num_mergables_vertical = 0;

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
							(board[i][j] == 1 && board[k][j] == 1))
						{
							if (max_tile_up < board[i][j] + board[k][j]) {
								max_tile_up = board[i][j] + board[k][j];
							}
							num_mergables_vertical++;
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
							if (max_tile_down < board[i][j] + board[k][j]) {
								max_tile_down = board[i][j] + board[k][j];
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

int Fib2584Ai::numberOfHorizontalMergables(int board[4][4], int &max_tile_left, int &max_tile_right)
{
	int num_mergables_horizontal = 0;

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
							(board[i][j] == 1 && board[i][k] == 1))
						{
							if (max_tile_left < board[i][j] + board[i][k]) {
								max_tile_left = board[i][j] + board[i][k];
							}
							num_mergables_horizontal++;
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

void Fib2584Ai::getLocalDensity(int board[4][4], int local_density[4])
{
	for(int i = 0; i < 4; i++) {
		local_density[i] = 0;
	}

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if (i < 2 && j < 2) {
				local_density[TOP_LEFT] += board[i][j];
			}
			else if (i < 2 && j >= 2) {
				local_density[TOP_RIGHT] += board[i][j];
			}
			else if (i >= 2 && j < 2) {
				local_density[BOTTOM_LEFT] += board[i][j];
			}
			else {
				local_density[BOTTOM_RIGHT] += board[i][j];
			}
		}
	}
}

void generateFibMap(std::map<int, int> &fib_map)
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

void displayBoard(int board[4][4])
{
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			printf("%d\t", board[i][j]);
		}
		printf("\n");
	}
}

bool boardcmp(int a[4][4], int b[4][4])
{
	bool changed = 0;
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if (a[i][j] != b[i][j]) {
				a[i][j] = b[i][j];
				changed = 1;
			}
		}
	}
	return changed;
}