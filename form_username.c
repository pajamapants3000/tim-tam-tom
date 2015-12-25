/*
 * File   : form.c
 * Program: tim-tam-tom
 * Purpose: form that prompts for username - lib
 * Author : Tommy Lincoln <pajamapants3000@gmail.com>
 * License: MIT -- See LICENSE
 */

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include "form_username.h"

int set_name(FIELD* field, char* username)/*{{{*/
{
    unsigned short i = 0;                   // position wrt fixed buffer size
    char* buffer = field_buffer(field, 0);  // point to the field buffer!
    signed short retval = errno;            // store field_buffer error value

    if (buffer == NULL)
        return retval;                      // field_buffer encountered error

    while (isspace(*(buffer + i)))          // skip leading whitespace
        ++i;
    for (; i < NAME_MAXLEN; i++)            // read remaining buffer characters
    {
        if (!isspace(*(buffer + i)))        // copy non-ws chars to username
            *username++ = *(buffer + i);
        else if (!isspace(*(username - 1))) // copy at most one whitespace char
            *username++ = *(buffer + i);
    }
    if (isspace(*(username - 1)))           // eliminate trailing whitespace
        *(username - 1) = NUL;

    return 0;
}
/*}}}*/
int set_name_as_is(FIELD* field, char* username)/*{{{*/
{
    char* buffer = field_buffer(field, 0);  // point to the field buffer!
    int retval = errno;                     // store field_buffer error value

    if (buffer == NULL)
        return retval;                      // field_buffer encountered error

    for (int i = 0; i < NAME_MAXLEN; i++)   // copy buffer directly to username
        *(username + i) = *(buffer + i);

    return 0;
}
/*}}}*/
bool process_input(FORM* prompt_for_name, chtype ch)/*{{{*/
{
    static unsigned short n = 0;    // number of characters entered
    static unsigned short pos = 0;  // cursor precedes last char by pos chars

    if (isalnum(ch) || ch == ' ')
    {
        form_driver(prompt_for_name, ch);
        ++n;
        if (n == NAME_MAXLEN)
            return 1;
        else
            return 0;
    }

    switch (ch)
    {
        case KEY_DOWN: case KEY_RIGHT:
            if (pos != 0)
            {
                form_driver(prompt_for_name, REQ_NEXT_CHAR);
                --pos;
            }
            break;
        case KEY_UP: case KEY_LEFT:
            if (pos != n)
            {
                form_driver(prompt_for_name, REQ_PREV_CHAR);
                ++pos;
            }
            break;
        case KEY_BACKSPACE:
            if (pos != n)
            {
                form_driver(prompt_for_name, REQ_PREV_CHAR);
                form_driver(prompt_for_name, REQ_DEL_CHAR);
                --n;
            }
            break;
        case KEY_DC:
            if (pos != 0)
            {
                form_driver(prompt_for_name, REQ_DEL_CHAR);
                --n;
                --pos;
            }
            break;
        case KB_ENTER:
            if (form_driver(prompt_for_name, REQ_VALIDATION)
                    == E_INVALID_FIELD)
            {
                form_driver(prompt_for_name, REQ_CLR_FIELD);
                form_driver(prompt_for_name, REQ_BEG_FIELD);
                n = 0;
                pos = 0;
                move(1, 1);
                printw("Error in entry, try again!");
                refresh();
                return 0;
            }
            else
                return 1;
        default:
            break;
    }
    return 0;
}
/*}}}*/
struct Prompt* draw_form()/*{{{*/
{
    struct Prompt* P4N_ptr =                            // Prompt-4-Name
        (struct Prompt*) malloc(sizeof(struct Prompt));
    int rows, cols;

    P4N_ptr->fields = create_fields();
    set_field_attrs(P4N_ptr->fields);
    P4N_ptr->form = create_form(P4N_ptr->fields);
    scale_form(P4N_ptr->form, &rows, &cols);
    P4N_ptr->form_win = create_form_win(P4N_ptr->form);
    P4N_ptr->form_subwin =create_form_subwin(P4N_ptr->form, P4N_ptr->form_win);
    set_win_subwin_attrs(P4N_ptr->form_win, P4N_ptr->form_subwin);

    post_form(P4N_ptr->form);
    wrefresh(P4N_ptr->form_win);

    return P4N_ptr;
}
/*}}}*/
FIELD** create_fields()/*{{{*/
{
    FIELD** fields = (FIELD**) malloc((NUM_FIELDS + 1) * sizeof(FIELD*));
    FIELD** field_ptr = fields;

    *(field_ptr++) = new_field(2, NAME_MAXLEN, 2, 1, 0, 0);
    *(field_ptr++) = new_field(1, NAME_MAXLEN, 8, 1, 0, 0);
    *field_ptr = NULL;

    return fields;
}
/*}}}*/
void set_field_attrs(FIELD** fields)/*{{{*/
{
    // The first field will display a two-line message; uneditable;
    set_field_buffer(*fields, 0, QUESTION);
    set_field_opts(*fields, O_VISIBLE | O_PUBLIC);
    set_field_just(*fields, JUSTIFY_CENTER);

    // go to next field
    ++fields;

    // this is where the user will enter their name
    set_field_just(*fields, JUSTIFY_LEFT);
    set_field_back(*fields, A_UNDERLINE);
    set_field_type(*fields, TYPE_REGEXP, "^[a-zA-Z ]* *$");
}
/*}}}*/
FORM* create_form(FIELD** fields)/*{{{*/
{
    FORM* prompt_for_name = new_form(fields);
    return prompt_for_name;
}
/*}}}*/
WINDOW* create_form_win(FORM* prompt_for_name)/*{{{*/
{
    int form_rows, form_cols;
    scale_form(prompt_for_name, &form_rows, &form_cols);
    WINDOW* form_win = newwin(form_rows + FORM_WIN_dH, form_cols + FORM_WIN_dW,
            FORM_WIN_Y, FORM_WIN_X);
    set_form_win(prompt_for_name, form_win);
    return form_win;
}
/*}}}*/
WINDOW* create_form_subwin(FORM* prompt_for_name, WINDOW* form_win)/*{{{*/
{
    int rows, cols;
    scale_form(prompt_for_name, &rows, &cols);
    WINDOW* form_subwin = derwin(form_win, rows, cols,
            FORM_SUBWIN_Y, FORM_SUBWIN_X);
    set_form_sub(prompt_for_name, form_subwin);
    return form_subwin;
}
/*}}}*/
void set_win_subwin_attrs(WINDOW* form_win, WINDOW* form_subwin)/*{{{*/
{
    box(form_win, 0, 0);
    keypad(form_win, TRUE);
}
/*}}}*/
void free_form_mem(struct Prompt* Prompt_for_Name)/*{{{*/
{
    free_form(Prompt_for_Name->form);
    for (unsigned short i = 0; i < NUM_FIELDS; ++i)
        free_field(*(Prompt_for_Name->fields + i));
    delwin(Prompt_for_Name->form_win);
    delwin(Prompt_for_Name->form_subwin);
    free(Prompt_for_Name);
}
/*}}}*/
