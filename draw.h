/*
 * File   : draw_board.h
 * Program: tim-tam-tom
 * Purpose: presentation macros, functions, and structures - header
 * Author : Tommy Lincoln <pajamapants3000@gmail.com>
 * License: MIT -- See LICENSE
 * Notes  : Link with -lm
 */

#include <ncurses.h>
#include "moves.h"


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

// message window
#define MSG_WIN_H  8
#define MSG_WIN_W 40
#define MSG_WIN_Y ((LINES - MSG_WIN_H) / 2)
#define MSG_WIN_X ((COLS  - MSG_WIN_W) / 2)
#define MESSAGE { "Hello, and welcome to the message!",\
                  "This is line two of the message.",\
                  ""}
#define MSG_YSTART 2

// pane window
#define PANE_WIN_H BOARD_H
#define PANE_WIN_W (80 - BOARD_W)
#define PANE_WIN_Y BOARD_Y
#define PANE_WIN_X (BOARD_X - PANE_WIN_W)
#define NUM_PLAYERS 2
#define PLYRNFO_NUM_LINES 2
#define PLYRNFO_TB_PADDING 2
// Number of lines devoted to each player in the side info pane
// number of lines of text, double spaced, padded on top and bottom
#define PLYR_TEXT_SECTION (2 * PLYRNFO_NUM_LINES + 2 * PLYRNFO_TB_PADDING)
#define PLYR_TEXT_HDR (player->mark ? "Player 1 - X's" : "Player 2 - O's")

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
#define COLOR_PAIR_NUM_DEFAULT 1
#define COLOR_PAIR_NUM_MYTURN  2
#define COLOR_PAIR_NUM_WAITING 3
#define COLOR_PAIR_DEFAULT COLOR_PAIR(COLOR_PAIR_NUM_DEFAULT)
#define COLOR_PAIR_MYTURN  COLOR_PAIR(COLOR_PAIR_NUM_MYTURN)
#define COLOR_PAIR_WAITING COLOR_PAIR(COLOR_PAIR_NUM_WAITING)
#define COLOR_MYTURN_F  COLOR_MAGENTA
#define COLOR_MYTURN_B  COLOR_BLUE
#define COLOR_WAITING_F COLOR_WHITE
#define COLOR_WAITING_B COLOR_BLACK
#define COLOR_DEFAULT_F COLOR_WHITE
#define COLOR_DEFAULT_B COLOR_BLACK

#define NAME_MAXLEN 20

/*********************************************************************
 * draw_board: draws the tic-tac-toe 3x3 board, with optional        *
 *             numbering of the squares. The function, in its        *
 *             present implementation, is hardcoded with values      *
 *             supplied by various macros, all prefixed BOARD_       *
 *             Its window, and therefore its dimensions, are defined *
 *             outside the function and passed in, to allow for      *
 *             persistence of the WINDOW state in the calling        *
 *             function (probably main).                             *
 *********************************************************************/
void draw_board(WINDOW* board, bool numbered);

/*********************************************************************
 * draw_numbers: draws numbers in the center of each square on the   *
 *               3x3 board contained in the WINDOW* board passed in. *
 *********************************************************************/
void draw_numbers(WINDOW* board);

/*********************************************************************
 * draw_msg_simple: displays a message; takes the window, the line   *
 *                  on which to place the first line of the message, *
 *                  and the (possibly multi-line) message as         *
 *                  arguments in that order; Also uses the           *
 *                  COLOR_DEFAULT macro.                             *
 *********************************************************************/
void draw_msg_simple(WINDOW* msg_win, int msg_y, const char** message);

/*********************************************************************
 * draw_pane: Shows information and current status of the game,      *
 *            displayed in the WINDOW* pane_win passed as first arg; *
 *            Highlights the player whose turn it is; Depends on     *
 *            various hardcoded macros with PANE_ prefix, as well as *
 *            several macros with COLOR_ prefix.                     *
 *********************************************************************/
void draw_pane(WINDOW* pane_win, const Player* playerX, const Player* playerO);

/*********************************************************************
 * draw_msg: A nice function for displaying a message, possibly in   *
 *           color! Centers the message vertically and center-       *
 *           justifies each line. Fills the entire window with       *
 *           color. Depends only on <ncurses.h> and my function      *
 *           print_xcenter_w_pad. Also requires color to be          *
 *           initialized. Takes window, message, and color as args.  *
 *********************************************************************/
void draw_msg(WINDOW* msg_win, const char** message, chtype color);

/*********************************************************************
 * print_xcenter: prints a center-justified string on a single line; *
 *                does so on the y_pos'th line of window win, in     *
 *                color color. Depends on <ncurses.h>, with color    *
 *                initialized; also <string.h>.
 *********************************************************************/
void print_xcenter(WINDOW* win, int y_pos, const char* string, chtype color);

/*********************************************************************
 * print_xcenter_w_pad: Calls print_xcenter twice: once with blanks  *
 *                      in a given width to establish background     *
 *                      color, and a second time to print text; both *
 *                      outputs are center-justified; takes the same *
 *                      arguments as print_xcenter, but with a set   *
 *                      width added to define width of output.
 *********************************************************************/
void print_xcenter_w_pad(WINDOW* win, int y_pos, const char* string,
        chtype color, int width);

/*********************************************************************
 * print_player_info: prints a heading and the player's name,        *
 *                    highlighted in color if it is that player's    *
 *                    turn; takes window in which to print, line on  *
 *                    which to start printing, the player, and the   *
 *                    color in which to print; returns the total     *
 *                    number of lines printed; depends on macros     *
 *                    PLYRNFO_TB_PADDING, PLYR_TEXT_HDR, and         *
 *                    NAME_MAXLEN; calls print_xcenter_w_pad; and,   *
 *                    of course, requires <ncurses.h>                *
 *********************************************************************/
int print_player_info(WINDOW* pane_win, int text_y,
        const Player* player, chtype color);

/*********************************************************************
 * turn_color: returns the color pair in which a player's info       *
 *             should be displayed, based on whether it's currently  *
 *             their turn; depends on hardcoded values, defined in   *
 *             macros COLOR_PAIR_MYTURN and COLOR_PAIR_WAITING;      *
 *********************************************************************/
chtype turn_color(const Player* player);

/*********************************************************************
 * leave_mark: given a pointer to the gameboard, board, the square   *
 *             chosen by the player, indicated by count from 0, and  *
 *             the mark associated with player who made the choice,  *
 *             calls either draw_X or draw_Y to draw the mark on the *
 *             gameboard.
 *********************************************************************/
void leave_mark(WINDOW* board, int choice, bool mark);
/*********************************************************************
 * draw_X: given a y_position and x_position relative to WINDOW      *
 *         pointed to by board, draws a large X, designated with 'X' *
 *         characters, fitting exactly in a square on the 3x3 board. *
 *         requires math.h (nearbyint); compile with -lm             *
 *********************************************************************/
void draw_X(WINDOW* board, int y_position, int x_position);
/*********************************************************************
 * draw_O: given a y_position and x_position relative to WINDOW      *
 *         pointed to by board, draws a large O, designated with 'O' *
 *         characters, fitting exactly in a square on the 3x3 board. *
 *         requires math.h (nearbyint); compile with -lm             *
 *********************************************************************/
void draw_O(WINDOW* board, int y_position, int x_position);

