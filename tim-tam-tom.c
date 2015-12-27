/*
 * File   : tim-tam-tom.c
 * Program: tim-tam-tom -- A tic-tac-toe game written in curses
 * Purpose: A tic-tac-toe game!
 * Author : Tommy Lincoln <pajamapants3000@gmail.com>
 * License: MIT - See LICENSE
 * Notes  : Link with -lm -lform -lncurses;
 */

#include <stdlib.h>
#include "draw.h"
#include "moves.h"
#include "form_username.h"

#define NUL '\0'
#define INIT_CURS initscr(); cbreak(); noecho(); start_color(); refresh()
#define REDRAW draw_msg(title_win, msg_title, COLOR_PAIR_TITLE); \
               draw_board(board, true);                          \
               draw_pane(pane_win, playerX, playerO)
#define RESET_GAME werase(board);                    \
                wrefresh(board);                     \
                playerX->markers = 0;                \
                playerO->markers = 0;                \
                playerX->num_turns_taken = 0;        \
                playerO->num_turns_taken = 0;        \
                playerX->is_my_turn = playerX->mark; \
                playerO->is_my_turn = playerO->mark
#define MSG_WELCOME { "Welcome to Tim-Tam-Tom...", \
                      "Get ready to have FUN!!!",  \
                      ""}
#define MSG_TITLE {                                   \
"  =======   =======      =======       ",            \
"     |         | _          | _        ",            \
"     |x|/\\ /\\ox|/o\\ |/\\x/\\ox|/o\\|/\\ /\\ ",    \
"     |||  \\  \\ |\\_/\\|  \\  \\ |\\_/|  \\  \\", "" }

#define MSG_WARNING { "Uh-oh!", "You have pressed an invalid key.", ""}
#define MSG_GOODBYE { "Thanks for playing...", "See you again soon!", ""}
#define MSG_STALE  { "You're both out of moves...", " ", "Stalemate!", ""}
#define MSG_PLAYAGAIN  { "Play", "Again?", "(press 'y'!)", ""}

void winner(Player* winner, Player* loser);
void stalemate();

