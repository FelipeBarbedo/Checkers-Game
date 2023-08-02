#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define BOARD_SIZE 8

typedef struct
{
	int source_row;
	int source_col;
	int target_row;
	int target_col;
} Move;

void free_valid_moves(Move* valid_moves)
{
	free(valid_moves);
}

void print_board(int** board)
{
	printf("   ");
	for (int i = 0; i < BOARD_SIZE; i++)
		printf("%d ", i);

	printf("\n   ");
	for (int i = 0; i < 15; i++)
		printf("_");

	printf("\n");
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		//printf("%c |", ('A' + i));
		printf("%d |", i);

		for (int j = 0; j < BOARD_SIZE; j++)
			printf("%d ", board[i][j]);

		printf("\n");
	}
}

int is_inside_board(int row, int col)
{
	return (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE);
}

int is_empty_cell(int** board, int row, int col) 
{
	return (board[row][col] == 0);
}

int is_player_piece(int** board, int row, int col, int player)
{
	return (board[row][col] == player);
}

int is_capture_position(int** board, int row, int col, int player)
{
	if (!is_inside_board(row, col) || !is_empty_cell(board, row, col))
	{
		return 0;
	}

	if (player == 1)
	{
		if (is_empty_cell(board, row + 2, col - 2) && 
			is_player_piece(board, row + 1, col - 1, player))
		{
			return 1;
		}

		if (is_empty_cell(board, row + 2, col + 2) &&
			is_player_piece(board, row + 1, col + 1, player))
		{
			return 1;
		}
	}

	if (player == 2)
	{
		if (is_empty_cell(board, row - 2, col - 2) &&
			is_player_piece(board, row - 1, col - 1, player))
		{
			return 1;
		}

		if (is_empty_cell(board, row - 2, col + 2) &&
			is_player_piece(board, row - 1, col + 1, player))
		{
			return 1;
		}
	}
	
	return 0;
}

void add_valid_move(Move* valid_moves, int* num_moves, int source_row, int source_col, int target_row, int target_col)
{
	valid_moves[*num_moves].source_row = source_row;
	valid_moves[*num_moves].source_col = source_col;
	valid_moves[*num_moves].target_row = target_row;
	valid_moves[*num_moves].target_col = target_col;
	(*num_moves)++;
}

void get_valid_moves_for_piece(int** board, int row, int col, int player, Move* valid_moves, int* num_moves)
{
	if (player == 1)
	{
		// Left Move P1
		if (is_inside_board(row + 1, col - 1) && is_empty_cell(board, row + 1, col - 1))
			add_valid_move(valid_moves, num_moves, row, col, row + 1, col - 1);

		// Right Move P1
		if (is_inside_board(row + 1, col + 1) && is_empty_cell(board, row + 1, col + 1))
			add_valid_move(valid_moves, num_moves, row, col, row + 1, col + 1);

		// Left Capture P1
		if (is_capture_position(board, row + 1, col - 1, player))
			add_valid_move(valid_moves, num_moves, row, col, row + 2, col - 2);

		// Right Capture P1
		if (is_capture_position(board, row + 1, col + 1, player))
			add_valid_move(valid_moves, num_moves, row, col, row + 2, col + 2);

	} else {
		// Left Move P2
		if (is_inside_board(row - 1, col - 1) && is_empty_cell(board, row - 1, col - 1))
			add_valid_move(valid_moves, num_moves, row, col, row - 1, col - 1);

		// Right Move P2
		if (is_inside_board(row - 1, col + 1) && is_empty_cell(board, row - 1, col + 1))
			add_valid_move(valid_moves, num_moves, row, col, row - 1, col + 1);

		// Left Capture P1
		if (is_capture_position(board, row - 1, col - 1, player))
			add_valid_move(valid_moves, num_moves, row, col, row - 2, col - 2);

		// Right Capture P1
		if (is_capture_position(board, row - 1, col + 1, player))
			add_valid_move(valid_moves, num_moves, row, col, row - 2, col + 2);

	}
}

