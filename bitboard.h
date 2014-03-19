#ifndef __BITBOARD_H__
#define __BITBOARD_H__

#include <stdint.h>


/* Macros */

#define BITBOARD_HAS_PIECE(a, x, y) (((a) >> (63 - 8*(y) - (x))) & 1)
#define BITBOARD_SET_BIT(a, x, y) ((a) = ((a) | (0x01ULL << (63 - 8*(y) - (x)))))
#define BITBOARD_UNSET_BIT(a, x, y) ((a) = ((a) & ~(0x01ULL << (63 - 8*(y) - (x)))))

#define ANSI_ESCAPE_RED   "\x1b[32m"
#define ANSI_ESCAPE_GREEN "\x1b[31m"
#define ANSI_ESCAPE_WHITE "\x1b[37m"


/* Data Structures */

typedef uint64_t bitboard;


/* Functions */

void bitboard_print(void);

#endif /* defined __BITBOARD_H__ */