int main(void)
{
    WINDOW *msg_win, *board, *pane_win, *title_win;
    struct Prompt* Prompt_for_Name;
    Player *playerX, *playerO;
    char input;

    INIT_PLAYER(playerX, 1);
    INIT_PLAYER(playerO, 0);
    const char* msg_title    [] = MSG_TITLE;
    const char* msg_welcome  [] = MSG_WELCOME;
    const char* msg_goodbye  [] = MSG_GOODBYE;
    const char* msg_warning  [] = MSG_WARNING;
    const char* msg_playagain[] = MSG_PLAYAGAIN;

    INIT_CURS;
    init_pair(COLOR_PAIR_NUM_DEFAULT, COLOR_DEFAULT_F, COLOR_DEFAULT_B);
    init_pair(  COLOR_PAIR_NUM_TITLE,   COLOR_TITLE_F,   COLOR_TITLE_B);
    init_pair( COLOR_PAIR_NUM_MYTURN,  COLOR_MYTURN_F,  COLOR_MYTURN_B);
    init_pair(COLOR_PAIR_NUM_WAITING, COLOR_WAITING_F, COLOR_WAITING_B);
    init_pair(COLOR_PAIR_NUM_WARNING, COLOR_WARNING_F, COLOR_WARNING_B);

    board     = newwin(    BOARD_H,     BOARD_W,     BOARD_Y,     BOARD_X);
    msg_win   = newwin(  MSG_WIN_H,   MSG_WIN_W,   MSG_WIN_Y,   MSG_WIN_X);
    pane_win  = newwin( PANE_WIN_H,  PANE_WIN_W,  PANE_WIN_Y,  PANE_WIN_X);
    title_win = newwin(TITLE_WIN_H, TITLE_WIN_W, TITLE_WIN_Y, TITLE_WIN_X);

    // Welcome!
    draw_msg(title_win, msg_title, COLOR_PAIR_TITLE);
    draw_board(board, false);
    draw_msg(msg_win, msg_welcome, COLOR_PAIR_DEFAULT);
    PRESS_ANY_KEY;

    // Clear welcome messaage...
    werase(msg_win);
    wrefresh(msg_win);
    draw_msg(title_win, msg_title, COLOR_PAIR_TITLE);
    draw_board(board, false);

    // ... and prompt for users to enter their names
    Prompt_for_Name = draw_form();
    get_username(Prompt_for_Name, playerX->name);
    get_username(Prompt_for_Name, playerO->name);
    werase(Prompt_for_Name->form_win);
    wrefresh(Prompt_for_Name->form_win);

    // Play the game!
  for (;;) {                  // option to play multiple games
    REDRAW;
    while ((input = getch()))
    {
        // (input - '1') converts 1-9 char to 0-8 int
        if ('1' <= input && input <= '9' &&
            is_valid_move(playerX, playerO, square_dec_to_bin(input - '1')))
        {
            leave_mark(board, input - '1', playerX->is_my_turn ? \
                    playerX->mark : playerO->mark);
            update_turns(playerX, playerO, square_dec_to_bin(input - '1'));
        }
        else if (input == 'q' || input == 'Q')
        {
            draw_msg(msg_win, msg_goodbye, COLOR_PAIR_MYTURN);
            PRESS_ANY_KEY;
            free_form_mem(Prompt_for_Name);
            endwin();
            return 0;
        }
        else
        {
            draw_msg(msg_win, msg_warning, COLOR_PAIR_WARNING);
            PRESS_ANY_KEY;
            werase(msg_win);
            wrefresh(msg_win);
            REDRAW;
            continue;
        }

        // NOTE: last player to move now has is_my_turn == false
        if (player_wins(playerX->is_my_turn ? playerO : playerX))
        {
            winner(playerX->is_my_turn ? playerO : playerX,
                   playerX->is_my_turn ? playerX : playerO);
            break;
        }
        else if (reached_stalemate(playerX, playerO))
        {
            stalemate();
            break;
        }
        REDRAW;
    }

    draw_msg(msg_win, msg_playagain, COLOR_PAIR_DEFAULT);
    input = getch();
    werase(msg_win);
    wrefresh(msg_win);
    if (input == 'y' || input == 'Y')
    {
        RESET_GAME;
        continue;
    }
    else
        break;
  }

    free_form_mem(Prompt_for_Name);
    draw_msg(msg_win, msg_goodbye, COLOR_PAIR_DEFAULT);
    endwin();

    return 0;
}

void winner(Player* winner, Player* loser)
{
    WINDOW* msg_win;
    char* msg_winner[5];
    const char* msg_winner0 = "Winner!";

    // ACH!  -- I need to find a neater way!
    /*  *   *   *   *   *   *   *   *   *   *   */
    msg_winner[0] = (char*) malloc(8 * sizeof(char));
    msg_winner[0] = (char*) msg_winner0;
    msg_winner[1] = (char*) malloc(2 * sizeof(char));
    msg_winner[1][0] = ' ';
    msg_winner[1][1] = NUL;
    msg_winner[2] = (char*) malloc((8 + NAME_MAXLEN) * sizeof(char));
    sprintf(msg_winner[2], "%s kicked", winner->name);
    msg_winner[3] = (char*) malloc((11 + NAME_MAXLEN) * sizeof(char));
    sprintf(msg_winner[3], "%s's butt!!!", loser->name);
    msg_winner[4] = (char*) malloc(1 * sizeof(char));
    msg_winner[4][0] = NUL;
    /*  *   *   *   *   *   *   *   *   *   *   */
    // I made a mess creating the message, may as well show it:
    msg_win   = newwin(  MSG_WIN_H,   MSG_WIN_W,   MSG_WIN_Y,   MSG_WIN_X);
    draw_msg(msg_win, (const char**) msg_winner, COLOR_PAIR_DEFAULT);
    wrefresh(msg_win);
    PRESS_ANY_KEY;
    delwin(msg_win);
    refresh();
}
void stalemate()
{
    WINDOW* msg_win;
    const char* msg_stale[] = MSG_STALE;
    msg_win   = newwin(  MSG_WIN_H,   MSG_WIN_W,   MSG_WIN_Y,   MSG_WIN_X);
    draw_msg(msg_win, msg_stale, COLOR_PAIR_DEFAULT);
    PRESS_ANY_KEY;
    delwin(msg_win);
    refresh();
}
