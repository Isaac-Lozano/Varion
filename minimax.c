#include "minimax.h"
#include "Varion.h"
#include "moves.h"
#include <limits.h>

int minimax_min(uint8_t depth, int parent_best)
{
    int best = INT_MAX, score;
    uint8_t move_len;
    move_t move_arr[MAX_MOVES], best_move;

    if(check_game_over())
    {
        node_count++;
        return INT_MAX - depth;
    }

    if(depth == MAX_DEPTH) return (minimax_eval());

    move_len = get_enemy_legal_moves(move_arr);

    for(uint8_t i = 0; i < move_len; i++)
    {
        move_perform(&move_arr[i]);
        score = minimax_max(depth+1, best);

        if(score < best)
        {
            best = score;
            best_move = move_arr[i];
        }
        if(best < parent_best)
        {
            move_undo(&move_arr[i]);
            if(depth == 1)
            {
                rejection_moves[rejection_len++] = best_move;
            }
            return best;
        }

        move_undo(&move_arr[i]);
    }

    if(depth == 1) rejection_moves[rejection_len++] = best_move;
    return best;
}


int minimax_max(uint8_t depth, int parent_worst)
{
    int best = INT_MIN, score;
    uint8_t move_len;
    move_t move_arr[MAX_MOVES];

    if(check_game_over())
    {
        node_count++;
        return INT_MIN + depth;
    }

    if(depth == MAX_DEPTH) return (minimax_eval());

    move_len = get_machine_legal_moves(move_arr);

    for(uint8_t i = 0; i < move_len; i++)
    {
        move_perform(&move_arr[i]);
        score = minimax_min(depth+1, best);

        if(score > best) best = score;
        if(best > parent_worst)
        {
            move_undo(&move_arr[i]);
            return best;
        }

        move_undo(&move_arr[i]);
    }

    return best;
}


inline int minimax_eval()
{
    node_count++;


    return 0;
}
