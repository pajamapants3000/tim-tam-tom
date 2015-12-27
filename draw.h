/*
 * File   : draw.h
 * Program: tim-tam-tom -- A tic-tac-toe game written in curses
 * Purpose: presentation macros, functions, and structures - header
 * Author : Tommy Lincoln <pajamapants3000@gmail.com>
 * License: MIT -- See LICENSE
 * Notes  : Link with -lm -lncurses
 */

#ifndef GUARD_DRAW_H
#define GUARD_DRAW_H

#include <ncurses.h>
#include "moves.h"                  // for Player (struct player_struct)
#include "form_username.h"          // for NAME_MAXLEN

// who's in? //
#define NUM_PLAYERS 2
// total screen space to occupy
#define TOTAL_W 80
#define TOTAL_H 35

 /*             *              */
/***        Gameboard       ***/
// defining macros -- these may be modified as needed
// NOTE: (BOARD_H - 1) / BOARD_NUM_SQUARES_Y == integer!
//+(similarly for BOARD_W and BOARD_NUM_SQUARES_X)
#define BOARD_NUM_SQUARES_X 3
#define BOARD_NUM_SQUARES_Y 3
#define BOARD_H 25
#define BOARD_W 49
// derived macros -- these should not be changed
#define SQUARE_DIMY ((BOARD_H - 1) / BOARD_NUM_SQUARES_Y)
#define SQUARE_DIMX ((BOARD_W - 1) / BOARD_NUM_SQUARES_X)
#define BOARD_Y (TITLE_WIN_Y + TITLE_WIN_H)
#define BOARD_X (PANE_WIN_X + PANE_WIN_W)
#define SQUARE_MIDX (SQUARE_DIMX / 2)
#define SQUARE_MIDY (SQUARE_DIMY / 2)
#define STEP_ALONG_CENTERS_Y(i) (BOARD_H - 1 - (SQUARE_MIDY + \
        (SQUARE_DIMY * (i / BOARD_NUM_SQUARES_Y))))
#define STEP_ALONG_CENTERS_X(i) (SQUARE_MIDX + \
        (SQUARE_DIMX * (i % BOARD_NUM_SQUARES_X)))
////////////////////////////////

/* macros to assist in creating X's and O's on the board    */
/************************************************************/
// f(x) proportional to x; take this and its negative to make the 'X'
#define X_SLOPE ((double) (SQUARE_DIMY - 1) / (SQUARE_DIMX - 1))
#define X_PLUSY(x, Y, X) ((((double) (Y-1)) / (X-1)) * abs(x))
//
// positive values of f(x) for an ellipse centered at origin (-*modified)
// Ry = vertical radius; Rx = horizontal radius
//-*NOTE: I toyed with the parameters a bit; at least for the board size which
//-*I tested, raising the second term to 1.2 instead of 2 made a better shape.
#define ELLIPSE_PLUSY(x, Ry, Rx) (sqrt(pow((Ry-1), 2) *\
        (1 - (pow(((double) abs(x)) / (Rx-1), 1.2)))))
/************************************************************/

/***        Title Window        ***/
// do not edit, with the exception of TITLE_WIN_Y;
//+looks better near the top rather than centered;
// TITLE_WIN_X and TITLE_WIN_Y fix the positioning
//+of the entire setup; probably want to leave it
//+centered on X.
// this value centers the board vertically;
//#define TITLE_WIN_Y ((LINES - (BOARD_H + TITLE_WIN_H)) / 2)
// this is a custom-defined value:
#define TITLE_WIN_Y 2
/*      *       */
#define TITLE_WIN_X ((COLS  - (BOARD_W + PANE_WIN_W)) / 2)
#define TITLE_WIN_H (TOTAL_H - BOARD_H)
#define TITLE_WIN_W (BOARD_W + PANE_WIN_W)
///////////////////////////////////

