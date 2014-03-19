#ifndef __MINIMAX_H__
#define __MINIMAX_H__

#include <stdint.h>


int minimax_min(uint8_t depth, int parent_best);
int minimax_max(uint8_t depth, int parent_worst);
int minimax_eval();

#endif /* defined __MINIMAX_H__ */
