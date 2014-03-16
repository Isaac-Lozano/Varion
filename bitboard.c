#include "bitboard.h"
#include "Varion.h"
#include "moves.h"


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
