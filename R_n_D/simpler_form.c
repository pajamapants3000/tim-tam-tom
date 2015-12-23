// simpler_form.c
// direct construction of form all in main

#include <ncurses.h>
#include <form.h>

int main(void)
{
//    FORM* my_form;
//    FIELD* my_field[2];
//    WINDOW *form_win, *form_subwin;
    WINDOW *form_win;
    int x=10, y=10;

    initscr();
    cbreak();
    refresh();
/*
    my_field[0] = new_field(1, 20, 6, 1, 0, 0);
    my_field[1] = NULL;

    my_form = new_form(my_field);
    scale_form(my_form, &y, &x);
*/

    form_win = newwin(4, x + 4, 5, 5);
//    form_subwin = derwin(form_win, y, x, 2, 2);
    box(form_win, 0, 0);
 /*   set_field_buffer(my_field[0], 0, "Hey there!");
    set_form_win(my_form, form_win);
//    set_form_sub(my_form, form_subwin);
    post_form(my_form);
*/
    wrefresh(form_win);
    refresh();

    getch();

    endwin();
    return 0;
}

