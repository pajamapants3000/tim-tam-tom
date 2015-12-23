// draw_board.c
// simple test for drawing tic-tac-toe board; size 28 rows x 56 columns;
//***  DONE! ***
// I would like to add (later) a check on the terminal size that determines
//+the appropriate board size. The current size should be good for most
//+screens.

#include <ncurses.h>
#include <stdbool.h>

// *gameboard dimensions*
// defining macros
#define BOARD_NUM_SQUARES_X 3
#define BOARD_NUM_SQUARES_Y 3
#define SQUARE_DIMY  8
#define SQUARE_DIMX 16
// derived macros
#define BOARD_DIMY (3 * SQUARE_DIMY + 1)
#define BOARD_DIMX (3 * SQUARE_DIMX + 1)
#define SQUARE_MIDX (SQUARE_DIMX / 2)
#define SQUARE_MIDY (SQUARE_DIMY / 2)
#define STEP_ALONG_CENTERS_Y(i) (SQUARE_MIDY + \
        (SQUARE_DIMY * (i / BOARD_NUM_SQUARES_Y)))
#define STEP_ALONG_CENTERS_X(i) (SQUARE_MIDX + \
        (SQUARE_DIMX * (i % BOARD_NUM_SQUARES_X)))
// *                    *

#define MESSAGE_H  8
#define MESSAGE_W 40
#define MESSAGE { "Hello, and welcome to the message!",\
                  "This is line two of the message." }

void draw_board(WINDOW* board, int board_ystart, int board_xstart, bool numbered);
void draw_numbers(WINDOW* board);
void draw_message(char** message);

int main(void)
{
    int board_ystart, board_xstart;
    bool numbered = true;
    WINDOW* board;
    char** message = MESSAGE;

    initscr();
    refresh();
    board_xstart = (COLS  - BOARD_DIMX) / 2;
    board_ystart = (LINES - BOARD_DIMY) / 2;

    board = newwin(BOARD_DIMY, BOARD_DIMX, board_ystart, board_xstart);

    draw_board(board, board_ystart, board_xstart, numbered);

    move(0,0);
    refresh();

    getch();

    endwin();

    return 0;
}

void draw_board(WINDOW* board, int board_ystart, int board_xstart, bool numbered)
{

    // Draw outer border
    box(board, 0, 0);
    // top horizontal line, with end pieces
    mvwhline(board,     BOARD_DIMY / 3, 1, 0, BOARD_DIMX - 2);
    mvwaddch(board, BOARD_DIMY / 3, 0, ACS_LTEE);
    mvwaddch(board, BOARD_DIMY / 3, BOARD_DIMX - 1, ACS_RTEE);
    // bottom horizontal line, with end pieces
    mvwhline(board, 2 * BOARD_DIMY / 3, 1, 0, BOARD_DIMX - 2);
    mvwaddch(board, 2 * BOARD_DIMY / 3, 0, ACS_LTEE);
    mvwaddch(board, 2 * BOARD_DIMY / 3, BOARD_DIMX - 1, ACS_RTEE);
    // left vertical line, with end pieces
    mvwvline(board, 1,     BOARD_DIMX / 3, 0, BOARD_DIMY - 2);
    mvwaddch(board, 0, BOARD_DIMX / 3, ACS_TTEE);
    mvwaddch(board, BOARD_DIMY - 1, BOARD_DIMX / 3, ACS_BTEE);
    // right vertical line, with end pieces
    mvwvline(board, 1, 2 * BOARD_DIMX / 3, 0, BOARD_DIMY - 2);
    mvwaddch(board, 0, 2 * BOARD_DIMX / 3, ACS_TTEE);
    mvwaddch(board, BOARD_DIMY - 1, 2 * BOARD_DIMX / 3, ACS_BTEE);

    // AND to make things nice and neat, the 4-way crossings
    mvwaddch(board,     BOARD_DIMY / 3,     BOARD_DIMX / 3, ACS_PLUS);
    mvwaddch(board, 2 * BOARD_DIMY / 3,     BOARD_DIMX / 3, ACS_PLUS);
    mvwaddch(board,     BOARD_DIMY / 3, 2 * BOARD_DIMX / 3, ACS_PLUS);
    mvwaddch(board, 2 * BOARD_DIMY / 3, 2 * BOARD_DIMX / 3, ACS_PLUS);

    if (numbered)
        draw_numbers(board);

    wrefresh(board);
}

void draw_numbers(WINDOW* board)
{
        for(short i = 0; i < 9; ++i)
            mvwaddch(board, STEP_ALONG_CENTERS_Y(i),
                    STEP_ALONG_CENTERS_X(i), '1' + i);
}

void draw_message(char** message)
{
    int message_ystart, message_xstart;
    WINDOW* message_win;
    message_xstart = (COLS  - MESSAGE_W) / 2;
    message_ystart = (LINES - MESSAGE_H) / 2;

    message = newwin(MESSAGE_H, MESSAGE_W, message_ystart, message_xstart);
}
