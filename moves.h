#ifndef __MOVES_H__
#define __MOVES_H__

#include <stdint.h>


#define MOVE_EQUAL(a, b) ((a).x[0] == (b).x[0] &&\
                          (a).x[1] == (b).x[1] &&\
                          (a).x[2] == (b).x[2] &&\
                          (a).x[3] == (b).x[3])


typedef struct
{
    uint8_t x[4];
} move_t;


void move_print_arr(move_t *move_arr, uint8_t move_len);
void move_perform(move_t input_move);

#endif /* defined __MOVES_H__ */
