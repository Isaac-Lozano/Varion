#ifndef __BITBOARD_H__
#define __BITBOARD_H__

#include <stdint.h>


/* Macros */

#define BITBOARD_HAS_PIECE(a, x, y) (((a) >> (63 - 8*(y) - (x))) & 1)
#define BITBOARD_SET_BIT(a, x, y) ((a) | (0x01 << (63 - 8*(y) - (x))))
#define BITBOARD_UNSET_BIT(a, x, y) ((a) ^ (0x01 << (63 - 8*(y) - (x))))


/* Data Structures */

typedef uint64_t bitboard;

#endif /* defined __BITBOARD_H__ */