/***        Side Pane       ***/
// do not edit
#define PANE_WIN_H BOARD_H
#define PANE_WIN_W (TOTAL_W - BOARD_W)
#define PANE_WIN_Y BOARD_Y
#define PANE_WIN_X TITLE_WIN_X
#define PLYRNFO_NUM_LINES 2
#define PLYRNFO_TB_PADDING 2
// Number of lines devoted to each player in the side info pane
// number of lines of text, double spaced, padded on top and bottom
#define PLYR_TEXT_SECTION (2 * PLYRNFO_NUM_LINES + 2 * PLYRNFO_TB_PADDING)
#define PLYR_TEXT_HDR (player->mark ? "Player 1 - X's" : "Player 2 - O's")

/***        Message Window          ***/
// Nothing else depends on these, so do what you will!
//+just leave enough room for the messages, at
//+LEAST 4x25
#define MSG_WIN_H  8
#define MSG_WIN_W 40
#define MSG_WIN_Y 10
#define MSG_WIN_X ((COLS  - MSG_WIN_W) / 2)

/***        Colors      ***/
// edit colors to taste; leave COLOR_PAIR_* alone
#define COLOR_MYTURN_F  COLOR_BLACK     // left pane info - active player
#define COLOR_MYTURN_B  COLOR_BLUE      // left pane info - active player
#define COLOR_WAITING_F COLOR_WHITE     // left pane info - waiting player
#define COLOR_WAITING_B COLOR_BLACK     // left pane info - waiting player
#define COLOR_DEFAULT_F COLOR_CYAN      // default colorscheme
#define COLOR_DEFAULT_B COLOR_BLACK     // default colorscheme
#define COLOR_TITLE_F   COLOR_BLACK     // main title/heading
#define COLOR_TITLE_B   COLOR_CYAN      // main title/heading
#define COLOR_WARNING_F COLOR_MAGENTA   // warning message
#define COLOR_WARNING_B COLOR_RED       // warning message
#define COLOR_PAIR_NUM_DEFAULT 1
#define COLOR_PAIR_NUM_TITLE   2
#define COLOR_PAIR_NUM_MYTURN  3
#define COLOR_PAIR_NUM_WAITING 4
#define COLOR_PAIR_NUM_WARNING 5
#define COLOR_PAIR_DEFAULT COLOR_PAIR(COLOR_PAIR_NUM_DEFAULT)
#define COLOR_PAIR_TITLE   COLOR_PAIR(COLOR_PAIR_NUM_TITLE)
#define COLOR_PAIR_MYTURN  COLOR_PAIR(COLOR_PAIR_NUM_MYTURN)
#define COLOR_PAIR_WAITING COLOR_PAIR(COLOR_PAIR_NUM_WAITING)
#define COLOR_PAIR_WARNING COLOR_PAIR(COLOR_PAIR_NUM_WARNING)

/*********************************************************************
 * draw_board: draws the tic-tac-toe 3x3 board, with optional        *
 *             numbering of the squares. The function, in its        *
 *             present implementation, is hardcoded with values      *
 *             supplied by various macros, all prefixed BOARD_       *
 *             Its window, and therefore its dimensions, are defined *
 *             outside the function and passed in, to allow for      *
 *             persistence of the WINDOW state in the calling        *
 *             function (probably main).                             *
 * Depends on: <ncurses.h>, draw_numbers                             *
 *********************************************************************/
void draw_board(WINDOW* board, bool numbered);

/*********************************************************************
 * draw_numbers: draws numbers in the center of each square on the   *
 *               3x3 board contained in the WINDOW* board passed in. *
 *   Depends on: <ncurses.h>, BOARD_H, BOARD_W
 *********************************************************************/
void draw_numbers(WINDOW* board);

/*********************************************************************
 * draw_msg_simple: displays a message; takes the window, the line   *
 *                  on which to place the first line of the message, *
 *                  and the (possibly multi-line) message as         *
 *                  arguments in that order; Also uses the           *
 *                  COLOR_DEFAULT macro.                             *
 *      Depends on: <ncurses.h>, COLOR_PAIR_DEFAULT
 *********************************************************************/
void draw_msg_simple(WINDOW* msg_win, int msg_y, const char** message);

