// #ifndef TERMCAP_H
// # define TERMCAP_H
#ifndef TERM_H
# define TERM_H

# include <term.h>
# include <termios.h>
# include <sys/ioctl.h>
# include "../libft/libft.h"

typedef struct termios t_term;
static char term_buffer[2048];

# define ENTER_KEY (caps.buf[0] == 10 && caps.buf[1] == 0 && caps.buf[2] == 0 && caps.buf[3] == 0 && caps.buf[4] == 0)
# define CTRL_L_KEY (caps.buf[0] == 12 && caps.buf[1] == 0 && caps.buf[2] == 0 && caps.buf[3] == 0 && caps.buf[4] == 0)
# define CTRL_D_KEY (caps.buf[0] == 4 && caps.buf[1] == 0 && caps.buf[2] == 0 && caps.buf[3] == 0 && caps.buf[4] == 0)

# define BUF_EQUALS_ARRAY (caps.buf[0] == tmp_tab->key0 && caps.buf[1] == tmp_tab->key1 && caps.buf[2] == tmp_tab->key2 && caps.buf[3] == tmp_tab->key3 && caps.buf[4] == tmp_tab->key4)
/*
*** - Structure that contains all the required info
*** - to manage the termcaps
*/

typedef struct          s_tcap
{
    int             i; // i used for the print function - helps for the initialization
    int             sz_str; // size of the str
    int             size_prompt;// size of the prompt
    int             cursor;//position of the cursor in the string
    int             window_size[2]; // row in [0], col in [1]
    char            buf[5]; // String that contains the return of the read
    char            **str; // string that gets constantly realloc'd    
    char            *res; // return the tgetstr function
    int             curs_pos[2]; //Cursor position, X in [0], Y in [1]
    int             y_prompt; // y position of the prompt at all time
    int             char_pos[2]; // X(0) and Y(1) position in the window of any char of the string
    int             x_lines[3]; // (0) == x of line before cursor, (1) == x of line cursor, (2) x of line after
    char            last_char; // char for the return of the position char in window function. It's the char right before the passage to the next line
    char            *tmp_str; // str that is a copy of the current str when in history
    t_dlist         **history;// history - to be able to pass it to the pointer of functions
    int             ct_arrow; // count the number of arrows that were used, 1 == last arrow was down key ; 2 == last arrow was up key
    char            *copy_str; // copy of str when copy_paste
}                       t_tcap;

// typedef struct		s_dlist
// {
// 	void			*content;
// 	struct s_dlist	*next;
// 	struct s_dlist	*prev;
// }					t_dlist;

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

// cursor_position.c
void        cursor_position(int curs_pos[2]);

// utils.c
int			ft_outc(int c);
void		initialize_caps(t_tcap *caps, char *prompt);
void		size_windows(t_tcap *caps);

//left_right_key.c
int 		right_key(t_tcap *caps);
int 		left_key(t_tcap *caps);

// del_key.c
int			del_key(t_tcap *caps);// -> Faire suppr key

// pos_char.c
void		position_char_in_window_left_alt_keys(int pos, t_tcap *caps, int curs_pos[2]);
// void		position_char_in_window_print_inside_string(int pos, t_tcap *caps, int end);
int		    position_char_in_window_print_inside_string(int pos, t_tcap *caps, int end, int bulean);

// home_end.c
int			home_key(t_tcap *caps);
int			end_key(t_tcap *caps);


// termios.c
int         terminal_data (t_term *term);
int         modify_terminos(t_term *term);
int         reset_termios(t_term *term);

// alt_up_down_key.c
int			alt_up_key(t_tcap *caps);
int			alt_down_key(t_tcap *caps);

//print.c
int			     print_normal_char(t_tcap *caps);
void            print_end_line(t_tcap *caps, char *string, char *tmp);


// termcap.c - MAIN FILE
int         get_line_term(char **cmd, char *str, t_dlist **history);

// up_down_key.c
int			     up_key(t_tcap *caps);
int			     down_key(t_tcap *caps);

// alt_right_left_key.c
int 		alt_right_key(t_tcap *caps);
int 		alt_left_key(t_tcap *caps);

// alt_copy.c
int 		alt_x(t_tcap *caps);
int 		alt_s(t_tcap *caps);
int 		alt_w(t_tcap *caps);
int 		alt_s(t_tcap *caps);
int 		alt_p(t_tcap *caps);

// control_commands.c
int			ctrl_l(t_tcap *caps);

#endif