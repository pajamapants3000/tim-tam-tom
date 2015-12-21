// simple_form.c
// A simple form is presented to the user, asking for their name; once
//+they have submitted an acceptable string, a message says hi and then the
//+program exits.

#include <ncurses.h>
#include <form.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#define INI_NCRS initscr(); cbreak(); noecho();

#define MAXLEN 20
#define Question "Hello! My name is Tommy, what's yours?"

// type fits return types of field_buffer, which will be called by set_name
//+and forwarded back to its caller.
// Copies contents of buffer to username variable, cropping whitespace;
signed short set_name_no_space(char username[], FIELD* field);
signed short set_name_as_is(char username[], FIELD* field);
bool process_input(char ch, FORM* prompt_for_name)
bool isspace(char ch);

int main(void)
{

    return 0;
}


signed short set_name_no_space(char username[], FIELD* field)
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
signed short set_name_as_is(char username[], FIELD* field)
{
    char* buffer = field_buffer(field, 0);  // point to the field buffer!
    signed short retval = errno;    // store any error value from field_buffer

    if (buffer == NULL)
        return retval;              // field_buffer encountered an error

    unsigned short i = 0;           // fixed buffer size - keep track of pos
    do username[i] = *(buffer + i);
    while (++i < MAXLEN)

    return 0;
}
