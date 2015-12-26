/*
 * File   : draw.c
 * Program: tim-tam-tom
 * Purpose: presentation macros, functions, and structures - lib
 * Author : Tommy Lincoln <pajamapants3000@gmail.com>
 * License: MIT -- See LICENSE
 * Notes  : Link with -lm
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "draw.h"

void draw_board(WINDOW* board, bool numbered)/*{{{*/
{

    // Draw outer border
    box(board, 0, 0);
    // top horizontal line, with end pieces
    mvwhline(board, BOARD_H / 3, 1, 0, BOARD_W - 2);
    mvwaddch(board, BOARD_H / 3,           0, ACS_LTEE);
    mvwaddch(board, BOARD_H / 3, BOARD_W - 1, ACS_RTEE);
    // bottom horizontal line, with end pieces
    mvwhline(board, 2 * BOARD_H / 3, 1, 0, BOARD_W - 2);
    mvwaddch(board, 2 * BOARD_H / 3,           0, ACS_LTEE);
    mvwaddch(board, 2 * BOARD_H / 3, BOARD_W - 1, ACS_RTEE);
    // left vertical line, with end pieces
    mvwvline(board, 1, BOARD_W / 3, 0, BOARD_H - 2);
    mvwaddch(board,           0, BOARD_W / 3, ACS_TTEE);
    mvwaddch(board, BOARD_H - 1, BOARD_W / 3, ACS_BTEE);
    // right vertical line, with end pieces
    mvwvline(board, 1, 2 * BOARD_W / 3, 0, BOARD_H - 2);
    mvwaddch(board,           0, 2 * BOARD_W / 3, ACS_TTEE);
    mvwaddch(board, BOARD_H - 1, 2 * BOARD_W / 3, ACS_BTEE);

    // AND to make things nice and neat, the 4-way crossings
    mvwaddch(board,     BOARD_H / 3,     BOARD_W / 3, ACS_PLUS);
    mvwaddch(board, 2 * BOARD_H / 3,     BOARD_W / 3, ACS_PLUS);
    mvwaddch(board,     BOARD_H / 3, 2 * BOARD_W / 3, ACS_PLUS);
    mvwaddch(board, 2 * BOARD_H / 3, 2 * BOARD_W / 3, ACS_PLUS);

    if (numbered)
        draw_numbers(board);

    move(0,0);
    wrefresh(board);

}
/*}}}*/
void draw_numbers(WINDOW* board)/*{{{*/
{
        for(short i = 0; i < 9; ++i)
            mvwaddch(board, STEP_ALONG_CENTERS_Y(i),
                    STEP_ALONG_CENTERS_X(i), '1' + i);
}
/*}}}*/
void draw_msg_simple(WINDOW* msg_win, int msg_y, const char** message)/*{{{*/
{
    box(msg_win, 0, 0);

    while (**message)
        print_xcenter(msg_win, msg_y++, *message++, COLOR_PAIR_DEFAULT);

    move(0, 0);
    wrefresh(msg_win);
}
/*}}}*/
void draw_msg(WINDOW* msg_win, const char** message, chtype color)/*{{{*/
{
    int msg_win_h, msg_win_w, msg_y = 1, msg_lines = 0;

    box(msg_win, 0, 0);

    getmaxyx(msg_win, msg_win_h, msg_win_w);

    while (**(message + msg_lines++) != NUL)
        if (msg_lines == (msg_win_h - 1))
                break;

    while (msg_y != (msg_win_h - msg_lines) / 2)
        print_xcenter_w_pad(msg_win, msg_y++, "", color, msg_win_w - 2);
    while (**message)
        print_xcenter_w_pad(msg_win, msg_y++, *message++, color, msg_win_w-2);
    while (msg_y != (msg_win_h - 1))
        print_xcenter_w_pad(msg_win, msg_y++, "", color, msg_win_w - 2);

    move(0, 0);
    wrefresh(msg_win);
}
/*}}}*/
void print_xcenter(WINDOW* win, int y_pos, const char* string, chtype color)/*{{{*/
{
    int length, x_pos, win_w;
    float temp;

    if (win == NULL)
        win = stdscr;
    win_w = getmaxx(win);

    length = strlen(string);
    temp = (win_w - length) / 2;
    x_pos = (int) temp;

    wattron(win, color);
    mvwprintw(win, y_pos, x_pos, "%s", string);
    wattroff(win, color);

    move(0, 0);
    wrefresh(win);
}
/*}}}*/
void print_xcenter_w_pad(WINDOW* win, int y_pos, const char* string,/*{{{*/
        chtype color, int width)
{
    char* padding = (char*) malloc((width) * sizeof(char));

    for (int i = 0; i < width; ++i)
        *(padding + i) = ' ';
    *(padding + width) = NUL;

    print_xcenter(win, y_pos, padding, color);
    print_xcenter(win, y_pos, string, color);

}
/*}}}*/
void draw_pane(WINDOW* pane_win, const Player* playerX, const Player* playerO)/*{{{*/
{
    int pane_win_h, text_y;

    pane_win_h = getmaxy(pane_win);
    text_y = (pane_win_h - (NUM_PLAYERS * PLYR_TEXT_SECTION)) / 2;

    box(pane_win, 0, 0);

    // ******* Print text for each player ******* //
    // *** Active player highlighted in color *** //
    text_y +=print_player_info(pane_win, text_y, playerX, turn_color(playerX));
    text_y +=print_player_info(pane_win, text_y, playerO, turn_color(playerO));

    move(0, 0);
    wrefresh(pane_win);
}
/*}}}*/
int print_player_info(WINDOW* pane_win, int text_y,/*{{{*/
        const Player* player, chtype color)
{
    int line_start = text_y;

                    /* padding          */
    for (int i = 0; i < PLYRNFO_TB_PADDING; i++)    // blank, colored lines
        print_xcenter_w_pad(pane_win, text_y++, "", color, NAME_MAXLEN);
                    /* player heading  */
    print_xcenter_w_pad(pane_win, text_y++, PLYR_TEXT_HDR, color, NAME_MAXLEN);
    print_xcenter_w_pad(pane_win, text_y++, "", color, NAME_MAXLEN); // dbl-spc
                    /* player name     */
    print_xcenter_w_pad(pane_win, text_y++, player->name, color, NAME_MAXLEN);
    print_xcenter_w_pad(pane_win, text_y++, "", color, NAME_MAXLEN); // dbl-spc
                    /* padding          */
    for (int i = 0; i < PLYRNFO_TB_PADDING; i++)    // blank, colored lines
        print_xcenter_w_pad(pane_win, text_y++, "", color, NAME_MAXLEN);

    return text_y - line_start;
}
/*}}}*/
chtype turn_color(const Player* player)/*{{{*/
{
    return player->is_my_turn ? COLOR_PAIR_MYTURN : COLOR_PAIR_WAITING;
}
/*}}}*/
void leave_mark(WINDOW* board, int choice, bool mark)/*{{{*/
{
    int x_position, y_position;

    // S_A_C requires count from 0, but choices count from 1;
    y_position = STEP_ALONG_CENTERS_Y(choice);
    x_position = STEP_ALONG_CENTERS_X(choice);

    if (mark)
        draw_X(board, y_position, x_position);
    else
        draw_O(board, y_position, x_position);
}
/*}}}*/
void draw_X(WINDOW* board, int y_position, int x_position)/*{{{*/
{
    chtype ch = 'X';
    signed int delta_y, delta_x;
    double ddel_y;

    for (delta_x = 1 - SQUARE_MIDX; delta_x < SQUARE_MIDX; ++delta_x)
    {
        // skip center square - where the square number goes!
        if (delta_x == 0)
            continue;
        ddel_y = X_SLOPE * abs(delta_x);    // X_SLOPE has explicit double type
        delta_y = nearbyint(ddel_y);
        mvwaddch(board, y_position + delta_y, x_position + delta_x, ch);
        ddel_y = -X_SLOPE * abs(delta_x);   // X_SLOPE has explicit double type
        delta_y = nearbyint(ddel_y);
        mvwaddch(board, y_position + delta_y, x_position + delta_x, ch);
    }
    move(0, 0);
    wrefresh(board);
}
/*}}}*/
void draw_O(WINDOW* board, int y_position, int x_position)/*{{{*/
{
    chtype ch = 'O';
    signed int delta_y, delta_x;
    double ddel_y;

    for (delta_x = 1 - SQUARE_MIDX; delta_x < SQUARE_MIDX; ++delta_x)
    {
        // ELLIPSE_PLUSY has explicit double type
        ddel_y = ELLIPSE_PLUSY(delta_x, SQUARE_MIDY, SQUARE_MIDX);
        delta_y = nearbyint(ddel_y);
        mvwaddch(board, y_position + delta_y, x_position + delta_x, ch);
        // ELLIPSE_PLUSY has explicit double type
        ddel_y = -ELLIPSE_PLUSY(delta_x, SQUARE_MIDY, SQUARE_MIDX);
        delta_y = nearbyint(ddel_y);
        mvwaddch(board, y_position + delta_y, x_position + delta_x, ch);
    }
    move(0, 0);
    wrefresh(board);
}
/*}}}*/
