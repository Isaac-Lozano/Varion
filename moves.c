#include "moves.h"
#include "Varion.h"
#include "bitboard.h"
#include <stdio.h>


void move_print_arr(move_t *move_arr, uint8_t move_len)
{
    printf("Found %d moves: ", move_len);
    for(uint8_t i = 0; i < move_len; i++)
    {
        printf("%d%d%d%d ", move_arr[i].x[0], move_arr[i].x[1], move_arr[i].x[2], move_arr[i].x[3]);
    }
    printf("\n");
}


void move_perform(move_t *input_move) /* TODO: Optimize */
{
    (*input_move).cap_flags = 0;
    for(uint8_t j = 0; j < BITBOARD_TOTAL; j++)
    {
        BITBOARD_UNSET_BIT(boards[j], (*input_move).x[2], (*input_move).x[3]);
    }

    for(uint8_t i = 0; i < BITBOARD_TOTAL; i++)
    {
        if(BITBOARD_HAS_PIECE(boards[i], (*input_move).x[0], (*input_move).x[1]))
        {
            BITBOARD_UNSET_BIT(boards[i], (*input_move).x[0], (*input_move).x[1]);
            BITBOARD_SET_BIT(boards[i], (*input_move).x[2], (*input_move).x[3]);
        }
    }
}
