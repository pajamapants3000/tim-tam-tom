/*
 * File   : player_move.h
 * Program: tim-tam-tom
 * Purpose: logic - header
 * Author : Tommy Lincoln <pajamapants3000@gmail.com>
 * License: MIT -- See LICENSE
 */

#include <stdbool.h>
#include <ncurses.h>

// Square representations y(0, 1, 2) X x(0, 1, 2)
#define _0_x_0_ (1 << 0)
#define _0_x_1_ (1 << 1)
#define _0_x_2_ (1 << 2)
#define _1_x_0_ (1 << 3)
#define _1_x_1_ (1 << 4)
#define _1_x_2_ (1 << 5)
#define _2_x_0_ (1 << 6)
#define _2_x_1_ (1 << 7)
#define _2_x_2_ (1 << 8)

// Winning square combinations
#define WIN_ROW0 ((1 << 0) | (1 << 1) | (1 << 2))
#define WIN_ROW1 ((1 << 3) | (1 << 4) | (1 << 5))
#define WIN_ROW2 ((1 << 6) | (1 << 7) | (1 << 8))
#define WIN_COL0 ((1 << 0) | (1 << 3) | (1 << 6))
#define WIN_COL1 ((1 << 1) | (1 << 4) | (1 << 7))
#define WIN_COL2 ((1 << 2) | (1 << 5) | (1 << 8))
#define WIN_DGDR ((1 << 0) | (1 << 4) | (1 << 8))
#define WIN_DGUR ((1 << 2) | (1 << 4) | (1 << 6))
// array of all possible winning sets
#define WINS { WIN_ROW0, WIN_ROW1, WIN_ROW2, WIN_COL0, WIN_COL1, WIN_COL2, \
    WIN_DGDR, WIN_DGUR }
#define NUM_WINTYPES 8

// squares is a bitmask that is constructed by a set of squares on the board
typedef unsigned short squares;
// Player manages all attributes for a given player
struct player_struct
{
    char* name;
    bool mark;             // 0 for O, 1 for X;
    bool is_my_turn;
    unsigned int num_turns_taken;
    squares markers;
};
typedef struct player_struct Player;

// Header/*{{{*/
/*********************************************************************
 * is_valid_move: checks last_move against previously taken moves    *
 *                and returns false if the move was taken already    *
 *********************************************************************/
bool is_valid_move(Player* playeX, Player* playerO, squares last_move);
/*********************************************************************
 * update_turns: updates the players' data structs to account for    *
 *               the latest move. Adds argument last_move to the     *
 *               bitmask indicating all moves taken for the player   *
 *               who made the move, then increments that player's    *
 *               num_turns_taken, and switches each player's         *
 *               is_my_turn value to indicate that it is now the     *
 *               other player's turn.
 *********************************************************************/
void update_turns(Player* playerX, Player* playerO, squares last_move);
/*********************************************************************
 * square_dec_to_bin: takes a single integer (value from 0 to 8      *
 *                    inclusive); converts this value to the binary  *
 *                    mask representing this square and returns it   *
 *********************************************************************/
squares square_dec_to_bin(int choice);
/*********************************************************************
 * player_wins: checks validated moves by player for winning set     *
 *********************************************************************/
bool player_wins(Player* player);
/*********************************************************************
 * reached_stalemate: checks validated moves by both players to      *
 *                    determine if all possible moves have been made *
 *********************************************************************/
bool reached_stalemate(Player* playerX, Player* playerO);
/*}}}*/
