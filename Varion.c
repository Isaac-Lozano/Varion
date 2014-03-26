#include "Varion.h"
#include "minimax.h"
#include "moves.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>

uint64_t node_count;
uint8_t best_move_number = 100;

int main(void)
{
    unsigned char game_over = 0;

    setup();
    bitboard_print();

    printf("Machine goes first? [Y/n] ");
    int ch = getc(stdin);

    //while((ch = fgetc(stdin)) != '\n' && ch != EOF); /* Flush stdin */

    if(ch != 'N' && ch != 'n')
    {
        goto ai_first;
    }

    /* TODO: Add who will go first. (goto ai_first if ai == first) */

    while(!game_over)
    {
        get_move();
        bitboard_print();
        node_count = 0;

ai_first:
        if((game_over = check_game_over()))
        {
            continue;
        }


        int best = make_move();
        bitboard_print();

        printf("Current score: %d\n\n", best);
        printf("Nodes traveled: %ld\n", node_count);

        game_over = check_game_over();
    }

    return 0;
}


/* Set up default board */
void setup(void)
{
    /* Every two hex corresponds to a row.*/
    /*                 ROW =   0 1 2 3 4 5 6 7    */
    boards[PAWN_BOARD]     = 0x00000E00000E0000;
    boards[KNIGHT_BOARD]   = 0x0000110000110000;
    boards[L_ROOK_BOARD]   = 0x1000000000000010;
    boards[R_ROOK_BOARD]   = 0x0100000000000001;
    boards[KING_BOARD]     = 0x0800000000000002;
    boards[ENEMY_BOARD]    = 0x00000000001F0013;
    boards[MACHINE_BOARD]  = 0x19001F0000000000;
}



/* Get move from opponent */
void get_move(void)
{
    uint8_t move_len;
    char move_str[4]; /* I will make sure that this does not overflow */
    move_t move_arr[MAX_MOVES], input_move;

    move_len = get_enemy_legal_moves(move_arr);
    move_print_arr(move_arr, move_len);

enter_command:
    printf("Enter move: ");
    for(uint8_t i = 0; i < 4; i++)
    {
        move_str[i] = fgetc(stdin);
    }

    int ch;
    while((ch = fgetc(stdin)) != '\n' && ch != EOF); /* Flush stdin */

    if(move_str[0] <= 'E' && move_str[0] >= 'A' &&
       move_str[1] <= '8' && move_str[1] > '0' &&
       move_str[2] <= 'E' && move_str[2] >= 'A' &&
       move_str[3] <= '8' && move_str[3] > '0')
    {
        input_move.from = BITBOARD_GET_INDEX(move_str[0] - 'A', move_str[1] - '1');
        input_move.to = BITBOARD_GET_INDEX(move_str[2] - 'A', move_str[3] - '1');
    }
    else
    {
        printf("Invalid move.\n");
        while((ch = fgetc(stdin)) != '\n' && ch != EOF); /* Flush stdin */
        goto enter_command;
    }


    for(uint8_t i = 0; i < move_len; i++) /* Check if the string is valid */
    {
        if(MOVE_EQUAL(input_move, move_arr[i]))
        {
            printf("Comparing %d%d%d%d and %d%d%d%d [In Rej]\n", BITBOARD_GET_FILE(input_move.from),
                        BITBOARD_GET_RANK(input_move.from),
                        BITBOARD_GET_FILE(input_move.to),
                        BITBOARD_GET_RANK(input_move.to),
                        BITBOARD_GET_FILE((rejection_moves[best_move_number]).from),
                        BITBOARD_GET_RANK((rejection_moves[best_move_number]).from),
                        BITBOARD_GET_FILE((rejection_moves[best_move_number]).to),
                        BITBOARD_GET_RANK((rejection_moves[best_move_number]).to));

            if(best_move_number != 100 && MOVE_EQUAL(input_move, rejection_moves[best_move_number]))
            {
                printf("I reject that move. Pick another one.\n");
                while((ch = fgetc(stdin)) != '\n' && ch != EOF); /* Flush stdin */
                goto enter_command;
            }
            move_perform(&input_move);
            printf("cap_flags = %d\n", input_move.cap_flags);
            return;
        }
    }

    printf("Bad move inputted.\n");
    goto enter_command;
}


