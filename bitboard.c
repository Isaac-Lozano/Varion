#include "bitboard.h"
#include "Varion.h"
#include "moves.h"

#include <stdio.h>

uint8_t bitboard_magic_table[64] = {
     0,  1, 48,  2, 57, 49, 28,  3,
    61, 58, 50, 42, 38, 29, 17,  4,
    62, 55, 59, 36, 53, 51, 43, 22,
    45, 39, 33, 30, 24, 18, 12,  5,
    63, 47, 56, 27, 60, 41, 37, 16,
    54, 35, 52, 21, 44, 32, 23, 11,
    46, 26, 40, 15, 34, 20, 31, 10,
    25, 14, 19,  9, 13,  8,  7,  6
};


/* Print current board state */
void bitboard_print(void)
{
    bitboard full_board = boards[PAWN_BOARD] | boards[KNIGHT_BOARD] | boards[L_ROOK_BOARD] |
                          boards[R_ROOK_BOARD] | boards[KING_BOARD];

    for(int i = GAME_BOARD_HEIGHT - 1; i >= 0; i--)
    {
        printf("\t%d", i);

        for(uint8_t j = 0; j < 8; j++)
        {

            if(BITBOARD_HAS_PIECE(full_board, j, i))
            {
                printf(" ");

                if(BITBOARD_HAS_PIECE(boards[ENEMY_BOARD], j, i))
                { printf(ANSI_ESCAPE_RED); }
                else
                { printf(ANSI_ESCAPE_GREEN); }

                if(BITBOARD_HAS_PIECE(boards[PAWN_BOARD], j, i))
                { printf("P"); } 
                else if(BITBOARD_HAS_PIECE(boards[KNIGHT_BOARD], j, i))
                { printf("N"); } 
                else if(BITBOARD_HAS_PIECE(boards[L_ROOK_BOARD], j, i))
                { printf("L"); } 
                else if(BITBOARD_HAS_PIECE(boards[R_ROOK_BOARD], j, i))
                { printf("R"); } 
                else
                { printf("K"); }

                printf(ANSI_ESCAPE_WHITE);
                printf(" ");
            }
            else
            {
                printf(" - ");
            }
        }

        printf("\n");
    }

    printf("\t  0  1  2  3  4\n\n\n");
}


bitboard bitboard_flip(bitboard x)
{
    x = ((x >>  1) & 0x5555555555555555) | ((x & 0x5555555555555555) <<  1);
    x = ((x >>  2) & 0x3333333333333333) | ((x & 0x3333333333333333) <<  2);
    x = ((x >>  4) & 0x0F0F0F0F0F0F0F0F) | ((x & 0x0F0F0F0F0F0F0F0F) <<  4);
    x = ((x >>  8) & 0x00FF00FF00FF00FF) | ((x & 0x00FF00FF00FF00FF) <<  8);
    x = ((x >> 16) & 0x0000FFFF0000FFFF) | ((x & 0x0000FFFF0000FFFF) << 16);
    x = ( x >> 32) | ( x << 32);
    return x;
}

//uint8_t get_enemy_legal_moves(move_t *move_arr) /* Depth tells us where to put the moves list */
//{
//    uint8_t counter = 0;
//    bitboard full_board = pawn_board | knight_board | l_rook_board |
//                          r_rook_board | king_board;
//
//    for(uint8_t i = 0; i < GAME_BOARD_HEIGHT; i++)
//    {
//        for(uint8_t j = 0; j < GAME_BOARD_WIDTH; j++)
//        {
//            if(BITBOARD_HAS_PIECE(enemy_board, j, i))
//            {
//               if(BITBOARD_HAS_PIECE(pawn_board, j, i))
//               {
//                  if(i > 0 && j < 4 && BITBOARD_HAS_PIECE(full_board ^ enemy_board, j+1, i-1))
//                  {
//                      printf("Found an upright move: %d, %d\n", j, i);
//                      move_t new_move = {j, i, j+1, i-1};
//                      move_arr[counter++] = new_move;
//                  }
//                  if(i > 0 && j > 0 && BITBOARD_HAS_PIECE(full_board ^ enemy_board, j-1, i-1))
//                  {
//                      printf("Found an upleft move: %d, %d\n", j, i);
//                      move_t new_move = {j, i, j-1, i-1};
//                      move_arr[counter++] = new_move;
//                  }
//                  if(i > 0 && !BITBOARD_HAS_PIECE(full_board, j, i-1))
//                  {
//                      printf("Found an up move: %d, %d\n", j, i);
//                      move_t new_move = {j, i, j, i-1};
//                      move_arr[counter++] = new_move;
//                  }
//               }
//            }
//        }
//    }
//    return counter;
//}
