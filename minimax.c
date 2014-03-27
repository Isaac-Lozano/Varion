#include "minimax.h"
#include "Varion.h"
#include "moves.h"
#include <limits.h>
#include <stdio.h>

unsigned char enemy_threatened = 0, machine_threatened = 0;

int minimax_min(uint8_t depth, int parent_best, int parent_second_best)
{
    int best = SCORE_MAX, second_best = SCORE_MAX, score;
    uint8_t move_len;
    move_t move_arr[MAX_MOVES], best_move;

    int game_over;
    if((game_over = check_game_over()))
    {
        node_count++;
        if(depth == 1)
        {
            best_move.from = 63;
            best_move.to = 63;
            rejection_moves[rejection_len++] = best_move;
//            printf("O1\n");
        }
        return game_over - depth;
    }

    if(depth == MAX_DEPTH) return (minimax_eval());

    move_len = get_enemy_legal_moves(move_arr);

    for(uint8_t i = 0; i < move_len; i++)
    {
        move_perform(&move_arr[i]);
        score = minimax_max(depth+1, best, second_best);

//        if(depth == 1)
//        printf("| |Checking move [%d%d%d%d]; Score = %d\n", BITBOARD_GET_FILE(move_arr[i].from),
//                        BITBOARD_GET_RANK(move_arr[i].from),
//                        BITBOARD_GET_FILE(move_arr[i].to),
//                        BITBOARD_GET_RANK(move_arr[i].to), score);

        if(score < second_best)
        {
            if(score < best)
            {
                second_best = best;
                best = score;
                if(depth == 1)
                {
                    best_move = move_arr[i];
//                    printf("| |\\\n| | |Turns out that it was the best move.\n");
                }
            }
            second_best = score;
        }
        if(second_best < parent_second_best)
        {
            move_undo(&move_arr[i]);
            if(depth == 1)
            {
                rejection_moves[rejection_len++] = best_move;
//                printf("O2\n");
            }
            return second_best;
        }

        move_undo(&move_arr[i]);
    }

    if(depth == 1)
    {
        rejection_moves[rejection_len++] = best_move;
//        printf("O3\n");
    }
    return second_best;
}


int minimax_max(uint8_t depth, int parent_best, int parent_second_best)
{
    int best = SCORE_MIN, second_best = SCORE_MIN, score;
    uint8_t move_len;
    move_t move_arr[MAX_MOVES];

    int game_over;
    if((game_over = check_game_over()))
    {
        node_count++;
        return game_over + depth;
    }

    if(depth == MAX_DEPTH) return (minimax_eval());

    move_len = get_machine_legal_moves(move_arr);

    for(uint8_t i = 0; i < move_len; i++)
    {
        move_perform(&move_arr[i]);
        score = minimax_min(depth+1, best, second_best);

        if(score >= second_best)
        {
            if(score >= best)
            {
                second_best = best;
                best = score;
            }
            second_best = score;
        }
        if(second_best >= parent_second_best)
        {
            move_undo(&move_arr[i]);
            return second_best;
        }

        move_undo(&move_arr[i]);
    }

    return second_best;
}


inline int minimax_eval()
{
    node_count++;

    bitboard machine_copy = boards[MACHINE_BOARD], enemy_copy = boards[ENEMY_BOARD];
    uint8_t machine_count, enemy_count;
    
    while(machine_copy)
    {
        machine_copy &= machine_copy - 1;
        machine_count++;
    }

    while(enemy_copy)
    {
        enemy_copy &= enemy_copy - 1;
        enemy_count++;
    }

    return machine_count - enemy_count;
}
