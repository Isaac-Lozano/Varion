#ifndef __VARION_H__
#define __VARION_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


/* Defines */

#define GAME_BOARD_WIDTH 5
#define GAME_BOARD_HEIGHT 8

#define MAX_MOVES 51
#define MAX_DEPTH 20

#define ANSI_ESCAPE "\x1b"

/* Macros */

#define BITBOARD_HAS_PIECE(a, x, y) ((a >> (63 - 8*y - x)) & 1)


/* Data Structures */

typedef uint64_t bitboard;


/* Global Variables */

bitboard pawn_board, knight_board, l_rook_board, r_rook_board, king_board, enemy_board;
uint8_t legal_moves[MAX_DEPTH][MAX_MOVES];
int legal_moves_length[MAX_DEPTH]; /* How many legal moves in each row */


/* Function Declarations */

void setup(void);
void print_board(void);
void get_move(void);
unsigned char check_game_over(void);
void make_move(void);

#endif /* defined __VARION_H__ */
