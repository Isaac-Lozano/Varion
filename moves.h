#ifndef __MOVES_H__
#define __MOVES_H__

#include <stdint.h>


#define MOVE_CAP_PAWN    0x01
#define MOVE_CAP_KNIGHT  0x02
#define MOVE_CAP_L_ROOK  0x04
#define MOVE_CAP_R_ROOK  0x08
#define MOVE_CAP_KING    0x10
#define MOVE_CAP_ENEMY   0x20
#define MOVE_CAP_MACHINE 0x40
#define MOVE_CAP_TRUE    0x80


#define MOVE_EQUAL(a, b) ((a).from == (b).from && (a).to == (b).to)


typedef struct
{
    uint8_t from;
    uint8_t to;
    uint8_t cap_flags;
} move_t;


void move_print_arr(move_t *move_arr, uint8_t move_len);
void move_perform(move_t *input_move);
void move_undo(move_t *input_move);

#endif /* defined __MOVES_H__ */