/* Make move against opponent */
int make_move(void)
{
    int best = SCORE_MIN, next_best = SCORE_MIN, depth = 0, score;
    uint8_t move_len, best_move_num = 100, next_best_move_num = 100;
    move_t move_arr[MAX_MOVES], best_move, next_best_move = {44,44,0};

    rejection_len = 0;
    move_len = get_machine_legal_moves(move_arr);

//    move_perform(move_arr);
    printf("Moves for the machine ");
    move_print_arr(move_arr, move_len);
//    return 0;
    for(uint8_t i = 0; i < move_len; i++)
    {
        move_perform(&move_arr[i]);
        printf("Checking machine move [%d%d%d%d]\n|\\\n", BITBOARD_GET_FILE(move_arr[i].from),
                        BITBOARD_GET_RANK(move_arr[i].from),
                        BITBOARD_GET_FILE(move_arr[i].to),
                        BITBOARD_GET_RANK(move_arr[i].to));
        score = minimax_min(depth+1, best, SCORE_MIN);
//        printf("Rejection move for [%d%d%d%d] is [%d%d%d%d].\n", BITBOARD_GET_FILE(move_arr[i].from),
//                        BITBOARD_GET_RANK(move_arr[i].from),
//                        BITBOARD_GET_FILE(move_arr[i].to),
//                        BITBOARD_GET_RANK(move_arr[i].to),
//                        BITBOARD_GET_FILE(rejection_moves[i].from),
//                        BITBOARD_GET_RANK(rejection_moves[i].from),
//                        BITBOARD_GET_FILE(rejection_moves[i].to),
//                        BITBOARD_GET_RANK(rejection_moves[i].to));


        if(score >= next_best)
        {
            if(score >= best)
            {
                next_best = best;
                next_best_move = best_move;
                next_best_move_num = best_move_num;
                best = score;
                best_move = move_arr[i];
                best_move_num = i;
//                printf("My next_best_move (BEST) is [%d%d%d%d]\n", BITBOARD_GET_FILE(next_best_move.from),
//                        BITBOARD_GET_RANK(next_best_move.from),
//                        BITBOARD_GET_FILE(next_best_move.to),
//                        BITBOARD_GET_RANK(next_best_move.to));
            }
            else
            {
                next_best = score;
                next_best_move = move_arr[i];
                next_best_move_num = i;
//                printf("My next_best_move is [%d%d%d%d]\n", BITBOARD_GET_FILE(next_best_move.from),
//                        BITBOARD_GET_RANK(next_best_move.from),
//                        BITBOARD_GET_FILE(next_best_move.to),
//                        BITBOARD_GET_RANK(next_best_move.to));
            }
        }
        move_undo(&move_arr[i]);
    }

    printf("My move is [%c%d%c%d], Do you Accept [Y/n] ", BITBOARD_GET_FILE(best_move.from) + 'A',
            BITBOARD_GET_RANK(best_move.from) + 1,
            BITBOARD_GET_FILE(best_move.to) + 'A',
            BITBOARD_GET_RANK(best_move.to) + 1);
    best_move_number = best_move_num;

    int ch = getc(stdin);

    if(ch == 'N' || ch == 'n')
    {
        printf("My move is [%c%d%c%d], then.\n", BITBOARD_GET_FILE(next_best_move.from) + 'A',
                                                 BITBOARD_GET_RANK(next_best_move.from) + 1,
                                                 BITBOARD_GET_FILE(next_best_move.to) + 'A',
                                                 BITBOARD_GET_RANK(next_best_move.to) + 1);
        best_move_number = next_best_move_num;
        best_move = next_best_move;
    }

    move_perform(&best_move);
    printf("Move %d was picked.\nRejection moves: ", best_move_number);
    move_print_arr(rejection_moves, rejection_len);
    return best;
}


/* Check for a game-over state */
/* Returns 1 if game_over */
int check_game_over(void)
{
    if(!(boards[KING_BOARD] & boards[ENEMY_BOARD])) /* If there is no enemy king */
    {
        return SCORE_MAX;
    }
    else if(!(boards[KING_BOARD] & boards[MACHINE_BOARD])) /* If there is no machine king */
    {
        return SCORE_MIN;
    }
    return 0;
}


