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


// *gameboard dimensions*
// defining macros
#define BOARD_NUM_SQUARES_X 3
#define BOARD_NUM_SQUARES_Y 3
#define SQUARE_DIMY  8
#define SQUARE_DIMX 16
// derived macros
#define BOARD_H (3 * SQUARE_DIMY + 1)
#define BOARD_W (3 * SQUARE_DIMX + 1)
#define BOARD_Y (TITLE_WIN_Y + TITLE_WIN_H)
#define BOARD_X (((COLS  - (BOARD_W + PANE_WIN_W)) / 2) + PANE_WIN_W)
#define SQUARE_MIDX (SQUARE_DIMX / 2)
#define SQUARE_MIDY (SQUARE_DIMY / 2)
#define STEP_ALONG_CENTERS_Y(i) (SQUARE_MIDY + \
        (SQUARE_DIMY * (i / BOARD_NUM_SQUARES_Y)))
#define STEP_ALONG_CENTERS_X(i) (SQUARE_MIDX + \
        (SQUARE_DIMX * (i % BOARD_NUM_SQUARES_X)))
// *                    *

// message window constants
#define MSG_WIN_H  8
#define MSG_WIN_W 40
#define MSG_WIN_Y ((LINES - MSG_WIN_H) / 2)
#define MSG_WIN_X ((COLS  - MSG_WIN_W) / 2)
#define MESSAGE { "Hello, and welcome to the message!",\
                  "This is line two of the message.",\
                  ""}
#define MSG_YSTART 2

// pane window constants
#define PANE_WIN_H BOARD_H
#define PANE_WIN_W (80 - BOARD_W)
#define PANE_WIN_Y BOARD_Y
#define PANE_WIN_X (BOARD_X - PANE_WIN_W)
#define NUM_PLAYERS 2
#define PLYRNFO_NUM_LINES 2
#define PLYRNFO_TB_PADDING 4
// Number of lines devoted to each player in the side info pane
//+number of lines of text, double spaced, padded on top and bottom
#define PLYR_TEXT_SECTION (2 * PLYRNFO_NUM_LINES + 2 * PLYRNFO_TB_PADDING)

// title window
#define TITLE_WIN_H 10
#define TITLE_WIN_W (BOARD_W + PANE_WIN_W)
#define TITLE_WIN_Y ((LINES - (BOARD_H + TITLE_WIN_H)) / 2)
#define TITLE_WIN_X PANE_WIN_X
#define TITLE_YSTART 2
#define TITLE { "Hello, and welcome to the title!",\
                  "This is line two of the title.",\
                  ""}

#define NUL '\0'
#define INIT_CURS initscr(); cbreak(); noecho(); start_color(); refresh()
#define COLOR_PAIR_DEFAULT     1
#define COLOR_PAIR_NUM_MYTURN  2
#define COLOR_PAIR_NUM_WAITING 3
#define COLOR_MYTURN_F  COLOR_MAGENTA
#define COLOR_MYTURN_B  COLOR_BLUE
#define COLOR_WAITING_F COLOR_WHITE
#define COLOR_WAITING_B COLOR_BLACK
#define COLOR_DEFAULT_F COLOR_WHITE
#define COLOR_DEFAULT_B COLOR_BLACK

#define NAMLEN 20
#define INIT_NCURSES initscr(); cbreak(); noecho(); refresh()

#define KB_ENTER 10     // KEY_ENTER doesn't work for some reason
#define NAMLEN   20     // maximum length for name (re-label?)

typedef unsigned short squares;     // at least 2 bytes; need 9 bits to
                                    //+represent the 9 possible squares in any
                                    //+possible combination
struct player_struct
{
    char name[NAMLEN];
    bool mark;             // 0 for O (playerO), 1 for X (playerX);
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

