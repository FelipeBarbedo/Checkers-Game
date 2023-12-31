// cd /mnt/c/Users/felip/Desktop/Study-WS/IPPD/Checkers_Game
// gcc -o checkers checkers.c -Wall
// ./checkers

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

int** create_board();

int** copy_board(int** board, int** aux_board);

void free_board(int** board);

void print_board(int** board);

int is_inside_board(int row, int col);

int is_empty_cell(int** board, int row, int col);

int is_player_piece(int** board, int row, int col, int player);

int is_capture_position(int** board, int row, int col, int player);

void add_valid_move(Move* valid_moves, int* num_moves, int source_row, int source_col, int target_row, int target_col);

void get_valid_moves_for_piece(int** board, int row, int col, int player, Move* valid_moves, int* num_moves);

void get_valid_capture_for_piece(int** board, int row, int col, int player, Move* valid_moves, int* num_moves);

Move* get_valid_moves(int** board, int player, int* num_moves);

void free_valid_moves(Move* valid_moves);

void make_move(int** board, Move move, Move* valid_moves, int num_moves);

int evaluate_board(int** board);

int minimax(int** board, int depth, int maximizingPlayer, int alpha, int beta);

void robot_move(int** board);

int main()
{
	int** board = create_board();
	int player = 1;

	int num_moves;
	Move move;
	Move* valid_moves;

	while (1)
	{
		printf("\ec");
		print_board(board);
		scanf("%d%d%d%d", &move.source_row, &move.source_col, &move.target_row, &move.target_col);
	
		valid_moves = get_valid_moves(board, player, &num_moves);
		
		make_move(board, move, valid_moves, num_moves);
	
		printf("\ec");
		
		print_board(board);
	
		robot_move(board);
	
		printf("\ec");
		print_board(board);

	}


/*
	valid_moves = get_valid_moves(board, player, &num_moves);
	move.source_row = 2;
	move.source_col = 3;
	move.target_row = 3;
	move.target_col = 4;
	make_move(board, move, valid_moves, num_moves);

	print_board(board);

	robot_move(board);

	print_board(board);

	robot_move(board);

	print_board(board);

	robot_move(board);

	print_board(board);

/*	player = 2;
	print_board(board);

	valid_moves = get_valid_moves(board, player, &num_moves);
	move.source_row = 5;
	move.source_col = 4;
	move.target_row = 4;
	move.target_col = 3;
	make_move(board, move, valid_moves, num_moves);


	player = 1;
	print_board(board);

	valid_moves = get_valid_moves(board, player, &num_moves);
	move.source_row = 3;
	move.source_col = 2;
	move.target_row = 5;
	move.target_col = 4;
	make_move(board, move, valid_moves, num_moves);

	player = 2;
	print_board(board);

	valid_moves = get_valid_moves(board, player, &num_moves);

	make_move(board, move, valid_moves, num_moves);

	print_board(board);
*/
	for (int i = 0; i < num_moves; i++)
	{
		printf("Move %d: (%d, %d) -> (%d, %d)\n", i + 1, valid_moves[i].source_row,
			valid_moves[i].source_col, valid_moves[i].target_row, valid_moves[i].target_col);
	}

	free_valid_moves(valid_moves);

	free_board(board);

	return 0;
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

int** copy_board(int** board, int** aux_board)
{

	for (int i = 0; i < BOARD_SIZE; i++)
		for(int j = 0; j < BOARD_SIZE; j++)
		{
			aux_board[i][j] = board[i][j];
		}

	return aux_board;
}

void free_board(int** board)
{
	for (int i = 0; i < BOARD_SIZE; i++)
		free(board[i]);
	
	free(board);
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
	if (player == 1)
	{
		if (is_inside_board(row + 2, col - 2) && is_empty_cell(board, row + 2, col - 2) && 
			!is_player_piece(board, row + 1, col - 1, player) && 
			!is_empty_cell(board, row + 1, col - 1))
		{
			return 1;
		}

		if (is_inside_board(row + 2, col + 2) && is_empty_cell(board, row + 2, col + 2) &&
			!is_player_piece(board, row + 1, col + 1, player) && 
			!is_empty_cell(board, row + 1, col + 1))
		{
			return 2;
		}
	}

	if (player == 2)
	{
		if (is_inside_board(row - 2, col - 2) && is_empty_cell(board, row - 2, col - 2) &&
			!is_player_piece(board, row - 1, col - 1, player) && 
			!is_empty_cell(board, row - 1, col - 1))
		{
			return 3;
		}

		if (is_inside_board(row - 2, col + 2) && is_empty_cell(board, row - 2, col + 2) &&
			!is_player_piece(board, row - 1, col + 1, player) && 
			!is_empty_cell(board, row - 1, col + 1))
		{
			return 4;
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

	} else {
		// Left Move P2
		if (is_inside_board(row - 1, col - 1) && is_empty_cell(board, row - 1, col - 1))
			add_valid_move(valid_moves, num_moves, row, col, row - 1, col - 1);

		// Right Move P2
		if (is_inside_board(row - 1, col + 1) && is_empty_cell(board, row - 1, col + 1))
			add_valid_move(valid_moves, num_moves, row, col, row - 1, col + 1);

	}
}

void get_valid_capture_for_piece(int** board, int row, int col, int player, Move* valid_moves, int* num_moves)
{
	int aux = is_capture_position(board, row, col, player);

	switch (aux)
	{
	case 0:
		break;

	case 1:
		add_valid_move(valid_moves, num_moves, row, col, row + 2, col - 2);
		break;

	case 2:
		add_valid_move(valid_moves, num_moves, row, col, row + 2, col + 2);
		break;

	case 3:
		add_valid_move(valid_moves, num_moves, row, col, row - 2, col - 2);
		break;

	case 4:
		add_valid_move(valid_moves, num_moves, row, col, row - 2, col + 2);
		break;
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
				get_valid_capture_for_piece(board, row, col, player, valid_moves, num_moves);
			}
		}

	return valid_moves;
}

void free_valid_moves(Move* valid_moves)
{
	free(valid_moves);
}

void make_move(int** board, Move move, Move* valid_moves, int num_moves)
{
	int player = board[move.source_row][move.source_col];

	if (is_capture_position(board, move.source_row, move.source_col, player))
	{
		for (int i = 0; i < num_moves; i++)
		{
			if (move.source_row == valid_moves[i].source_row && 
				move.source_col == valid_moves[i].source_col)
			{
				if (move.target_row == valid_moves[i].target_row && 
					move.target_col == valid_moves[i].target_col)
				{
					if (player == 1)
					{
						board[move.target_row][move.target_col] = 
						board[move.source_row][move.source_col];

						board[move.source_row][move.source_col] = 0;

						if ((move.target_col - move.source_col) < 0) 
						{
							board[move.source_row + 1][move.source_col - 1] = 0;
						} else {
							board[move.source_row + 1][move.source_col + 1] = 0;
						}
					}

					if (player == 2)
					{
						board[move.target_row][move.target_col] = 
						board[move.source_row][move.source_col];

						board[move.source_row][move.source_col] = 0;

						if ((move.target_col - move.source_col) < 0) 
						{						
							board[move.source_row - 1][move.source_col - 1] = 0;
						} else {
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
					board[move.target_row][move.target_col] = 
					board[move.source_row][move.source_col];

					board[move.source_row][move.source_col] = 0;
				}
			}
		}	
	}
}

int evaluate_board(int** board)
{
	int robot_piece = 0;

	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			if (board[i][j] == 2)
				robot_piece++;

	return robot_piece;
}

int minimax(int** board, int depth, int maximizingPlayer, int alpha, int beta)
{
	int** aux_board = create_board();
	aux_board = copy_board(board, aux_board);

	if (depth == 0)
	{
		return evaluate_board(aux_board);
	}

	if (maximizingPlayer)
	{
		int maxEval = INT_MIN;
		int num_moves;

		Move* valid_moves;

		valid_moves = get_valid_moves(aux_board, 2, &num_moves);

		for (int i = 0; i < num_moves; i++)
		{
			make_move(aux_board, valid_moves[i], valid_moves, num_moves);

			int eval = minimax(aux_board, depth - 1, 0, alpha, beta);

			aux_board = copy_board(board, aux_board);

			maxEval = (eval > maxEval) ? eval : maxEval;

			alpha = (alpha > maxEval) ? alpha : maxEval;

			if (beta <= alpha)
			{
				break;
			}
		}

		free_board(aux_board);
		free_valid_moves(valid_moves);

		return maxEval;

	} else {
		int minEval = INT_MAX;
		int num_moves;

		Move* valid_moves;

		valid_moves = get_valid_moves(aux_board, 1, &num_moves);

		for (int i = 0; i < num_moves; i++)
		{
			make_move(aux_board, valid_moves[i], valid_moves, num_moves);

			int eval = minimax(aux_board, depth - 1, 1, alpha, beta);

			aux_board = copy_board(board, aux_board);

			minEval = (eval < minEval) ? eval : minEval;

			beta = (beta < minEval) ? beta : minEval;

			if (beta <= alpha)
			{
				break;
			}
		}

		free_board(aux_board);
		free_valid_moves(valid_moves);

		return minEval;
	}
}

void robot_move(int** board)
{
	int** aux_board = create_board();
	aux_board = copy_board(board, aux_board);

	int depth = 3;
	int maximizingPlayer = 2;
	int num_moves;

	Move* valid_moves = get_valid_moves(aux_board, maximizingPlayer, &num_moves);

	int bestScore = INT_MIN;
	int bestMoveIndex = -1;

	for (int i = 0; i < num_moves; i++)
	{
		make_move(aux_board, valid_moves[i], valid_moves, num_moves);

		int score = minimax(aux_board, depth - 1, 0, INT_MIN, INT_MAX);

		aux_board = copy_board(board, aux_board);		

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

	free_board(aux_board);
	free_valid_moves(valid_moves);
}