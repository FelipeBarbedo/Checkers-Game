#include <stdio.h>
#include <stdlib.h>

#define BOARD_SIZE 8

typedef struct
{
	int source_row;
	int source_col;
	int target_row;
	int target_col;
} Move;

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

int main()
{
	int** board = create_board();
	int player = 2;
	int num_moves;

	Move* valid_moves = get_valid_moves(board, player, &num_moves);

	for (int i = 0; i< num_moves; i++)
	{
		printf("Move %d: (%d, %d) -> (%d, %d)\n", i + 1, valid_moves[i].source_row,
			valid_moves[i].source_col, valid_moves[i].target_row, valid_moves[i].target_col);
	}

	free_valid_moves(valid_moves);

	print_board(board);

	free_board(board);

	return 0;
}