/*********************************************************************
 * draw_pane: Shows information and current status of the game,      *
 *            displayed in the WINDOW* pane_win passed as first arg; *
 *            Highlights the player whose turn it is; Depends on     *
 *            various hardcoded macros with PANE_ prefix, as well as *
 *            several macros with COLOR_ prefix.                     *
 *Depends on: <ncurses.h>, print_player_info, turn_color,            *
 *            NUM_PLAYERS, PLYR_TEXT_SECTION                         *
 *********************************************************************/
void draw_pane(WINDOW* pane_win, const Player* playerX, const Player* playerO);

/*********************************************************************
 * draw_msg: A nice function for displaying a message, possibly in   *
 *           color! Centers the message vertically and center-       *
 *           justifies each line. Fills the entire window with       *
 *           color. Depends only on <ncurses.h> and my function      *
 *           print_xcenter_w_pad. Also requires color to be          *
 *           initialized. Takes window, message, and color as args.  *
 *Depends on: <ncurses.h>, print_xcenter_w_pad
 *********************************************************************/
void draw_msg(WINDOW* msg_win, const char** message, chtype color);

/*********************************************************************
 * print_xcenter: prints a center-justified string on a single line; *
 *                does so on the y_pos'th line of window win, in     *
 *                color color. Depends on <ncurses.h>, with color    *
 *                initialized; also <string.h>.
 *    Depends on: <ncurses.h>
 *********************************************************************/
void print_xcenter(WINDOW* win, int y_pos, const char* string, chtype color);

/*********************************************************************
 * print_xcenter_w_pad: Calls print_xcenter twice: once with blanks  *
 *                      in a given width to establish background     *
 *                      color, and a second time to print text; both *
 *                      outputs are center-justified; takes the same *
 *                      arguments as print_xcenter, but with a set   *
 *                      width added to define width of output.       *
 *        Depends on: <ncurses.h>, <stdlib.h> (malloc) print_xcenter *
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
 *        Depends on: <ncurses.h>, PLYRNFO_TB_PADDING, NAME_MAXLEN,  *
 *                    print_xcenter_w_pad
 *********************************************************************/
int print_player_info(WINDOW* pane_win, int text_y,
        const Player* player, chtype color);

/*********************************************************************
 * turn_color: returns the color pair in which a player's info       *
 *             should be displayed, based on whether it's currently  *
 *             their turn; depends on hardcoded values, defined in   *
 *             macros COLOR_PAIR_MYTURN and COLOR_PAIR_WAITING;      *
 * Depends on: <ncurses.h>, COLOR_PAIR_MYTURN, COLOR_PAIR_WAITING    *
 *********************************************************************/
chtype turn_color(const Player* player);

/*********************************************************************
 * leave_mark: given a pointer to the gameboard, board, the square   *
 *             chosen by the player, indicated by count from 0, and  *
 *             the mark associated with player who made the choice,  *
 *             calls either draw_X or draw_Y to draw the mark on the *
 *             gameboard.                                            *
 * Depends on: <ncurses.h>, draw_X, draw_O,                          *
 *             STEP_ALONG_CENTERS_Y, STEP_ALONG_CENTERS_X            *
 *********************************************************************/
void leave_mark(WINDOW* board, int choice, bool mark);

/*********************************************************************
 * draw_X: given a y_position and x_position relative to WINDOW      *
 *         pointed to by board, draws a large X, designated with 'X' *
 *         characters, fitting exactly in a square on the 3x3 board. *
 *         requires math.h (nearbyint); compile with -lm             *
 *Depends on: <ncurses.h>, SQUARE_MIDY, SQUARE_MIDX, X_SLOPE         *
 *********************************************************************/
void draw_X(WINDOW* board, int y_position, int x_position);

/*********************************************************************
 * draw_O: given a y_position and x_position relative to WINDOW      *
 *         pointed to by board, draws a large O, designated with 'O' *
 *         characters, fitting exactly in a square on the 3x3 board. *
 *         requires math.h (nearbyint); compile with -lm             *
 *Depends on: <ncurses.h>, SQUARE_MIDY, SQUARE_MIDX, ELLIPSE_PLUSY   *
 *********************************************************************/
void draw_O(WINDOW* board, int y_position, int x_position);

#endif      // end of header guard

