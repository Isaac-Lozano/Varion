#ifndef __BITBOARD_H__
#define __BITBOARD_H__

#include <stdint.h>


/* Macros */

#define BITBOARD_HAS_PIECE(a, x, y) (((a) >> ((y << 3) + x)) & 1)
#define BITBOARD_HAS_INDEX(a, i) (((a) >> i) & 0x1)
#define BITBOARD_SET_BIT(a, x) ((a) |= (0x01ULL << x))
#define BITBOARD_UNSET_BIT(a, x) ((a) &= ~(0x01ULL << x))
#define BITBOARD_GET_INDEX(x, y) (((y) << 0x03) + (x))
#define BITBOARD_GET_RANK(a) ((a) >> 3)
#define BITBOARD_GET_FILE(a) ((a) & 7)
#define BITBOARD_ITERATE(a) ((a) & (-1 * (a))); ((a) &= (a) - 1)

#define ANSI_ESCAPE_RED   "\x1b[32m"
#define ANSI_ESCAPE_GREEN "\x1b[31m"
#define ANSI_ESCAPE_WHITE "\x1b[37m"

#define BITBOARD_MAGIC 0x03f79d71b4cb0a89


/* Global Variables */

extern uint8_t bitboard_magic_table[64];


/* Data Structures */

typedef uint64_t bitboard;


/* Functions */

void bitboard_print(void);
bitboard bitboard_flip(bitboard x);

#endif /* defined __BITBOARD_H__ */
