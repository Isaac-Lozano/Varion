#include "Varion.h"
//#include "moves.h"

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
            char piece_str[10];

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

    printf("\t  A  B  C  D  E\n");
}


/* Get move from opponent */
void get_move(void)
{
//    get_enemy_legal_moves();
//    print_moves(0);
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
