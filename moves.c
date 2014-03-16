#include "moves.h"
#include "Varion.h"
#include "bitboard.h"
#include <stdio.h>


void move_print_arr(move_t *move_arr, uint8_t move_len)
{
    printf("Found %d moves.\n", move_len);
    for(uint8_t i = 0; i < move_len; i++)
    {
        printf("%d%d%d%d ", move_arr[i].x[0], move_arr[i].x[1], move_arr[i].x[2], move_arr[i].x[3]);
    }
    printf("\n");
}


void move_perform(move_t input_move) /* MUST OPTIMIZE LATER */
{
    
}
