// player_move_main.c
// stripping player_move.c of all but main to see if headers and libraries
//+work correctly

#include <stdlib.h>
#include "../moves.h"
#include "../draw.h"


#define INIT_NCURSES initscr(); cbreak(); noecho(); refresh()
#define KB_ENTER 10     // KEY_ENTER doesn't work for some reason
#define NAMLEN   20     // maximum length for name (re-label?)
#define INPUT    '3'    // randomly chosen "input" value

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
    board_xstart = BOARD_X;
    board_ystart = BOARD_Y;

    board = newwin(BOARD_H, BOARD_W, board_ystart, board_xstart);
    draw_board(board, numbered);
    getch();

    if (!is_valid_move(playerX, playerO, insq))
    {
        endwin();
        printf("error: invalid move1!\n");
        return 1;
    }

    leave_mark(board, input - '1', 1);
    update_turns(playerX, playerO, square_dec_to_bin(input - '1'));

    getch();
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

