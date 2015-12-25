// player_move.c
// portion of tim-tam-tom which processes a single turn
// DONE!

#include <ncurses.h>
#include <math.h>
#include <stdlib.h>

// *gameboard dimensions* (newer version in draw_board.c)
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

// Wins
#define WIN_ROW0 ((1 << 0) | (1 << 1) | (1 << 2))
#define WIN_ROW1 ((1 << 3) | (1 << 4) | (1 << 5))
#define WIN_ROW2 ((1 << 6) | (1 << 7) | (1 << 8))
#define WIN_COL0 ((1 << 0) | (1 << 3) | (1 << 6))
#define WIN_COL1 ((1 << 1) | (1 << 4) | (1 << 7))
#define WIN_COL2 ((1 << 2) | (1 << 5) | (1 << 8))
#define WIN_DGDR ((1 << 0) | (1 << 4) | (1 << 8))
#define WIN_DGUR ((1 << 2) | (1 << 4) | (1 << 6))
#define WINS { WIN_ROW0, WIN_ROW1, WIN_ROW2, WIN_COL0, WIN_COL1, WIN_COL2, \
    WIN_DGDR, WIN_DGUR }
#define NUM_WINTYPES 8

#define INIT_NCURSES initscr(); cbreak(); noecho(); refresh()
#define KB_ENTER 10     // KEY_ENTER doesn't work for some reason
#define NAMLEN   20     // maximum length for name (re-label?)
#define INPUT    '3'    // randomly chosen "input" value

// f(x) for f proportional to x; take this and its negative to make the 'X'
#define X_PLUSY(x, Y, X) ((((double) (Y-1)) / (X-1)) *\
        abs(x))
// positive values of f(x) for an ellipse centered at origin
// vertical radius of Y, horizontal radius of X
// NOTE: I toyed with the equation a bit; at least for the size I tested on,
// raising the second term to 1.5 instead of 2 made a better shape.
#define ELLIPSE_PLUSY(x, Y, X) (sqrt(pow((Y-1), 2) *\
        (1 - (pow(((double) abs(x)) / (X-1), 1.5)))))

// Error codes
#define BOOKKEEPING_ERROR 83        // some stored data has impossible value

typedef unsigned short squares;
struct player_struct
{
    char name[NAMLEN];
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
void draw_board(WINDOW* board, int board_ystart, int board_xstart, bool numbered);
void draw_numbers(WINDOW* board);
/*}}}*/
// main function to test player_move functions
int main(void)
{
    // NOTE: Input is the char value '1' to '9', gets converted to choice, an
    //+integer between 0 and 8 inclusive. This may in turn be converted to
    //+the bitmask square for bookkeeping.
    char input = INPUT;
    squares insq = square_dec_to_bin(input - '1');
    int board_ystart, board_xstart;
    bool numbered = true;
    WINDOW* board;
    Player *playerX, *playerO;
    playerX = (Player*) malloc(sizeof(Player));
    playerO = (Player*) malloc(sizeof(Player));
    playerX->is_my_turn = 1;
    playerO->is_my_turn = 0;
    playerX->num_turns_taken = 0;
    playerO->num_turns_taken = 0;
    playerX->mark = 1;
    playerO->mark = 0;
    playerX->markers =  _0_x_0_ | _1_x_2_ | _1_x_1_ | _0_x_1_;
    playerO->markers =  _1_x_0_ | _2_x_1_ | _2_x_2_ | _2_x_0_;

    INIT_NCURSES;
    board_xstart = (COLS  - BOARD_DIMX) / 2;
    board_ystart = (LINES - BOARD_DIMY) / 2;

    board = newwin(BOARD_DIMY, BOARD_DIMX, board_ystart, board_xstart);
    draw_board(board, board_ystart, board_xstart, numbered);
    getch();

    if (!is_valid_move(playerX, playerO, insq))
    {
        endwin();
        printf("error: invalid move1!\n");
        return 1;
    }

    leave_mark(board, input - '1', 1);
    update_turns(playerX, playerO, square_dec_to_bin(input - '1'));

    if (player_wins(playerX))
    {
        endwin();
        printf("playerX wins!\n");
        return 1;
    }

    if (reached_stalemate(playerX, playerO))
    {
        endwin();
        printf("stalemate!\n");
        return 1;
    }
    getch();

    input = '2';
    if (!is_valid_move(playerX, playerO, square_dec_to_bin(input - '1')))
    {
        endwin();
        printf("error: invalid move2!\n");
        return 1;
    }

    leave_mark(board, input - '1', 0);
    update_turns(playerX, playerO, square_dec_to_bin(input - '1'));

    getch();

    endwin();
    return 0;
}

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
        ddel_y = (((double) (SQUARE_DIMY-1)) / (SQUARE_DIMX-1)) * abs(delta_x);
        delta_y = nearbyint(ddel_y);
        mvwaddch(board, y_position + delta_y, x_position + delta_x, ch);
        ddel_y = -(((double) (SQUARE_DIMY-1)) / (SQUARE_DIMX-1)) * abs(delta_x);
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
        ddel_y = ELLIPSE_PLUSY(delta_x, SQUARE_MIDY, SQUARE_MIDX);
        delta_y = nearbyint(ddel_y);
        mvwaddch(board, y_position + delta_y, x_position + delta_x, ch);
        ddel_y = -ELLIPSE_PLUSY(delta_x, SQUARE_MIDY, SQUARE_MIDX);
        delta_y = nearbyint(ddel_y);
        mvwaddch(board, y_position + delta_y, x_position + delta_x, ch);
    }
    move(0, 0);
    wrefresh(board);
}
/*}}}*/
squares square_dec_to_bin(int choice)/*{{{*/
{
    return (1 << choice);
}
/*}}}*/
void draw_board(WINDOW* board, int board_ystart, int board_xstart, bool numbered)/*{{{*/
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
/*}}}*/
void draw_numbers(WINDOW* board)/*{{{*/
{
        for(short i = 0; i < 9; ++i)
            mvwaddch(board, STEP_ALONG_CENTERS_Y(i),
                    STEP_ALONG_CENTERS_X(i), '1' + i);
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
