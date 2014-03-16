#include "Varion.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    unsigned char game_over = 0;

    setup();
    print_board();
    while(!game_over)
    {
        get_move();

        if(game_over = check_game_over())
        {
            continue;
        }

ai_first:

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
    pawn_board     = 0x0000700000700000;
    knight_board   = 0x0000880000880000;
    l_rook_board   = 0x0800000000000008;
    r_rook_board   = 0x8000000000000080;
    king_board     = 0x2000000000000020;
    enemy_board    = 0x0000000000F800A8; //ENDED HERE
}


/* Print current board state */
void print_board(void)
{
    bitboard full_board = pawn_board | knight_board | l_rook_board |
                          r_rook_board | king_board; 

    for(int i = 0; i < GAME_BOARD_HEIGHT; i++)
    {
        printf("\t%d", i);

        for(int j = 0; j < GAME_BOARD_WIDTH; j++)
        {

            if(BITBOARD_HAS_PIECE(full_board, j, i))
            {
                printf(" ");

                if(BITBOARD_HAS_PIECE(enemy_board, j, i))
                { printf("\x1b[32m"); }
                else
                { printf("\x1b[31m"); }

                if(BITBOARD_HAS_PIECE(pawn_board, j, i))
                { printf("P"); } 
                else if(BITBOARD_HAS_PIECE(knight_board, j, i))
                { printf("N"); } 
                else if(BITBOARD_HAS_PIECE(l_rook_board, j, i))
                { printf("L"); } 
                else if(BITBOARD_HAS_PIECE(r_rook_board, j, i))
                { printf("R"); } 
                else
                { printf("K"); }

                printf("\x1b[37m ");
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
    char move_not_inputted = 1; /* Used as a boolean */
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
            move_perform(input_move);
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
    return 1; // Sets game-over. Will be implemented later.
}


/* Get the moves that the opponent can play */
uint8_t get_enemy_legal_moves(move_t *move_arr) /* Depth tells us where to put the moves list */
{
    uint8_t counter = 0;
    bitboard full_board = pawn_board | knight_board | l_rook_board |
                          r_rook_board | king_board;

    for(uint8_t i = 0; i < GAME_BOARD_HEIGHT; i++)
    {
        for(uint8_t j = 0; j < GAME_BOARD_WIDTH; j++)
        {
            if(BITBOARD_HAS_PIECE(enemy_board, j, i))
            {
               if(BITBOARD_HAS_PIECE(pawn_board, j, i))
               {
                  if(i > 0 && j < 4 && BITBOARD_HAS_PIECE(full_board ^ enemy_board, j+1, i-1))
                  {
                      printf("Found an upright move: %d, %d\n", j, i);
                      move_t new_move = {j, i, j+1, i-1};
                      move_arr[counter++] = new_move;
                  }
                  if(i > 0 && j > 0 && BITBOARD_HAS_PIECE(full_board ^ enemy_board, j-1, i-1))
                  {
                      printf("Found an upleft move: %d, %d\n", j, i);
                      move_t new_move = {j, i, j-1, i-1};
                      move_arr[counter++] = new_move;
                  }
                  if(i > 0 && !BITBOARD_HAS_PIECE(full_board, j, i-1))
                  {
                      printf("Found an up move: %d, %d\n", j, i);
                      move_t new_move = {j, i, j, i-1};
                      move_arr[counter++] = new_move;
                  }
               }
            }
        }
    }
    return counter;
}
