#ifndef __VARION_H__
#define __VARION_H__

#include <stdint.h>
#include "moves.h"
#include "bitboard.h"


/* Defines */

#define GAME_BOARD_WIDTH 5
#define GAME_BOARD_HEIGHT 8

#define PAWN_BOARD 0
#define KNIGHT_BOARD 1
#define L_ROOK_BOARD 2
#define R_ROOK_BOARD 3
#define KING_BOARD 4
#define ENEMY_BOARD 5
#define MACHINE_BOARD 6
#define BITBOARD_TOTAL 7 /* Number of bitboards there are */

#define MAX_MOVES 51
#define MAX_DEPTH 11


/* Global Variables */

//bitboard pawn_board, knight_board, l_rook_board, r_rook_board, king_board, enemy_board;
bitboard boards[BITBOARD_TOTAL];
move_t legal_moves[MAX_DEPTH][MAX_MOVES];
int legal_moves_length[MAX_DEPTH]; /* How many legal moves in each row */

move_t rejection_moves[MAX_MOVES];
uint8_t rejection_len;
extern uint64_t node_count;


/* Function Declarations */

void setup(void);
void print_board(void);
void get_move(void);
unsigned char check_game_over(void);
int make_move(void);
uint8_t get_enemy_legal_moves(move_t *move_arr);
uint8_t get_machine_legal_moves(move_t *move_arr);

#endif /* defined __VARION_H__ */
