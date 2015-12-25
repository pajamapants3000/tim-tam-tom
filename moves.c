/*
 * File   : moves.c
 * Program: tim-tam-tom
 * Purpose: logic - libraries
 * Author : Tommy Lincoln <pajamapants3000@gmail.com>
 * License: MIT -- See LICENSE
 */

#include <stdbool.h>
#include "moves.h"

bool is_valid_move(Player* playerX, Player* playerO, squares last_move)/*{{{*/
{
    return ((playerX->markers | playerO->markers) & last_move) == 0;
}
/*}}}*/
void update_turns(Player* playerX, Player* playerO, squares last_move)/*{{{*/
{
    if (playerX->is_my_turn)
        playerX->markers |= last_move;
    else
        playerO->markers |= last_move;

    playerX->is_my_turn = !(playerX->is_my_turn);
    playerO->is_my_turn = !(playerO->is_my_turn);
}
/*}}}*/
squares square_dec_to_bin(int choice)/*{{{*/
{
    return (1 << choice);
}
/*}}}*/
bool player_wins(Player* player)/*{{{*/
{
    squares winning_combinations[] = WINS;

    for (int i = 0; i < NUM_WINTYPES; ++i)
    {
        if ((winning_combinations[i] & player->markers)
                == winning_combinations[i])
            return true;
    }
    return false;
}
/*}}}*/
bool reached_stalemate(Player* playerX, Player* playerO)/*{{{*/
{
    return (playerX->markers | playerO->markers) == ((1 << 9) - 1);
}
/*}}}*/
