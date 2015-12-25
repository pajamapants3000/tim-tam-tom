// draw_board.c
// simple test for drawing tic-tac-toe board; size 28 rows x 56 columns;
//***  DONE! ***
// I would like to add (later) a check on the terminal size that determines
//+the appropriate board size. The current size should be good for most
//+screens.

#include <ncurses.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

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

// program to test draw_board functions
int main(void)/*{{{*/
{
    bool numbered = true;
    WINDOW* board;
    WINDOW* msg_win;
    WINDOW* pane_win;
    WINDOW* title_win;
    const char* message[] = MESSAGE;
    const char* title[] = TITLE;
    Player player1, player2;
    Player *playerX = &player1;
    Player *playerO = &player2;
    playerX->name = (char*) malloc(NAME_MAXLEN * sizeof(char));
    playerO->name = (char*) malloc(NAME_MAXLEN * sizeof(char));
    playerX->name = "Dick";
    playerO->name = "Jane";
    playerX->is_my_turn = false;
    playerO->is_my_turn = true;

    INIT_CURS;
    init_pair(COLOR_PAIR_NUM_DEFAULT, COLOR_DEFAULT_F, COLOR_DEFAULT_B);
    init_pair(COLOR_PAIR_NUM_MYTURN, COLOR_MYTURN_F, COLOR_MYTURN_B);
    init_pair(COLOR_PAIR_NUM_WAITING, COLOR_WAITING_F, COLOR_WAITING_B);

    board     = newwin(    BOARD_H,     BOARD_W,     BOARD_Y,     BOARD_X);
    msg_win   = newwin(  MSG_WIN_H,   MSG_WIN_W,   MSG_WIN_Y,   MSG_WIN_X);
    pane_win  = newwin( PANE_WIN_H,  PANE_WIN_W,  PANE_WIN_Y,  PANE_WIN_X);
    title_win = newwin(TITLE_WIN_H, TITLE_WIN_W, TITLE_WIN_Y, TITLE_WIN_X);

    draw_board(board, numbered);

    refresh();

    getch();

    draw_msg(msg_win, message, COLOR_PAIR_MYTURN);
    getch();

    werase(msg_win);
    draw_board(board, numbered);
    draw_pane(pane_win, playerX, playerO);
    getch();
    draw_msg_simple(title_win, 2, title);
    getch();

    endwin();

    return 0;
}
/*}}}*/
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
void draw_numbers(WINDOW* board)/*{{{*//*{{{*//*{{{*//*{{{*//*{{{*//*{{{*/
{
        for(short i = 0; i < 9; ++i)
            mvwaddch(board, STEP_ALONG_CENTERS_Y(i),
                    STEP_ALONG_CENTERS_X(i), '1' + i);
}
/*}}}*/
void draw_msg_simple(WINDOW* msg_win, int msg_y, const char** message)
{
    box(msg_win, 0, 0);

    while (**message)
        print_xcenter(msg_win, msg_y++, *message++, COLOR_PAIR_DEFAULT);

    move(0, 0);
    wrefresh(msg_win);
}
/*}}}*/
void draw_msg(WINDOW* msg_win, const char** message, chtype color)
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
void print_xcenter(WINDOW* win, int y_pos, const char* string, chtype color)
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
void print_xcenter_w_pad(WINDOW* win, int y_pos, const char* string,
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
void draw_pane(WINDOW* pane_win, const Player* playerX, const Player* playerO)
{
    int pane_win_h, text_y;

    pane_win_h = getmaxy(pane_win);
    text_y = (pane_win_h - (NUM_PLAYERS * PLYR_TEXT_SECTION)) / 2;

    box(pane_win, 0, 0);

    // ******* Print text for each player ******* //
    // *** Active player highlighted in color *** //
    text_y +=print_player_info(pane_win, text_y, playerX, turn_color(playerX));
    text_y +=print_player_info(pane_win, text_y, playerX, turn_color(playerO));

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
