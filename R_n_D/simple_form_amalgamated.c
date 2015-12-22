// simple_form_amalgamated.c
// simple_form.c, but putting all of the code in to main, since the current
//+approach isn't working.

#include <ncurses.h>
#include <form.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#define INI_NCRS initscr(); cbreak(); noecho();

#define MAXLEN 20
#define NUM_FIELDS 2
#define QUESTION "Hello! My name is Tommy,\nwhat's yours?"

struct Prompt {
    FIELD** fields;
    FORM* form;
    WINDOW* form_win;
    WINDOW* form_subwin;
};
// type fits return types of field_buffer, which will be called by set_name
//+and forwarded back to its caller.
// Copies contents of buffer to username variable, cropping whitespace;
signed short set_name_no_space(char username[], FIELD* field);
signed short set_name_as_is(char username[], FIELD* field);
bool process_input(chtype ch, FORM* prompt_for_name);
bool is_space(char ch);
struct Prompt* draw_form();
FIELD** set_fields();
void set_field_attrs(FIELD** fields);
FORM* create_form(FIELD** fields);
WINDOW* create_win(FORM* prompt_for_name, int rows, int cols);
WINDOW* create_subwin(FORM* prompt_for_name, WINDOW* form_win,
        int rows, int cols);
void set_win_subwin_attrs(WINDOW* form_win, WINDOW* form_subwin);
void free_mem(struct Prompt* Prompt_for_Name);

int main(void)
{
    char username[MAXLEN];
    struct Prompt* Prompt_for_Name;

    INI_NCRS;

    Prompt_for_Name = draw_form();

    while (!process_input(getch(), Prompt_for_Name->form))
        ;

    set_name_as_is(username, *(Prompt_for_Name->fields + 1));

    endwin();
    return 0;
}


signed short set_name_no_space(char username[], FIELD* field)/*{{{*/
{
    char* buffer = field_buffer(field, 0);  // point to the field buffer!
    signed short retval = errno;    // store any error value from field_buffer

    if (buffer == NULL)
        return retval;              // field_buffer encountered an error

    unsigned short i = 0;           // fixed buffer size - keep track of pos
    while (isspace(*(buffer + i)))
        ++i;
    do username[i] = *(buffer + i);
    while (i < MAXLEN && !isspace(*(buffer + i++)));

    return 0;
}
/*}}}*/
signed short set_name_as_is(char username[], FIELD* field)/*{{{*/
{
    char* buffer = field_buffer(field, 0);  // point to the field buffer!
    signed short retval = errno;    // store any error value from field_buffer

    if (buffer == NULL)
        return retval;              // field_buffer encountered an error

    unsigned short i = 0;           // fixed buffer size - keep track of pos
    do username[i] = *(buffer + i);
    while (++i < MAXLEN);

    return 0;
}
/*}}}*/
bool process_input(chtype ch, FORM* prompt_for_name)/*{{{*/
{
    static unsigned short n = 0;    // number of characters entered
    static unsigned short pos = 0;  // cursor precedes last char by pos chars

    if (isalpha(ch))
    {
        form_driver(prompt_for_name, ch);
        ++n;
        if (n == MAXLEN)
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
        case KEY_ENTER:
            return 1;
        default:
            break;
    }
    return 0;
}
/*}}}*/
struct Prompt* draw_form()/*{{{*/
{
    struct Prompt* P4N_ptr =
        (struct Prompt*) malloc(sizeof(struct Prompt));
    int rows, cols;

    P4N_ptr->fields = set_fields();
    set_field_attrs(P4N_ptr->fields);
    P4N_ptr->form = create_form(P4N_ptr->fields);
    scale_form(P4N_ptr->form, &rows, &cols);
    P4N_ptr->form_win = create_win(P4N_ptr->form, rows, cols);
    P4N_ptr->form_subwin = create_subwin(P4N_ptr->form,
            P4N_ptr->form_win, rows, cols);

    post_form(P4N_ptr->form);
    wrefresh(P4N_ptr->form_win);

    return P4N_ptr;
}
/*}}}*/
FIELD** set_fields()/*{{{*/
{
    FIELD** fields = (FIELD**) malloc((NUM_FIELDS + 1) * sizeof(FIELD*));
    FIELD** field_ptr = fields;

    *(field_ptr++) = new_field(2, MAXLEN, 2, 1, 0, 0);
    *(field_ptr++) = new_field(1, MAXLEN, 8, 1, 0, 0);
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
}
/*}}}*/
FORM* create_form(FIELD** fields)/*{{{*/
{
    FORM* prompt_for_name = new_form(fields);
    return prompt_for_name;
}
/*}}}*/
WINDOW* create_win(FORM* prompt_for_name, int rows, int cols)/*{{{*/
{
    WINDOW* form_win = newwin(rows + 4, cols + 4, 10, 8);
    set_form_win(prompt_for_name, form_win);
    return form_win;
}
/*}}}*/
WINDOW* create_subwin(FORM* prompt_for_name, WINDOW* form_win, int rows, int cols)/*{{{*/
{
    WINDOW* form_subwin = derwin(form_win, rows, cols, 2, 2);
    set_form_sub(prompt_for_name, form_subwin);
    return form_subwin;
}
/*}}}*/
void set_win_subwin_attrs(WINDOW* form_win, WINDOW* form_subwin)/*{{{*/
{
    box(form_win, 0, 0);
    wrefresh(form_win);
}
/*}}}*/
void free_mem(struct Prompt* Prompt_for_Name)/*{{{*/
{
    free_form(Prompt_for_Name->form);
    for (unsigned short i = 0; i < NUM_FIELDS; ++i)
        free_field(*(Prompt_for_Name->fields + i));
}
/*}}}*/