Move* get_valid_moves(int** board, int player, int* num_moves)
{
	Move* valid_moves = (Move*)malloc(BOARD_SIZE * BOARD_SIZE * sizeof(Move));
	*num_moves = 0;

	for (int row = 0; row < BOARD_SIZE; row++)
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			if (is_player_piece(board, row, col, player))
			{
				get_valid_moves_for_piece(board, row, col, player, valid_moves, num_moves);
			}
		}

	return valid_moves;
}

void make_move(int** board, Move move, Move* valid_moves, int num_moves)
{
	int player = board[move.source_row][move.source_col];

	if (is_capture_position(board, move.target_row, move.target_col, player))
	{
		for (int i = 0; i < num_moves; i++)
		{
			if (move.source_row == valid_moves[i].source_row && 
				move.source_col == valid_moves[i].source_col)
			{
				if (move.target_row == valid_moves[i].target_row && 
					move.target_col == valid_moves[i].target_col)
				{
					board[move.target_row][move.target_col] = board[move.source_row][move.source_col];
					if (player == 1)
					{
						if (move.target_row > 0 && move.target_col < 0) 
						{
							board[move.target_row][move.target_col] = board[move.source_row][move.source_col];
							board[move.source_row][move.source_col] = 0;
							board[move.source_row + 1][move.source_col - 1] = 0;
						}

						if (move.target_row > 0 && move.target_col > 0) 
						{
							board[move.target_row][move.target_col] = board[move.source_row][move.source_col];
							board[move.source_row][move.source_col] = 0;
							board[move.source_row + 1][move.source_col + 1] = 0;
						}
					}

					if (player == 2)
					{
						if (move.target_row < 0 && move.target_col < 0) 
						{
							board[move.target_row][move.target_col] = board[move.source_row][move.source_col];
							board[move.source_row][move.source_col] = 0;
							board[move.source_row - 1][move.source_col - 1] = 0;
						}

						if (move.target_row < 0 && move.target_col > 0) 
						{
							board[move.target_row][move.target_col] = board[move.source_row][move.source_col];
							board[move.source_row][move.source_col] = 0;
							board[move.source_row - 1][move.source_col + 1] = 0;
						}
					}
					
				}
			}
		}
	} else {
		for (int i = 0; i < num_moves; i++)
		{
			if (move.source_row == valid_moves[i].source_row && 
				move.source_col == valid_moves[i].source_col)
			{
				if (move.target_row == valid_moves[i].target_row && 
				move.target_col == valid_moves[i].target_col)
				{
					board[move.target_row][move.target_col] = board[move.source_row][move.source_col];
					board[move.source_row][move.source_col] = 0;
				}
			}
		}	
	}
}

void undo_move(int** board, Move move)
{
	//int player = board[move.source_row][move.source_col];

	/*if (player == 1)
	{
		if ((move.source_row - move.target_row) == 2 && 
			(move.source_col - move.target_col) == -2)
		{
			board[move.source_row + 1][move.source_col - 1] = player;
		}

		if ((move.source_row - move.target_row) == 2 && 
			(move.source_col - move.target_col) == 2)
		{
			board[move.source_row + 1][move.source_col + 1] = player;
		}
	}

	if (player == 2)
	{
		if ((move.source_row - move.target_row) == -2 && 
			(move.source_col - move.target_col) == -2)
		{
			board[move.source_row - 1][move.source_col - 1] = player;
		}

		if ((move.source_row - move.target_row) == -2 && 
			(move.source_col - move.target_col) == 2)
		{
			board[move.source_row - 1][move.source_col + 1] = player;
		}
	}*/

	board[move.source_row][move.source_col] = board[move.target_row][move.target_col];
	board[move.target_row][move.target_col] = 0;
}

int evaluate_board(int** board)
{
	int robot_piece = 0;

	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] == 2)
			{
				robot_piece++;
			}
		}

	return robot_piece;
}

