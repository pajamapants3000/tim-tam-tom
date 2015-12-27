/*
 * File   : form.h
 * Program: tim-tam-tom -- A tic-tac-toe game written in curses
 * Notes  : Link with -lpanel -lncurses
 * Purpose: form that prompts for username - header
 * Author : Tommy Lincoln <pajamapants3000@gmail.com>
 * License: MIT -- See LICENSE
 * Notes  : Link with -lform -lncurses
 */

#ifndef GUARD_FORM_USERNAME_H
#define GUARD_FORM_USERNAME_H

#include <ncurses.h>
#include <form.h>

// form window
// size beyond enclosed form/subwindow
#define FORM_WIN_dH 10
#define FORM_WIN_dW 10
// position of form window
#define FORM_WIN_Y 10
#define FORM_WIN_X ((COLS  - FORM_WIN_dW - NAME_MAXLEN - 2) / 2)
// placement of form/subwindow within form window
//#define FORM_SUBWIN_Y (FORM_WIN_dH / 2)
#define FORM_SUBWIN_Y 2
#define FORM_SUBWIN_X (FORM_WIN_dW / 2)
// NOTE: may want FORM_WIN_dH = 2 * FORM_SUBWIN_Y; similar for dW/X

#define KB_ENTER 10     // KEY_ENTER doesn't work for some reason
#define NUL     '\0'

// form
#define NUM_FIELDS 2
#define QUESTION "   Hi! I'm Tom.       What's your name? "
#define NAME_MAXLEN 20

struct Prompt {
    FIELD** fields;
    FORM* form;
    WINDOW* form_win;
    WINDOW* form_subwin;
};

/*********************************************************************
 * set_name: reads the buffer of the field passed as the first       *
 *           argument and stores the nonspace characters in the      *
 *           char* passed in the second argument; crops any leading  *
 *           space and retains only at most a single space if any is *
 *           present between characters; that is, it allows for      *
 *           multiple, space-separated names. Returns any error      *
 *           message obtained from reading the buffer, otherwise 0.  *
 *  depends: <ncurses.h>, <form.h>, <ctype.h> (isspace), <errno.h>   *
 *           (errno), and NAME_MAXLEN                                *
 *********************************************************************/
int set_name(FIELD* field, char* username);

/*********************************************************************
 * set_name_as_is: reads the buffer from field passed in first arg   *
 *                 and copies char-for-char to the char* passed in   *
 *                 second argument. Returns any error returned on    *
 *                 on reading buffer, otherwise returns 0.           *
 *     Depends on: <ncurses.h>, <form.h>, and NAME_MAXLEN            *
 *********************************************************************/
int set_name_as_is(FIELD* field, char* username);

/*********************************************************************
 * process_input: takes the form taking input as the first arg, and  *
 *                a single input character (chtype) as the second;   *
 *                applies characters, spaces, and numbers to the     *
 *                field, handles backspace and delete, and returns   *
 *                1 on pressing enter, indicating input has          *
 *                completed, or returns 0 otherwise.                 *
 *    depends on: <ncurses.h>, <form.h>, <stdbool.h>, and KB_ENTER   *
 *********************************************************************/
bool process_input(FORM* prompt_for_name, chtype ch);

/*********************************************************************
 * draw_form: creates and displays a form, returning a struct        *
 *            Prompt* pointing to it, to allow the program to        *
 *            interact with it. Must call free_form_mem to free the  *
 *            memory allocated here.                                 *
 *Depends on: <ncurses.h>, <form.h>, and <stdlib.h> (malloc). Also   *
 *            calls create_fields, set_field_attrs, create_form,     *
 *            create_form_win, create_form_subwin, and               *
 *            set_win_subwin_attrs.                                  *
 *********************************************************************/
struct Prompt* draw_form();

/*********************************************************************
 * create_fields: creates/returns the fields to be used the the form;*
 *    depends on: <ncurses.h>, <form.h>, <stdlib.h> (malloc), and    *
 *                the macros NUM_FIELDS and NAME_MAXLEN              *
 *********************************************************************/
FIELD** create_fields();

/*********************************************************************
 * set_field_attrs: takes a set of fields and assigns attributes to  *
 *                  them; as currently written, this function is     *
 *                  entirely hardcoded.                              *
 *      Depends on: <ncurses.h> and <form.h>.                        *
 *********************************************************************/
void set_field_attrs(FIELD** fields);

/*********************************************************************
 * create_form: trivial creation of a form from set of fields passed *
 *              in; returns a pointer to the created form;           *
 *  Depends on: <ncurses.h> and <form.h>.                        *
 *********************************************************************/
FORM* create_form(FIELD** fields);

/*********************************************************************
 * create_form_win: creates and returns a window to contain the form *
 *                  passed in as the only argument; assigns the new  *
 *                  window to contain the form with set_form_win.    *
 *      Depends on: <ncurses.h>, <form.h>, FORM_WIN_dH, FORM_WIN_dW, *
 *                  FORM_WIN_Y, and FORM_WIN_X.                      *
 *********************************************************************/
WINDOW* create_form_win(FORM* prompt_for_name);

/*********************************************************************
 * create_form_subwin: takes a form and containing window as the     *
 *                     first and second arguments, respectively;     *
 *                     creates a subwindow within the window to      *
 *                     contain the form, and returns a pointer to the*
 *                     subwindow; calls set_form_sub on it to assign *
 *                     it to the form;                               *
 *          Depends on: <ncurses.h>, <panel.h>, and macros           *
 *                     FORM_SUBWIN_Y and FORM_SUBWIN_X.              *
 *********************************************************************/
WINDOW* create_form_subwin(FORM* prompt_for_name, WINDOW* form_win);

/*********************************************************************
 * set_win_subwin_attrs: takes the form-containing window and subwin *
 *                       as the first and second argument, resp.;    *
 *                       sets their attibutes and additional contents*
 *            Depends on: <ncurses.h>, <form.h>                      *
 *********************************************************************/
void set_win_subwin_attrs(WINDOW* form_win, WINDOW* form_subwin);

/*********************************************************************
 * free_form_mem: frees all memory allocated by draw_form and the    *
 *                functions it calls; takes a pointer to the struct  *
 *                Prompt associated with that form.                  *
 *    Depends on: <ncurses.h>, <form.h>, <stdlib.h> (free)           *
 *********************************************************************/
void free_form_mem(struct Prompt* Prompt_for_Name);

/*********************************************************************
 * get_username: Once the prompt is created and displayed, call this *
 *               function to collect the input, store the result in  *
 *               the desired char ptr/array, and erase the buffer    *
 *               to finish up and prepare for the possibility of     *
 *               collecting another name.                            *
 *   Depends on: <ncurses.h>, <form.h>, NUM_FIELDS, struct Prompt,   *
 *               process_input, set_name.                            *
 *********************************************************************/
void get_username(struct Prompt* Prompt_for_Name, char* username);

#endif      // end of header guard

