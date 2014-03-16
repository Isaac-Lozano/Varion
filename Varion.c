#include "Varion.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    unsigned char game_over = 0;

    setup();
    print_board();

    /* TODO: Add who will go first. (goto ai_first if ai == first) */

    while(!game_over)
    {
        get_move();

        if((game_over = check_game_over()))
        {
            continue;
        }

//ai_first:

        make_move();

        game_over = check_game_over();
    }

    return 0;
}


/* Set up default board */
void setup(void)
{
    /* Every two hex corresponds to a row.*/
    /*         ROW =   0 1 2 3 4 5 6 7    */
    boards[PAWN_BOARD]     = 0x0000700000700000;
    boards[KNIGHT_BOARD]   = 0x0000880000880000;
    boards[L_ROOK_BOARD]   = 0x0800000000000008;
    boards[R_ROOK_BOARD]   = 0x8000000000000080;
    boards[KING_BOARD]     = 0x2000000000000020;
    boards[ENEMY_BOARD]    = 0x0000000000F800A8; //ENDED HERE
}


/* Print current board state */
void print_board(void)
{
    bitboard full_board = boards[PAWN_BOARD] | boards[KNIGHT_BOARD] | boards[L_ROOK_BOARD] |
                          boards[R_ROOK_BOARD] | boards[KING_BOARD];

    for(int i = 0; i < GAME_BOARD_HEIGHT; i++)
    {
        printf("\t%d", i);

        for(int j = 0; j < GAME_BOARD_WIDTH; j++)
        {

            if(BITBOARD_HAS_PIECE(full_board, j, i))
            {
                printf(" ");

                if(BITBOARD_HAS_PIECE(boards[ENEMY_BOARD], j, i))
                { printf(ANSI_ESCAPE_RED); }
                else
                { printf(ANSI_ESCAPE_GREEN); }

                if(BITBOARD_HAS_PIECE(boards[PAWN_BOARD], j, i))
                { printf("P"); } 
                else if(BITBOARD_HAS_PIECE(boards[KNIGHT_BOARD], j, i))
                { printf("N"); } 
                else if(BITBOARD_HAS_PIECE(boards[L_ROOK_BOARD], j, i))
                { printf("L"); } 
                else if(BITBOARD_HAS_PIECE(boards[R_ROOK_BOARD], j, i))
                { printf("R"); } 
                else
                { printf("K"); }

                printf(ANSI_ESCAPE_WHITE);
                printf(" ");
            }
            else
            {
                printf(" - ");
            }
        }

        printf("\n");
    }

    printf("\t  0  1  2  3  4\n");
}


/* Get move from opponent */
void get_move(void)
{
    uint8_t move_len;
    char move_str[5]; /* I will make sure that this does not overflow */
    move_t move_arr[MAX_MOVES], input_move;

    move_len = get_enemy_legal_moves(move_arr);
    move_print_arr(move_arr, move_len);

enter_move:
    printf("Enter move: ");
    for(uint8_t i = 0; i < 4; i++)
    {
        move_str[i] = getc(stdin);
    }
    move_str[4] = '\0';

    int ch;
    while((ch = fgetc(stdin)) != '\n' && ch != EOF); /* Flush stdin */

    for(uint8_t i = 0; i < 4; i++)
    {
        if(move_str[i] < '8' && move_str[i] >= '0')
        {
            input_move.x[i] = move_str[i] - '0';
        }
        else
        {
            printf("Invalid move.\n");
            goto enter_move;
        }
    }

    for(uint8_t i = 0; i < move_len; i++)
    {
        if(MOVE_EQUAL(input_move, move_arr[i]))
        {
            move_perform(&input_move);
            print_board();
            return;
        }
    }

    printf("Bad move inputted.\n");
    goto enter_move;
}


/* Make move against opponent */
void make_move(void)
{
}


/* Check for a game-over state */
unsigned char check_game_over(void)
{
    return ((boards[KING_BOARD] & boards[ENEMY_BOARD]) == 0) ||
           ((boards[KING_BOARD] & ~(boards[ENEMY_BOARD])) == 0);
}


/* Get the moves that the opponent can play */
uint8_t get_enemy_legal_moves(move_t *move_arr) /* Depth tells us where to put the moves list */
{
    uint8_t counter = 0;
    bitboard full_board = boards[PAWN_BOARD] | boards[KNIGHT_BOARD] | boards[L_ROOK_BOARD] |
                          boards[R_ROOK_BOARD] | boards[KING_BOARD];

    for(uint8_t i = 0; i < GAME_BOARD_HEIGHT; i++)
    {
        for(uint8_t j = 0; j < GAME_BOARD_WIDTH; j++)
        {
            if(BITBOARD_HAS_PIECE(boards[ENEMY_BOARD], j, i))
            {
               if(BITBOARD_HAS_PIECE(boards[PAWN_BOARD], j, i))
               {
                  if(i > 0 && j < 4 && BITBOARD_HAS_PIECE(full_board ^ boards[ENEMY_BOARD], j+1, i-1))
                  {
                      move_t new_move = {{j, i, j+1, i-1}};
                      move_arr[counter++] = new_move;
                  }
                  if(i > 0 && j > 0 && BITBOARD_HAS_PIECE(full_board ^ boards[ENEMY_BOARD], j-1, i-1))
                  {
                      move_t new_move = {{j, i, j-1, i-1}};
                      move_arr[counter++] = new_move;
                  }
                  if(i > 0 && !BITBOARD_HAS_PIECE(full_board, j, i-1))
                  {
                      move_t new_move = {{j, i, j, i-1}};
                      move_arr[counter++] = new_move;
                  }
               }
            }
        }
    }
    return counter;
}