int minimax(int** board, int depth, int maximizingPlayer, int alpha, int beta)
{
	if (depth == 0)
	{
		return evaluate_board(board);
	}

	if (maximizingPlayer)
	{
		int maxEval = INT_MIN;
		int num_moves;

		Move* valid_moves;

		valid_moves = get_valid_moves(board, 2, &num_moves);

		for (int i = 0; i < num_moves; i++)
		{
			make_move(board, valid_moves[i], valid_moves, num_moves);

			int eval = minimax(board, depth - 1, 0, alpha, beta);

			undo_move(board, valid_moves[i]);

			maxEval = (eval > maxEval) ? eval : maxEval;

			alpha = (alpha > maxEval) ? alpha : maxEval;

			if (beta <= alpha)
			{
				break;
			}
		}

		free_valid_moves(valid_moves);

		return maxEval;

	} else {
		int minEval = INT_MAX;
		int num_moves;

		Move* valid_moves;

		valid_moves = get_valid_moves(board, 1, &num_moves);

		for (int i = 0; i < num_moves; i++)
		{
			make_move(board, valid_moves[i], valid_moves, num_moves);

			int eval = minimax(board, depth - 1, 1, alpha, beta);

			undo_move(board, valid_moves[i]);

			minEval = (eval < minEval) ? eval : minEval;

			beta = (beta < minEval) ? beta : minEval;

			if (beta <= alpha)
			{
				break;
			}
		}

		free_valid_moves(valid_moves);

		return minEval;
	}
}

void robot_move(int** board)
{
	int depth = 3;
	int maximizingPlayer = 2;

	int num_moves;

	Move* valid_moves = get_valid_moves(board, maximizingPlayer, &num_moves);

	int bestScore = INT_MIN;
	int bestMoveIndex = -1;

	for (int i = 0; i < num_moves; i++)
	{
		make_move(board, valid_moves[i], valid_moves, num_moves);

		print_board(board); //2

		int score = minimax(board, depth - 1, 0, INT_MIN, INT_MAX);

		undo_move(board, valid_moves[i]);

		

		if (score > bestScore)
		{
			bestScore = score;
			bestMoveIndex = i;
		}
	}

	if (bestMoveIndex >= 0)
	{
		make_move(board, valid_moves[bestMoveIndex], valid_moves, num_moves);
	}

	free_valid_moves(valid_moves);
}

int** create_board()
{
	int** board = (int**)malloc(BOARD_SIZE * sizeof(int*));

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		board[i] = (int*)malloc(BOARD_SIZE * sizeof(int));
	}

	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			board[i][j] = 0;

			// 1 para peças do jogador humano.
			if ((i % 2 == 0) && (j % 2 == 1) && (i < 3))
			{
				board[i][j] = 1;
			}

			if ((i % 2 == 1) && (j % 2 == 0) && (i < 3))
			{
				board[i][j] = 1;
			}

			// 2 para peças do jogador robô. 
			if ((i % 2 == 0) && (j % 2 == 1) && (i > 4) && (i < 8))
			{
				board[i][j] = 2;
			}

			if ((i % 2 == 1) && (j % 2 == 0) && (i > 4) && (i < 8))
			{
				board[i][j] = 2;
			}
		}

	return board;
}

void free_board(int** board)
{
	for (int i = 0; i < BOARD_SIZE; i++)
		free(board[i]);
	
	free(board);
}

int main()
{
	int** board = create_board();
	int player = 1;
	int num_moves;

	Move* valid_moves;
	Move move;

	valid_moves = get_valid_moves(board, player, &num_moves);
		
	move.source_row = 2;
	move.source_col = 1;
	move.target_row = 3;
	move.target_col = 0;

	print_board(board); //1

	make_move(board, move, valid_moves, num_moves);

	valid_moves = get_valid_moves(board, player, &num_moves);

	robot_move(board);

	print_board(board);

	free_valid_moves(valid_moves);

	free_board(board);

	/*for (int i = 0; i < num_moves; i++)
	{
		printf("Move %d: (%d, %d) -> (%d, %d)\n", i + 1, valid_moves[i].source_row,
			valid_moves[i].source_col, valid_moves[i].target_row, valid_moves[i].target_col);
	}*/

	return 0;
}