/* Get the moves that the opponent can play
 * Returns length of array
 * (HINT: Program as if you were the enemy fighting machine)
 * whom:
 * 1 = machine
 * 0 = enemy
 */
uint8_t get_enemy_legal_moves(move_t *move_arr)
{
    uint8_t counter = 0;

    bitboard full_board = boards[PAWN_BOARD] | boards[KNIGHT_BOARD] | boards[L_ROOK_BOARD] |
                          boards[R_ROOK_BOARD] | boards[KING_BOARD];

    for(uint8_t i = 0; i < ENEMY_BOARD; i++) /* Iterates through each piece board */
    {
        bitboard copy = boards[i] & boards[ENEMY_BOARD], move_board = 0, lsb_board;
        switch(i)
        {
            case PAWN_BOARD: /* Enemy's Pawn */
                while(copy) /* 0 means we have iterated through all the pieces */
                {
                    uint8_t from;
                    lsb_board = copy & (0 - copy); /* Gets first pawn piece */
                    copy &= copy - 1; /* Removes the first pawn piece from copy */
                    from = bitboard_magic_table[(lsb_board * BITBOARD_MAGIC) >> 58]; /* Get index of it */
                    /* Put assembly function here */
                    move_board |= (lsb_board << 7) & 0x0F0F0F0F0F0F0F0F; /* Upper left (With mask) */
                    move_board |= (lsb_board << 9) & 0x1E1E1E1E1E1E1E1E; /* Upper right (With mask) */
                    move_board &= boards[MACHINE_BOARD]; /* Only if pawn can attack */
                    move_board |= (lsb_board << 8) & (~full_board); /* Can move forward if nobody there */

                    while(move_board != 0) /* 0 means we have iterated through all the moves */
                    {
                        move_t new_move = {from, bitboard_magic_table[((move_board & (0 - move_board)) * BITBOARD_MAGIC) >> 58]};
                        move_board &= move_board - 1;
                        move_arr[counter++] = new_move; /* Makes new move */
                    }
                }
                break;

            case KNIGHT_BOARD: /* Enemy's Knight */
                while(copy) /* 0 means we have iterated through all the pieces */
                {
                    uint8_t from;
                    lsb_board = copy & (0 - copy); /* Gets first pawn piece */
                    copy &= copy - 1; /* Removes the first pawn piece from copy */
                    from = bitboard_magic_table[(lsb_board * BITBOARD_MAGIC) >> 58]; /* Get index of it */
                    /* Put assembly function here */
                    move_board |= (lsb_board << 6) & 0x0707070707070707; /* ULL (With mask) */
                    move_board |= (lsb_board << 15) & 0x0F0F0F0F0F0F0F0F; /* UUL (With mask) */
                    move_board |= (lsb_board << 17) & 0x1E1E1E1E1E1E1E1E; /* UUR (With mask) */
                    move_board |= (lsb_board << 10) & 0x1C1C1C1C1C1C1C1C; /* URR (With mask) */
                    move_board &= ~boards[ENEMY_BOARD]; /* Only if empty or enemy */

                    while(move_board != 0) /* 0 means we have iterated through all the moves */
                    {
                        move_t new_move = {from, bitboard_magic_table[((move_board & (0 - move_board)) * BITBOARD_MAGIC) >> 58]};
                        move_board &= move_board - 1;
                        move_arr[counter++] = new_move; /* Makes new move */
                    }
                }
                break;

            case L_ROOK_BOARD: /* Enemy's L-rook */

                while(copy) /* 0 means we have iterated through all the pieces */
                {
                    uint8_t from;
                    lsb_board = copy & (0 - copy); /* Gets first pawn piece */
                    copy &= copy - 1; /* Removes the first pawn piece from copy */
                    from = bitboard_magic_table[(lsb_board * BITBOARD_MAGIC) >> 58]; /* Get index of it */

                    bitboard blockers = full_board & (0x0101010101010101 << BITBOARD_GET_FILE(from));
                    bitboard diff     = blockers - (lsb_board << 1);
                    bitboard changed  = diff ^ full_board;
                    move_board |= changed & (0x0101010101010101 << BITBOARD_GET_FILE(from)) & ~boards[ENEMY_BOARD];

                    blockers = full_board & (0x000000000000001FULL << (BITBOARD_GET_RANK(from) << 3));
                    if(BITBOARD_GET_RANK(from) != 7) /* If it isn't on the top rank */
                    {
                        move_board |= (blockers ^ bitboard_flip( bitboard_flip(blockers) - (bitboard_flip(lsb_board) << 1))) & (0x000000000000001FULL << (BITBOARD_GET_RANK(from) << 3)) & (~boards[ENEMY_BOARD]);
                    }
                    else
                    {
                        move_board |= (blockers ^ (blockers - (lsb_board << 1))) & (0x000000000000001FULL << (BITBOARD_GET_RANK(from) << 3)) & (~boards[ENEMY_BOARD]);
                    }

                    while(move_board != 0) /* 0 means we have iterated through all the moves */
                    {
                        move_t new_move = {from, bitboard_magic_table[((move_board & (0 - move_board)) * BITBOARD_MAGIC) >> 58]};
                        move_board &= move_board - 1;
                        move_arr[counter++] = new_move; /* Makes new move */
                    }
                }
                break;

            case R_ROOK_BOARD: /* Enemy's R-rook */

                while(copy) /* 0 means we have iterated through all the pieces */
                {
                    uint8_t from;
                    lsb_board = copy & (0 - copy); /* Gets first pawn piece */
                    copy &= copy - 1; /* Removes the first pawn piece from copy */
                    from = bitboard_magic_table[(lsb_board * BITBOARD_MAGIC) >> 58]; /* Get index of it */

                    bitboard blockers = full_board & (0x0101010101010101 << BITBOARD_GET_FILE(from));
                    bitboard diff     = blockers - (lsb_board << 1);
                    bitboard changed  = diff ^ full_board;
                    move_board |= changed & (0x0101010101010101 << BITBOARD_GET_FILE(from)) & ~boards[ENEMY_BOARD];

                    blockers = full_board & (0x000000000000001FULL << (BITBOARD_GET_RANK(from) << 3));
                    if(BITBOARD_GET_RANK(from) != 7) /* If it isn't on the top rank */
                    {
                        move_board |= (blockers ^ (blockers - (lsb_board << 1))) & (0x000000000000001FULL << (BITBOARD_GET_RANK(from) << 3)) & (~boards[ENEMY_BOARD]);
                    }
                    else
                    {
                        move_board |= (blockers ^ bitboard_flip( bitboard_flip(blockers) - (bitboard_flip(lsb_board) << 1))) & (0x000000000000001FULL << (BITBOARD_GET_RANK(from) << 3)) & (~boards[ENEMY_BOARD]);
                    }

                    while(move_board != 0) /* 0 means we have iterated through all the moves */
                    {
                        move_t new_move = {from, bitboard_magic_table[((move_board & (0 - move_board)) * BITBOARD_MAGIC) >> 58]};
                        move_board &= move_board - 1;
                        move_arr[counter++] = new_move; /* Makes new move */
                    }
                }
                break;

            case KING_BOARD: /* Enemy's R-rook */

                while(copy) /* 0 means we have iterated through all the pieces */
                {
                    uint8_t from;
                    lsb_board = copy & (0 - copy); /* Gets first pawn piece */
                    copy &= copy - 1; /* Removes the first pawn piece from copy */
                    from = bitboard_magic_table[(lsb_board * BITBOARD_MAGIC) >> 58]; /* Get index of it */

                    move_board |= ((full_board ^ (full_board - (lsb_board << 1))) ^ (full_board ^ bitboard_flip( bitboard_flip(full_board) - (bitboard_flip(lsb_board) << 1)))) & (0x000000000000001FULL << (BITBOARD_GET_RANK(from) << 3)) & (boards[MACHINE_BOARD]);

                    while(move_board != 0) /* 0 means we have iterated through all the moves */
                    {
                        move_t new_move = {from, bitboard_magic_table[((move_board & (0 - move_board)) * BITBOARD_MAGIC) >> 58]};
                        move_board &= move_board - 1;
                        move_arr[counter++] = new_move; /* Makes new move */
                    }
                }
                break;
        }

    }

    return counter;
}


