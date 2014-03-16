#ifndef __VARION_H__
#define __VARION_H__

#include <stdint.h>
#include "moves.h"
#include "bitboard.h"


/* Defines */

#define GAME_BOARD_WIDTH 5
#define GAME_BOARD_HEIGHT 8

#define MAX_MOVES 51
#define MAX_DEPTH 20

#define ANSI_ESCAPE "\x1b"


/* Global Variables */

bitboard pawn_board, knight_board, l_rook_board, r_rook_board, king_board, enemy_board;
move_t legal_moves[MAX_DEPTH][MAX_MOVES];
int legal_moves_length[MAX_DEPTH]; /* How many legal moves in each row */


/* Function Declarations */

void setup(void);
void print_board(void);
void get_move(void);
unsigned char check_game_over(void);
void make_move(void);
uint8_t get_enemy_legal_moves(move_t *move_arr);

#endif /* defined __VARION_H__ */
