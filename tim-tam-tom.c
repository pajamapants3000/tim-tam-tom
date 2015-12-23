/*
 * File   : tim-tam-tom.c
 * Program: tim-tam-tom
 * Purpose: A tic-tac-toe game!
 * Author : Tommy Lincoln <pajamapants3000@gmail.com>
 * License: MIT - See LICENSE
 */

#include <stdbool.h>
#include <stdlib.h>
#include <ncurses.h>
#include <form.h>


#define INIT_NCURSES initscr(); cbreak(); noecho(); refresh()

#define KB_ENTER 10     // KEY_ENTER doesn't work for some reason
#define NAMLEN   20     // maximum length for name (re-label?)

typedef unsigned short squares;     // at least 2 bytes; need 9 bits to
                                    //+represent the 9 possible squares in any
                                    //+possible combination
struct player_struct
{
    char name[NAMLEN];
    bool mark;             // 0 for O, 1 for X;
    bool is_my_turn;
    squares markers;
};
typedef struct player_struct Player;

struct Prompt {
    FIELD** fields;
    FORM* form;
    WINDOW* form_win;
    WINDOW* form_subwin;
};

void draw_board(int board_ystart, int board_xstart, bool numbered);
struct Prompt* draw_form();

int main(void)
{
    Player *playerX, *playerO;

    return 0;
}

