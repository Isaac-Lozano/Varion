#include "moves.h"
#include "Varion.h"
#include "bitboard.h"
#include <stdio.h>


void move_print_arr(move_t *move_arr, uint8_t move_len)
{
    printf("Found %d moves: ", move_len);
    for(uint8_t i = 0; i < move_len; i++)
    {
        printf("%c%d%c%d ", BITBOARD_GET_FILE(move_arr[i].from) + 'A', BITBOARD_GET_RANK(move_arr[i].from) + 1,
                            BITBOARD_GET_FILE(move_arr[i].to) + 'A', BITBOARD_GET_RANK(move_arr[i].to) + 1);
    }
    printf("\n");
}


void move_perform(move_t *input_move) /* TODO: Optimize */
{
    input_move->cap_flags = 0x00;
    for(uint8_t i = 0; i < BITBOARD_TOTAL; i++)
    {
        uint8_t has_piece = BITBOARD_HAS_INDEX(boards[i], input_move->to);
        input_move->cap_flags |= (has_piece << i);
        input_move->cap_flags |= (has_piece << 7);
        BITBOARD_UNSET_BIT(boards[i], input_move->to);
    }

    for(uint8_t i = 0; i < BITBOARD_TOTAL; i++)
    {
        if(BITBOARD_HAS_INDEX(boards[i], input_move->from))
        {
            BITBOARD_UNSET_BIT(boards[i], input_move->from);
            BITBOARD_SET_BIT(boards[i], input_move->to);
        }
    }
}


void move_undo(move_t *input_move) /* TODO: Optimize */
{
    for(uint8_t i = 0; i < BITBOARD_TOTAL; i++)
    {
        if(BITBOARD_HAS_INDEX(boards[i], input_move->to))
        {
            BITBOARD_UNSET_BIT(boards[i], input_move->to);
            BITBOARD_SET_BIT(boards[i], input_move->from);
        }
    }

    for(uint8_t i = 0; i < BITBOARD_TOTAL; i++)
    {
        if(input_move->cap_flags & (0x01 << i))
        {
            BITBOARD_SET_BIT(boards[i], input_move->to);
        }
    }
}