/* Get the moves that the we can play */
uint8_t get_machine_legal_moves(move_t *move_arr)
{
    uint8_t counter = 0;

    bitboard full_board = boards[PAWN_BOARD] | boards[KNIGHT_BOARD] | boards[L_ROOK_BOARD] |
                          boards[R_ROOK_BOARD] | boards[KING_BOARD];

    for(uint8_t i = 0; i < ENEMY_BOARD; i++) /* Iterates through each piece board */
    {
        bitboard copy = boards[i] & boards[MACHINE_BOARD], move_board = 0, lsb_board;
        switch(i)
        {
            case PAWN_BOARD:
                while(copy) /* 0 means we have iterated through all the pieces */
                {
                    uint8_t from;
                    lsb_board = copy & (0 - copy); /* Gets first pawn piece */
                    copy &= copy - 1; /* Removes the first pawn piece from copy */
                    from = bitboard_magic_table[(lsb_board * BITBOARD_MAGIC) >> 58]; /* Get index of it */
                    /* Put assembly function here */
                    move_board |= (lsb_board >> 7) & 0x1E1E1E1E1E1E1E1E; /* Bottom right (With mask) */
                    move_board |= (lsb_board >> 9) & 0x0F0F0F0F0F0F0F0F; /* Bottom left (With mask) */
                    move_board &= boards[ENEMY_BOARD]; /* Only if pawn can attack */
                    move_board |= (lsb_board >> 8) & (~full_board); /* Can move forward if nobody there */

                    while(move_board != 0) /* 0 means we have iterated through all the moves */
                    {
                        move_t new_move = {from, bitboard_magic_table[((move_board & (0 - move_board)) * BITBOARD_MAGIC) >> 58]};
                        move_board &= move_board - 1;
                        move_arr[counter++] = new_move; /* Makes new move */
                    }
                }
                break;

            case KNIGHT_BOARD:
                while(copy) /* 0 means we have iterated through all the pieces */
                {
                    uint8_t from;
                    lsb_board = copy & (0 - copy); /* Gets first pawn piece */
                    copy &= copy - 1; /* Removes the first pawn piece from copy */
                    from = bitboard_magic_table[(lsb_board * BITBOARD_MAGIC) >> 58]; /* Get index of it */
                    /* Put assembly function here */
                    move_board |= (lsb_board >> 10) & 0x0707070707070707; /* DLL (With mask) */
                    move_board |= (lsb_board >> 17) & 0x0F0F0F0F0F0F0F0F; /* DDL (With mask) */
                    move_board |= (lsb_board >> 15) & 0x1E1E1E1E1E1E1E1E; /* DDR (With mask) */
                    move_board |= (lsb_board >> 6) & 0x1C1C1C1C1C1C1C1C; /* DRR (With mask) */
                    move_board &= ~boards[MACHINE_BOARD]; /* Only if empty or enemy */

                    while(move_board != 0) /* 0 means we have iterated through all the moves */
                    {
                        move_t new_move = {from, bitboard_magic_table[((move_board & (0 - move_board)) * BITBOARD_MAGIC) >> 58]};
                        move_board &= move_board - 1;
                        move_arr[counter++] = new_move; /* Makes new move */
                    }
                }
                break;

            case L_ROOK_BOARD: /* Machine's R-rook */

                while(copy) /* 0 means we have iterated through all the pieces */
                {
                    uint8_t from;
                    lsb_board = copy & (0 - copy); /* Gets first pawn piece */
                    copy &= copy - 1; /* Removes the first pawn piece from copy */
                    from = bitboard_magic_table[(lsb_board * BITBOARD_MAGIC) >> 58]; /* Get index of it */

                    /* o ^ reverse( o' - 2s' ) */
                    bitboard blockers = full_board & (0x0101010101010101 << BITBOARD_GET_FILE(from));
                    bitboard diff     = bitboard_flip( bitboard_flip(blockers) - ( bitboard_flip(lsb_board) << 1));
                    bitboard changed  = diff ^ full_board;
                    move_board |= changed & (0x0101010101010101 << BITBOARD_GET_FILE(from)) & ~boards[MACHINE_BOARD];

                    blockers = full_board & (0x000000000000001FULL << (BITBOARD_GET_RANK(from) << 3));
                    if(BITBOARD_GET_RANK(from) != 0) /* If it isn't on the bottom rank */
                    {
                        move_board |= (blockers ^ bitboard_flip( bitboard_flip(blockers) - (bitboard_flip(lsb_board) << 1))) & (0x000000000000001FULL << (BITBOARD_GET_RANK(from) << 3)) & (~boards[MACHINE_BOARD]);
                    }
                    else
                    {
                        move_board |= (blockers ^ (blockers - (lsb_board << 1))) & (0x000000000000001FULL << (BITBOARD_GET_RANK(from) << 3)) & (~boards[MACHINE_BOARD]);
                    }

                    while(move_board != 0) /* 0 means we have iterated through all the moves */
                    {
                        move_t new_move = {from, bitboard_magic_table[((move_board & (0 - move_board)) * BITBOARD_MAGIC) >> 58]};
                        move_board &= move_board - 1;
                        move_arr[counter++] = new_move; /* Makes new move */
                    }
                }
                break;

            case R_ROOK_BOARD: /* Machine's R-rook */

                while(copy) /* 0 means we have iterated through all the pieces */
                {
                    uint8_t from;
                    lsb_board = copy & (0 - copy); /* Gets first pawn piece */
                    copy &= copy - 1; /* Removes the first pawn piece from copy */
                    from = bitboard_magic_table[(lsb_board * BITBOARD_MAGIC) >> 58]; /* Get index of it */

                    /* o ^ reverse( o' - 2s' ) */
                    bitboard blockers = full_board & (0x0101010101010101 << BITBOARD_GET_FILE(from));
                    bitboard diff     = bitboard_flip( bitboard_flip(blockers) - ( bitboard_flip(lsb_board) << 1));
                    bitboard changed  = diff ^ full_board;
                    move_board |= changed & (0x0101010101010101 << BITBOARD_GET_FILE(from)) & ~boards[MACHINE_BOARD];

                    blockers = full_board & (0x000000000000001FULL << (BITBOARD_GET_RANK(from) << 3));
                    if(BITBOARD_GET_RANK(from) != 0) /* If it isn't on the bottom rank */
                    {
                        move_board |= (blockers ^ (blockers - (lsb_board << 1))) & (0x000000000000001FULL << (BITBOARD_GET_RANK(from) << 3)) & (~boards[MACHINE_BOARD]);
                    }
                    else
                    {
                        move_board |= (blockers ^ bitboard_flip( bitboard_flip(blockers) - (bitboard_flip(lsb_board) << 1))) & (0x000000000000001FULL << (BITBOARD_GET_RANK(from) << 3)) & (~boards[MACHINE_BOARD]);
                    }

                    while(move_board != 0) /* 0 means we have iterated through all the moves */
                    {
                        move_t new_move = {from, bitboard_magic_table[((move_board & (0 - move_board)) * BITBOARD_MAGIC) >> 58]};
                        move_board &= move_board - 1;
                        move_arr[counter++] = new_move; /* Makes new move */
                    }
                }
                break;

            case KING_BOARD: /* Machine's R-rook */

                while(copy) /* 0 means we have iterated through all the pieces */
                {
                    uint8_t from;
                    lsb_board = copy & (0 - copy); /* Gets first pawn piece */
                    copy &= copy - 1; /* Removes the first pawn piece from copy */
                    from = bitboard_magic_table[(lsb_board * BITBOARD_MAGIC) >> 58]; /* Get index of it */

                    move_board |= ((full_board ^ (full_board - (lsb_board << 1))) ^ (full_board ^ bitboard_flip( bitboard_flip(full_board) - (bitboard_flip(lsb_board) << 1)))) & (0x000000000000001FULL << (BITBOARD_GET_RANK(from) << 3)) & (boards[ENEMY_BOARD]);

                    while(move_board != 0) /* 0 means we have iterated through all the moves */
                    {
                        move_t new_move = {from, bitboard_magic_table[((move_board & (0 - move_board)) * BITBOARD_MAGIC) >> 58]};
                        move_board &= move_board - 1;
                        move_arr[counter++] = new_move; /* Makes new move */
                    }
                }
                break;
        }

    }

    return counter;
}
