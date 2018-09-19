// #ifndef TERMCAP_H
// # define TERMCAP_H
#ifndef TERM_H
# define TERM_H
# include <term.h>
# include <termios.h>

typedef struct termios t_term;
static char term_buffer[2048];

/*
*** - Structure that contains all the required info
*** - to manage the termcaps
*/

typedef struct          s_tcap
{
    int             sz_str; // size of the str
    int             cursor;
    char            buf[3]; // String that contains the return of the read
    char            **str; // string that gets constantly realloc'd    
    char            *res; // return the tgetstr function
    char            curs_pos[2]; //Cursor position, X in [0], Y in [1]
}                       t_tcap;

/*
*** - Structure that will be the array 
*** - of function pointers
*/

typedef struct          s_tab
{
    int             (*ptr)(t_tcap *caps); // pointer to the function that manages the specific pressed key
    char            key0; // char that contains value of buf[0] to be compared
    char            key1; // char that contains value of buf[1] to be compared
    char            key2; // char that contains value of buf[2] to be compared
    char            *cmd; // string that contains the cmd for tgetstr
}                       t_tab;

#endif