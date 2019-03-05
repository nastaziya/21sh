/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   termcap.h                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/06 01:05:33 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/28 15:54:09 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef TERMCAP_H
# define TERMCAP_H
# include <term.h>
# include <termios.h>
# include <sys/ioctl.h>
# include "../libft/libft.h"

typedef struct termios	t_term;

/*
*** - Structure that contains all the required info
*** - to manage the termcaps :
*** -
*** - int	i; // i used for the print function - helps for the initialization
*** - int	sz_str; // size of the str
*** - int	size_prompt;// size of the prompt
*** - int	cursor;//position of the cursor in the string
*** - int	window_size[2]; // row in [0], col in [1]
*** - char	buf[2048];
*** - char	**str; // string that gets constantly realloc'd
*** - char	*res; // return the tgetstr function
*** - int	curs_pos[2]; //Cursor position, X in [0], Y in [1]
*** - int	y_prompt; // y position of the prompt at all time
*** - int	char_pos[2]; // X(0) and Y(1) position in the window of any char
*** - of the string
*** - int	x_lines[3]; // (0) == x of line before cursor, (1) == x of line
*** - cursor, (2) x of line after
*** - char	last_char; // char for the return of the position char in window
*** - function. It's the char right before the passage to the next line
*** - char	*tmp_str; // str that is a copy of the current str when in history
*** - t_dlist	**history;// history - to be able to pass it to the pointer
*** - of functions
*** - int		ct_arrow; // count the number of arrows that were used,
*** - 		1 == last arrow was down key ; 2 == last arrow was up key
*** - char		*copy_str; // copy of str when copy_paste
*** - char		*prompt; // copy of the prompt str for the resize
*** - of the window management
*** - int		keeprun -> manage ctrl_c
*/

typedef struct			s_tcap
{
	int				i;
	int				sz_str;
	int				size_prompt;
	int				cursor;
	int				window_size[2];
	char			buf[2048];
	char			**str;
	char			*res;
	int				curs_pos[2];
	int				y_prompt;
	int				char_pos[2];
	int				x_lines[3];
	char			last_char;
	char			*tmp_str;
	t_dlist			**history;
	int				ct_arrow;
	char			*copy_str;
	char			*prompt;
}						t_tcap;

t_tcap					g_caps;

/*
*** - Structure that will be the array
*** - of function pointers
*** - typedef struct          s_tab
*** - {
*** - int				(*ptr)(t_tcap *caps); // pointer to the function
*** - that manages the specific pressed key
*** - char			key0; // char that contains value of buf[0] to be compared
*** - char			key1; // char that contains value of buf[1] to be compared
*** - char			key2; // char that contains value of buf[2] to be compared
*** - char			key3; // char that contains value of buf[2] to be compared
*** - char			key4;
*** - char			*cmd; // string that contains the cmd for tgetstr
*** - }						t_tab;
*/

typedef struct			s_tab
{
	int				(*ptr)(t_tcap *caps);
	char			key0;
	char			key1;
	char			key2;
	char			key3;
	char			key4;
	char			*cmd;
}						t_tab;

int					g_keeprun;

/*
** - cursor_position.c
*/

void					cursor_position(int curs_pos[2]);

/*
*** - utils.c
*/

int						ft_outc(int c);
void					initialize_caps(t_tcap *caps, char *prompt);
void					size_windows(t_tcap *caps);

/*
*** - left_right_key.c
*/

int						right_key(t_tcap *caps);
int						left_key(t_tcap *caps);

/*
*** - del_key.c
*/

int						del_key(t_tcap *caps);

/*
*** - pos_char.c
*/
void					position_char_in_window_left_alt_keys(t_tcap *caps,
							int curs_pos[2]);
int						pos_char_in_window_in_str(int pos,
							t_tcap *caps, int end, int bulean);

/*
*** - home_end.c
*/

int						home_key(t_tcap *caps);
int						end_key(t_tcap *caps);

/*
*** - termios.c
*/

int						terminal_data (t_term *term);
int						modify_terminos(t_term *term);
int						reset_termios(t_term *term);

/*
*** - alt_up_down_key.c
*/

int						alt_up_key(t_tcap *caps);
int						alt_down_key(t_tcap *caps);

/*
*** - print.c
*/

int						print_normal_char(t_tcap *caps);
void					print_end_line(t_tcap *caps, char *string, char *tmp);
int						print_buf(t_tcap *caps, char *buf);

/*
*** - print_2.c
*/

void					print_middle_line(t_tcap *caps, char *string, char *tmp,
							char *tmp2);
int						check_if_scroll(t_tcap *caps, char *str, int i);

/*
*** -  termcap.c - MAIN FILE
*/

int						get_term(char **cmd, char *str, t_dlist **history,
							t_tcap *caps);

/*
*** - up_down_key.c
*/

int						up_key(t_tcap *caps);
int						down_key(t_tcap *caps);

/*
*** - alt_right_left_key.c
*/

int						alt_right_key(t_tcap *caps);
int						alt_left_key(t_tcap *caps);

/*
*** - alt_copy.c
*/

int						alt_x(t_tcap *caps);
int						alt_s(t_tcap *caps);
int						alt_w(t_tcap *caps);
int						alt_s(t_tcap *caps);
int						alt_p(t_tcap *caps);
int						ctrl_dnorm(t_tcap *caps);

/*
*** - control_commands.c
*/

int						ctrl_l(t_tcap *caps);
int						ctrl_d(t_tcap *caps);

/*
*** - signals.c
*/

int						initialize_signals(void);
int						ctrl_c(t_tcap *caps);


/*
*** - print_buf.c
*/

int						print_buf(t_tcap *caps, char *buf);

/*
*** - win_resize.c
*/

// void					win_resize(int sig);
void					win_resize(t_tcap *caps);

/*
*** - norm_termcap.c
*/

int						new_line(t_tcap *caps);
int						ctrlld(t_tcap *caps);
int						equality(t_tcap *caps, t_tab *tmp_tab);

#endif
