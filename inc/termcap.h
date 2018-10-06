// #ifndef TERMCAP_H
// # define TERMCAP_H
#ifndef TERM_H
# define TERM_H
# include <term.h>
# include <termios.h>
# include <sys/ioctl.h>

typedef struct termios t_term;
static char term_buffer[2048];

/*
*** - Structure that contains all the required info
*** - to manage the termcaps
*/

typedef struct          s_tcap
{
    int             sz_str; // size of the str
    int             size_prompt;// size of the prompt
    int             cursor;//position cursor regarding the string
    int             window_size[2]; // row in [0], col in [1]
    char            buf[5]; // String that contains the return of the read
    char            **str; // string that gets constantly realloc'd    
    char            *res; // return the tgetstr function
    int             curs_pos[2]; //Cursor position, X in [0], Y in [1]
    int             y_prompt; // y position of the prompt at all time
    int             char_pos[2]; // X(0) and Y(1) position in the window of any char of the string
    int             x_lines[3]; // (0) == x of line before cursor, (1) == x of line cursor, (2) x of line after
    char            last_char; // char for the return of the position char in window function. It's the char right before the passage to the next line
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
    char            key3; // char that contains value of buf[2] to be compared
    char            key4;
    char            *cmd; // string that contains the cmd for tgetstr
}                       t_tab;

void        cursor_position(int curs_pos[2]);
int 		right_key(t_tcap *caps);
int 		left_key(t_tcap *caps);
void		position_char_in_window_left_alt_keys(int pos, t_tcap *caps, int curs_pos[2]);
void		position_char_in_window_print_inside_string(int pos, t_tcap *caps, int end);

#endif