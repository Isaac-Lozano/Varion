#include "Varion.h"
#include "minimax.h"
#include "moves.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>

uint64_t node_count;

int main(void)
{
    unsigned char game_over = 0;

    setup();
    bitboard_print();

    /* TODO: Add who will go first. (goto ai_first if ai == first) */

    while(!game_over)
    {
        get_move();
        bitboard_print();
        node_count = 0;

        if((game_over = check_game_over()))
        {
            continue;
        }

//ai_first:

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
    boards[PAWN_BOARD]     = 0x0000700000700000;
    boards[KNIGHT_BOARD]   = 0x0000880000880000;
    boards[L_ROOK_BOARD]   = 0x0800000000000008;
    boards[R_ROOK_BOARD]   = 0x8000000000000080;
    boards[KING_BOARD]     = 0x2000000000000020;
    boards[ENEMY_BOARD]    = 0x0000000000F800A8;
}



/* Get move from opponent */
void get_move(void)
{
    uint8_t move_len;
    char move_str[5]; /* I will make sure that this does not overflow */
    move_t move_arr[MAX_MOVES], input_move;

    move_len = get_enemy_legal_moves(move_arr);
    move_print_arr(move_arr, move_len);

enter_command:
    printf("Enter move: ");
    for(uint8_t i = 0; i < 4; i++)
    {
        move_str[i] = fgetc(stdin);
    }
    move_str[4] = '\0';

    int ch;
    while((ch = fgetc(stdin)) != '\n' && ch != EOF); /* Flush stdin */

    for(uint8_t i = 0; i < 4; i++) /* Check if the string is valid */
    {
        if(move_str[i] < '8' && move_str[i] >= '0')
        {
            input_move.x[i] = move_str[i] - '0';
        }
        else
        {
            printf("Invalid move.\n");
            goto enter_command;
        }
    }

    for(uint8_t i = 0; i < move_len; i++) /* Check if the string is valid */
    {
        if(MOVE_EQUAL(input_move, move_arr[i]))
        {
            for(uint8_t j = 0; j < rejection_len; j++)
            {
                if(MOVE_EQUAL(input_move, rejection_moves[j]))
                {
                    printf("I reject that move. Pick another one.\n");
                    goto enter_command;
                }
            }
            move_perform(&input_move);
            printf("cap_flags = %d\n", input_move.cap_flags);
            return;
        }
    }

    printf("Bad move inputted.\n");
}


/* Make move against opponent */
int make_move(void)
{
    int best = INT_MIN, next_best = INT_MIN, depth = 0, score;
    uint8_t move_len;
    move_t move_arr[MAX_MOVES], best_move, next_best_move;

    rejection_len = 0;
    move_len = get_machine_legal_moves(move_arr);

    for(uint8_t i = 0; i < move_len; i++)
    {
        move_perform(&move_arr[i]);
        score = minimax_min(depth+1, best);
        if(score > next_best)
        {
            if(score > best)
            {
                next_best = best;
                best = score;
                next_best_move = best_move;
                best_move = move_arr[i];
            }
            else
            {
                next_best = score;
                next_best_move = move_arr[i];
            }
        }
        move_undo(&move_arr[i]);
    }

    printf("My move is [%d%d%d%d], Do you Accept [Y/n] ", best_move.x[0], best_move.x[1], best_move.x[2], best_move.x[3]);

    int ch = getc(stdin);

    if(ch == 'N' || ch == 'n')
    {
        move_perform(&next_best_move);
        printf("My move is [%d%d%d%d], then.\n", next_best_move.x[0], next_best_move.x[1], next_best_move.x[2], next_best_move.x[3]);
        move_print_arr(rejection_moves, rejection_len);
        return best;
    }

    move_perform(&best_move);
    move_print_arr(rejection_moves, rejection_len);
    return best;
}


/* Check for a game-over state */
/* Returns 1 if game_over */
unsigned char check_game_over(void)
{
    return ((boards[KING_BOARD] & boards[ENEMY_BOARD]) == 0) ||
           ((boards[KING_BOARD] & ~(boards[ENEMY_BOARD])) == 0);
}


/* Get the moves that the opponent can play */
uint8_t get_enemy_legal_moves(move_t *move_arr)
{
    uint8_t counter = 0;
    bitboard full_board = boards[PAWN_BOARD] | boards[KNIGHT_BOARD] | boards[L_ROOK_BOARD] |
                          boards[R_ROOK_BOARD] | boards[KING_BOARD];

    for(uint8_t y = 0; y < GAME_BOARD_HEIGHT; y++)
    {
        for(uint8_t x = 0; x < GAME_BOARD_WIDTH; x++)
        {
            if(BITBOARD_HAS_PIECE(boards[ENEMY_BOARD], x, y))
            {
               if(BITBOARD_HAS_PIECE(boards[PAWN_BOARD], x, y))
               {
                  if(y > 0 && x < 4 && BITBOARD_HAS_PIECE(full_board ^ boards[ENEMY_BOARD], x+1, y-1))
                  {
                      move_t new_move = {{x, y, x+1, y-1}};
                      move_arr[counter++] = new_move;
                  }
                  if(y > 0 && x > 0 && BITBOARD_HAS_PIECE(full_board ^ boards[ENEMY_BOARD], x-1, y-1))
                  {
                      move_t new_move = {{x, y, x-1, y-1}};
                      move_arr[counter++] = new_move;
                  }
                  if(y > 0 && !BITBOARD_HAS_PIECE(full_board, x, y-1))
                  {
                      move_t new_move = {{x, y, x, y-1}};
                      move_arr[counter++] = new_move;
                  }
               }
               else if(BITBOARD_HAS_PIECE(boards[KNIGHT_BOARD], x, y))
               {
                   if(y > 0 && x > 1 && !BITBOARD_HAS_PIECE(full_board & boards[ENEMY_BOARD], x-2, y-1))
                   {
                       move_t new_move = {{x, y, x-2, y-1}};
                       move_arr[counter++] = new_move;
                   }
                   if(y > 1 && x > 0 && !BITBOARD_HAS_PIECE(full_board & boards[ENEMY_BOARD], x-1, y-2))
                   {
                       move_t new_move = {{x, y, x-1, y-2}};
                       move_arr[counter++] = new_move;
                   }
                   if(y > 1 && x < 4 && !BITBOARD_HAS_PIECE(full_board & boards[ENEMY_BOARD], x+1, y-2))
                   {
                       move_t new_move = {{x, y, x+1, y-2}};
                       move_arr[counter++] = new_move;
                   }
                   if(y > 0 && x < 3 && !BITBOARD_HAS_PIECE(full_board & boards[ENEMY_BOARD], x+2, y-1))
                   {
                       move_t new_move = {{x, y, x+2, y-1}};
                       move_arr[counter++] = new_move;
                   }
               }
            }
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
    bitboard machine_board = full_board ^ boards[ENEMY_BOARD];

    for(uint8_t y = 0; y < GAME_BOARD_HEIGHT; y++)
    {
        for(uint8_t x = 0; x < GAME_BOARD_WIDTH; x++)
        {
            if(BITBOARD_HAS_PIECE(machine_board, x, y))
            {
               if(BITBOARD_HAS_PIECE(boards[PAWN_BOARD], x, y))
               {
                  if(y < 7 && x < 4 && BITBOARD_HAS_PIECE(boards[ENEMY_BOARD], x+1, y+1))
                  {
                      move_t new_move = {{x, y, x+1, y+1}};
                      move_arr[counter++] = new_move;
                  }
                  if(y > 0 && x > 0 && BITBOARD_HAS_PIECE(boards[ENEMY_BOARD], x-1, y+1))
                  {
                      move_t new_move = {{x, y, x-1, y+1}};
                      move_arr[counter++] = new_move;
                  }
                  if(y > 0 && !BITBOARD_HAS_PIECE(full_board, x, y+1))
                  {
                      move_t new_move = {{x, y, x, y+1}};
                      move_arr[counter++] = new_move;
                  }
               }
               else if(BITBOARD_HAS_PIECE(boards[KNIGHT_BOARD], x, y))
               {
                   if(y < 7 && x > 1 && !BITBOARD_HAS_PIECE(machine_board, x-2, y+1))
                   {
                       move_t new_move = {{x, y, x-2, y+1}};
                       move_arr[counter++] = new_move;
                   }
                   if(y < 6 && x > 0 && !BITBOARD_HAS_PIECE(machine_board, x-1, y+2))
                   {
                       move_t new_move = {{x, y, x-1, y+2}};
                       move_arr[counter++] = new_move;
                   }
                   if(y < 6 && x < 4 && !BITBOARD_HAS_PIECE(machine_board, x+1, y+2))
                   {
                       move_t new_move = {{x, y, x+1, y+2}};
                       move_arr[counter++] = new_move;
                   }
                   if(y < 7 && x < 3 && !BITBOARD_HAS_PIECE(machine_board, x+2, y+1))
                   {
                       move_t new_move = {{x, y, x+2, y+1}};
                       move_arr[counter++] = new_move;
                   }
               }
            }
        }
    }
    return counter;
}
