#include "../../inc/sh.h"
// #include "../../inc/termcap.h"

/*
*** - Aim of the function :
*** - Putstr from index i to j
*/
// void	ft_putstr_i_to_j(char *str, int i, int j, int fd)
// {
// 	--i;
// 	while (++i < j)
// 		ft_putchar_fd(str[i], fd);
// }

/*
*** - Aim of the function :
*** - Obligatory to use tputs, as ft_putchar is (void*) type
*** - And tputs requires an int function (for the pointer function)
*/
int			ft_outc(int c)
{
  ft_putchar(c);
  return (0);
}

/*
*** - Aim of the function :
*** - Collect terminal data
*/
int     terminal_data (t_term *term)
{
  char   *name_term;
 
  if ((name_term = getenv("TERM")) == NULL)
     return (-1);
  if (tgetent(NULL, name_term) <= 0)
     return (-1);
  if (tcgetattr(0, term) == -1)
     return (-1);
  return (0);
}

/*
*** - Aim of the function :
*** - Modify the output options of the current terminal
*/
int     modify_terminos(t_term *term)
{
  term->c_lflag &= ~(ICANON);
  term->c_lflag &= ~(ECHO);
	// term->c_lflag &= ~(ECHO | ICANON);
  term->c_cc[VMIN] = 1;
  term->c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSADRAIN, term) == -1)
    return (-1);
//   tputs(tgetstr("sc", NULL), 0, ft_outc);
  return (0);
}

int		reset_termios(t_term *term)
{
	term->c_lflag = (ICANON | ECHO);
	if (tcsetattr(0, TCSADRAIN, term) == -1)
    	return (-1);
	return (0);
}

void		initialize_caps(t_tcap *caps, char *prompt)
{
	int 	tmp[2];
	caps->size_prompt = ft_strlen(prompt);
	caps->sz_str = caps->size_prompt;
	caps->cursor =  caps->size_prompt;
	cursor_position(tmp);
	caps->y_prompt = tmp[1];
	caps->i = 0;
	// dprintf(2, "YEEEEAAAH - %d\n", caps->y_prompt);
	if (!(caps->str = (char**)malloc(sizeof(char*))))
		return ;
}

/*
*** - Aim of the function : Collect at a given moment the
***	- position of the cursor - I use it to manage the size of 
*** - the cursor at any given time + at the beginning to 
*** - manage the history (after)
*/

void		cursor_position(int curs_pos[2])
{
 	char 	buf[32];
 	int		i;
 	int 	j;
	
	i = 0;
	j = 2;
	ft_bzero(buf, 32);
	        // fflush(stdout);

	if (write(0, "\x1b[6n", 4) != 4)
		return ;
  	while (i < sizeof(buf)-1)
  	{
        if (read(0,buf+i,1) != 1)
        	break;
        if (buf[i] == 'R')
        	break;
        i++;
    }
    buf[i] = '\0';
	if (!ft_isdigit(*(buf + 2)))
		return ;
	curs_pos[1] = ft_atoi(buf + 2);
	while (buf[j] && buf[j] != 59)
		j++;
	curs_pos[0] = ft_atoi(buf + j + 1);
	// dprintf(2, "DEBUG CURSOR: |%s|, [%d], [%d] -\n", buf + 2, curs_pos[1], curs_pos[0]);
}

/*
*** - Aim of the function : Collect the size of the
***	- window when asked
*/
void		size_windows(t_tcap *caps)
{
	struct winsize *w;

	if (!(w = (struct winsize *)malloc(sizeof(struct winsize))))
		return ;
	ioctl(STDOUT_FILENO,  TIOCGWINSZ, w);
	caps->window_size[0] = w->ws_row;
	caps->window_size[1] = w->ws_col;
	free(w);
}

int 		left_key(t_tcap *caps)
{
	int  curs_pos[2];
	if (caps->cursor > caps->size_prompt)
	{
		cursor_position(curs_pos);
		size_windows(caps);
		//DEBUG//
		// dprintf(2, "--[0: %c], [-1: %c], [deb: %c]--", caps->str[0][caps->cursor], caps->str[0][caps->cursor - 1], caps->str[0][caps->cursor - 3]);
		// dprintf(2, "SIZE : %d - %d - %c\n", caps->sz_str, caps->cursor, caps->str[0][caps->cursor- caps->size_prompt - 1]);
		// int i = -1;
		// while (++i < caps->cursor)
		// 	dprintf(2, "|%c - %d|\n", caps->str[0][i], caps->str[0][i]);
		
		// tputs(tgetstr("vi", NULL), 1, ft_outc);
		// dprintf(2, "gauche fin ligne avant : [%d - %d - %d - %d - %d]\n",caps->cursor, curs_pos[0], curs_pos[1], caps->window_size[1], caps->sz_str);
		// if (caps->str[0][caps->cursor - caps->size_prompt] != '\n')
		// {
			// if (curs_pos[0] == 1)
			dprintf(2, "position: %d\n", curs_pos[0]);
			if (curs_pos[0] == 1)
			{
				// if (caps->str[0][caps->cursor - caps->size_prompt - 1] == '\n')
				// dprintf(2, "passe ici %d", curs_pos[1]);
				// tputs(tgetstr("sf", NULL), 1, ft_outc);
				// {
					dprintf(2, "coucouuuumabiche\n");
					dprintf(2, "debug: cursor %d - curs_pos[0] %d - curs_pos[1] %d\n", caps->cursor, curs_pos[0], curs_pos[1]);
					position_char_in_window_left_alt_keys(caps->cursor, caps, curs_pos);
					// dprintf(2, "pos, end char : x:|%d| && y: |%d|\n", caps->char_pos[0], caps->char_pos[1]);
					// dprintf(2, "win, end char : winx:|%d| && winy: |%d|\n", caps->window_size[1], caps->window_size[0]);				
					// if (curs_pos[0] == 1)
					tputs(tgoto(tgetstr("cm", NULL), caps->x_lines[0], curs_pos[1] - 2), 1, ft_outc);
				// }
				// else
					// tputs(tgoto(tgetstr("cm", NULL), caps->window_size[1] - 1, curs_pos[1] - 2), 1, ft_outc);
			}
			else
			{
				tputs(tgetstr("le", NULL), 1, ft_outc);
			}
		// }
		// else
		// {
		// 	dprintf(2, "yiha\n");
		// 	tputs(tgoto(tgetstr("cm", NULL), (caps->cursor - 1 % caps->window_size[1]), curs_pos[1] - 2), 1, ft_outc);
		// }
		caps->cursor--;
		// tputs(tgetstr("ve", NULL), 1, ft_outc);
		//
		// cursor_position(caps->curs_pos);
		// dprintf(2, "gauche fin ligne avant : [%d - %d - %d - %d - %d]\n",caps->cursor, curs_pos[0], curs_pos[1], caps->window_size[1], caps->sz_str);
	}
	return (0);
}

int 		right_key(t_tcap *caps)
{
	int  curs_pos[2];
	if (caps->cursor < caps->sz_str)
	{
		cursor_position(curs_pos);
		size_windows(caps);
		//
		// tputs(tgetstr("vi", NULL), 1, ft_outc);
		dprintf(2, "droite fin ligne avant : [%d - %d - %d - %d - %d]\n",caps->cursor, curs_pos[0], curs_pos[1], caps->window_size[1], caps->sz_str);
		if (curs_pos[0] == caps->window_size[1] || caps->str[0][caps->cursor - caps->size_prompt] == '\n')
		{
			dprintf(2, "passe par la porte\n");
			tputs(tgoto(tgetstr("cm", NULL), 0, curs_pos[1]), 1, ft_outc);
		}
		else
		{
			tputs(tgetstr("nd", NULL), 1, ft_outc);
		}
		caps->cursor++;
		// tputs(tgetstr("ve", NULL), 1, ft_outc);
		//
		// cursor_position(caps->curs_pos);
		// dprintf(2, "droite fin ligne avant : [%d - %d - %d - %d - %d]\n",caps->cursor, caps->curs_pos[0], caps->curs_pos[1], caps->window_size[1], caps->sz_str);
	}
	return (0);
}

/*
*** - NEED TO NORM THE FUNCTION, BUT WORKS PROPERLY
*/

int			print_normal_char(t_tcap *caps)
{
	char		*tmp;
	char		*tmp2;
	char		*string;
	// static int	i = 0;

	if (caps->buf[0] >= 0 && caps->buf[0] <= 127 && caps->buf[1] == 0)
	{
		string = ft_strndup(caps->buf, 1);
		size_windows(caps);

		// dprintf(2, "i |%d|\n", i);
		if (caps->i == 0) // Initialization of the str, the first time
		{
			caps->str[0] = string;
			++(caps->i);
			caps->sz_str++;
			caps->cursor++;
			write(1, caps->buf, 3);
		}
		else if (caps->cursor < caps->sz_str) // Manages when prints char in the middle of string
		{
			// tputs(tgetstr("vi", NULL), 1, ft_outc);
			// saves cursor position
			tputs(tgetstr("sc", NULL), 1, ft_outc);
			// manages substrings
			
			tmp2 = ft_strdup(caps->str[0] + (caps->cursor - caps->size_prompt));
			tmp = ft_strsub(caps->str[0], 0, (caps->cursor - caps->size_prompt));
			free(caps->str[0]);
			caps->str[0] = ft_strjoin(tmp, string);
			free(tmp);
			free(string);
			tmp = caps->str[0];
			caps->str[0] = ft_strjoin(tmp, tmp2);
			free(tmp);
			

			// efface le reste
			tputs(tgetstr("cd", NULL), 1, ft_outc);
			
			// usleep(50000);
			//saves cursor position again, but not in the system tmp
			cursor_position(caps->curs_pos);
			// write the new char
			write(1, caps->buf, 3);// write(1, caps->buf, 4); ??
			// prints the rest (the tmp)
			write(1, tmp2, caps->sz_str - caps->cursor);
			
			// Incrémente le compteur
			caps->sz_str++;
			
			//replace le curseur
			tputs(tgetstr("rc", NULL), 1, ft_outc);

///////// DEBUG //////////
			position_char_in_window_print_inside_string(caps->cursor, caps, caps->sz_str);
			// dprintf(2, "pos, end char : x:|%d| && y: |%d|\n", caps->char_pos[0], caps->char_pos[1]);
			// dprintf(2, "win, end char : winx:|%d| && winy: |%d|\n", caps->window_size[1], caps->window_size[0]);

			// gère la ligne en plus en fin, quand la string touche le bas de la fenêtre, au bon moment
			// et replace le curseur au bon endroit
			// Check if x char fin str et y fin str est a la fin, en bas de la fenetre, même quand \n milieu str
			dprintf(2, "DEBUUUGGGG: |%c| - |%d| - |%d|\n", caps->last_char, caps->char_pos[1], caps->window_size[0]);
			// First manages the char at the end of the string when bottom right of the window, the second part manages when \n arrives at the end of line
			if ((caps->char_pos[0] == (caps->window_size[1]) && caps->char_pos[1] == caps->window_size[0])
				|| (caps->char_pos[1] == caps->window_size[0] && caps->last_char == '\n'))
			// if (((caps->sz_str ) % (caps->window_size[1])) == 0 && ((caps->y_prompt + ((caps->sz_str ) / (caps->window_size[1])) - 1) == caps->window_size[0])) 
			/////////// AJOUTER CONDITION --> NE FAIRE que quand string est en bas de fenêtre
			{
				// if (caps->char_pos[1] == caps->window_size[0] && caps->last_char == '\n')
				// write(1, " ", 1);
				int tst[2];
				// saves position
				cursor_position(tst);
				// if cursor.y is not at the end of the window.y, go to the line at the bottom
				if (tst[1] != caps->window_size[0])
					tputs(tgoto(tgetstr("cm", NULL), tst[0] - 1, caps->window_size[0]), 1, ft_outc);
				// scroll up one time
				tputs(tgetstr("sf", NULL), 1, ft_outc);
				// if (caps->char_pos[1] == caps->window_size[0] && caps->last_char == '\n')
				// {
				// 	tputs(tgoto(tgetstr("cm", NULL), caps->window_size[1] - 1, caps->curs_pos[1] - 1), 1, ft_outc);
				// 	write(1, " ", 1);
				// }
				//diminues the value of the y_prompt, to keep correct track of the y.position of the origin
				caps->y_prompt--;
				// replace the cursor at the previous position
				tputs(tgoto(tgetstr("cm", NULL), tst[0] - 1, tst[1] - 2), 1, ft_outc);
			}
			// Gère le y-- de la pos du prompt (pour la fonction de fleche gauche), quand on est en bas de la fenetre
			else if (caps->buf[0] == '\n' && caps->char_pos[1]-1 == caps->window_size[0])
				caps->y_prompt--;

			// Déplacer le curseur à droite, et incrémente en même temps
			right_key(caps);
			free(tmp2);
			// tputs(tgetstr("ve", NULL), 1, ft_outc);
		}
		else // prints and manages when end of string
		{
			tmp = caps->str[0];
			caps->str[0] = ft_strjoin(tmp, string);
			free(tmp);
			free(string);
			caps->sz_str++;
			caps->cursor++;
			cursor_position(caps->curs_pos);
			// tputs(tgetstr("vi", NULL), 1, ft_outc);
			write(1, caps->buf, 3);
			// ft_putstr_i_to_j(caps->buf, 0, 3, 1);
			// dprintf(2, "fin ligne: [%d - %d]\n", caps->curs_pos[0], caps->window_size[1]);
			if (caps->curs_pos[0] == caps->window_size[1])
			/////////// AJOUTER CONDITION --> NE FAIRE que quand string est en bas de fenêtre
			{
				dprintf(2, "DEBUGGY: | %d - %d - %d - %d |", caps->y_prompt, caps->sz_str, (caps->window_size[1]), caps->window_size[0]);
				position_char_in_window_print_inside_string(caps->cursor, caps, caps->sz_str);
				// if ((caps->y_prompt + ((caps->sz_str ) / (caps->window_size[1])) - 1) == caps->window_size[0])
				dprintf(2, "normal string: [%d], [%d], [%d], [%d]", caps->char_pos[0] + 1, caps->window_size[1], caps->char_pos[1], caps->window_size[0]);
				if (caps->char_pos[0] + 1 == (caps->window_size[1]) && caps->char_pos[1] == caps->window_size[0])
				{
					// dprintf(2, "oh non pas la\n");
					tputs(tgetstr("sf", NULL), 1, ft_outc);
					//diminues the value of the y_prompt, to keep correct track of the y.position of the origin
					caps->y_prompt--;
				}
				tputs(tgoto(tgetstr("cm", NULL), 0, caps->curs_pos[1]), 1, ft_outc);
			}
			else if (caps->curs_pos[1] == caps->window_size[0] && caps->buf[0] == '\n')
				caps->y_prompt--;
			// DEBUG
			// position_char_in_window(caps->cursor, caps);
			// dprintf(2, "pos_char_str - cursor: x: %d y: %d\n", caps->char_pos[0], caps->char_pos[1]);
			
			
			// position_char_in_window(caps->cursor - 1, caps);
			// dprintf(2, "pos_char_str - cursor-1: x: %d y: %d\n", caps->char_pos[0], caps->char_pos[1]);
			// position_char_in_window(caps->cursor - 2, caps);
			// dprintf(2, "pos_char_str - cursor - 2: x: %d y: %d\n", caps->char_pos[0], caps->char_pos[1]);
			// tputs(tgetstr("ve", NULL), 1, ft_outc);
		}
	}
	return (0);
}

/*
*** - Aim of the function : Managing of the del key
*** - Plus realloc properly the string
*/

int			del_key(t_tcap *caps)
{
		char *tmp2;
		char *tmp;
	if (caps->cursor > caps->size_prompt)
	{
		// tputs(tgetstr("vi", NULL), 1, ft_outc);
		left_key(caps);
		// saves cursor position
		tputs(tgetstr("sc", NULL), 1, ft_outc);
		// manages substrings
		tmp2 = ft_strdup(caps->str[0] + (caps->cursor - caps->size_prompt + 1));
		tmp = ft_strsub(caps->str[0], 0, (caps->cursor - caps->size_prompt));
		free(caps->str[0]);
		caps->str[0] = ft_strjoin(tmp, tmp2);
		free(tmp);
		// efface le reste
		tputs(tgetstr("cd", NULL), 1, ft_outc);
		// prints the rest (the tmp)
		write(1, tmp2, caps->sz_str - caps->cursor);
		//replace le curseur
		tputs(tgetstr("rc", NULL), 1, ft_outc);
		free(tmp2);
		//keeps counter correct
		caps->sz_str--;
		// prints the cursor back
		// tputs(tgetstr("ve", NULL), 1, ft_outc);
	}
	return (0);
}

int			home_key(t_tcap *caps)
{
	tputs(tgetstr("vi", NULL), 1, ft_outc);
	while (caps->cursor > caps->size_prompt)
		left_key(caps);
	tputs(tgetstr("ve", NULL), 1, ft_outc);
	return (0);
}

int			end_key(t_tcap *caps)
{
	tputs(tgetstr("vi", NULL), 1, ft_outc);
	while (caps->cursor < caps->sz_str)
		right_key(caps);
	tputs(tgetstr("ve", NULL), 1, ft_outc);
	return (0);
}

int			alt_up_key(t_tcap *caps)
{
	int		curs_pos[2];
	
	size_windows(caps);
	cursor_position(curs_pos);
	position_char_in_window_left_alt_keys(caps->cursor, caps, curs_pos);
	if (curs_pos[1] > caps->y_prompt) // && curs_pos[1] < ((caps->y_prompt) + (caps->sz_str / (caps->window_size[1] - 1)))
	{
		if (curs_pos[0] <= caps->size_prompt && curs_pos[1] == caps->y_prompt + 1)
			home_key(caps);
		else if (curs_pos[0] > caps->x_lines[0])
		{
			tputs(tgetstr("vi", NULL), 1, ft_outc);
			int i = -1;
			while (++i < curs_pos[0])// + 1
				left_key(caps);
			// tputs(tgetstr("up", NULL), 1, ft_outc);
			// caps->cursor = caps->cursor - caps->window_size[1];
			tputs(tgetstr("ve", NULL), 1, ft_outc);
		}
		else
		{
			tputs(tgetstr("vi", NULL), 1, ft_outc);
			tputs(tgetstr("up", NULL), 1, ft_outc);
			caps->cursor = caps->cursor - (curs_pos[0] + (caps->x_lines[0] - curs_pos[0]) + 1);//caps->cursor - 
			tputs(tgetstr("ve", NULL), 1, ft_outc);
		}
	}
	return (0);
}


int			alt_down_key(t_tcap *caps)
{
	int		curs_pos[2];
	
	size_windows(caps);
	cursor_position(curs_pos);
	position_char_in_window_left_alt_keys(caps->cursor, caps, curs_pos);
	if (caps->x_lines[2] != -1) // && curs_pos[1] < ((caps->y_prompt) + (caps->sz_str / (caps->window_size[1] - 1)))
	{
		dprintf(2, "miaouh");
		position_char_in_window_print_inside_string(caps->cursor, caps, caps->sz_str);
		// dprintf(2, "curs_pos[1]: %d, calc_complet_y: [%d], calc_alt_down: [%d]\n", curs_pos[1], ((caps->y_prompt) + (caps->sz_str / (caps->window_size[1] - 1))), (caps->sz_str % (caps->window_size[1])));
		if (curs_pos[1] == caps->char_pos[1] - 1 && curs_pos[0] > caps->x_lines[2]) //curs_pos[0] >= (caps->sz_str % (caps->window_size[1])) && 
		{
			dprintf(2, "ici mon char");
			end_key(caps);
		}
		else if (curs_pos[0] > caps->x_lines[2])
		{
			tputs(tgetstr("vi", NULL), 1, ft_outc);
			int i = -1;
			while (++i < (caps->x_lines[1] - curs_pos[0]) + caps->x_lines[2] + 2)// + 1
				right_key(caps);
			// tputs(tgetstr("up", NULL), 1, ft_outc);
			// caps->cursor = caps->cursor - caps->window_size[1];
			tputs(tgetstr("ve", NULL), 1, ft_outc);
		}
		else
		{
			// dprintf(2, "ici ma poule");
			tputs(tgetstr("vi", NULL), 1, ft_outc);
			// tputs(tgoto(tgetstr("cm", NULL), curs_pos[0] - 1, curs_pos[1]), 1, ft_outc);
			int i = -1;
			while (++i < (caps->x_lines[1] - curs_pos[0]) + curs_pos[0] + 1)
				right_key(caps);
			// caps->cursor = caps->cursor + caps->window_size[1];
			tputs(tgetstr("ve", NULL), 1, ft_outc);
		}
		// dprintf(2, "ma biche");
	}
	return (0);
}

/*
*** - Aim of the function :
*** - Returns the position in the window, in the form of X (0) and Y (1)
*** - For any char in the string
*** - The function receives the global struct + the position of the char you
*** - are looking to get info for
*/
void		position_char_in_window_left_alt_keys(int pos, t_tcap *caps, int curs_pos[2])
{
	int 	x;
	int 	y;
	int		i;

	i = -1;
	x = caps->size_prompt; // - 1 
	y = caps->y_prompt;
	// pos = pos - caps->size_prompt;
	ft_memset(caps->x_lines, -1, 3);
	// caps->x_lines[0] = -1;
	// caps->x_lines[1] = -1;
	// caps->x_lines[2] = -1;
	while (++i < caps->sz_str - caps->size_prompt)
	{
		// dprintf(2, "leftkey: %c\n", caps->str[0][i]);
		if (caps->str[0][i] == '\n' || (x + 1 == caps->window_size[1]))
		{
			dprintf(2, "y: %d - caps->curs_pos[1]: %d\n", y, curs_pos[1]);
			if (y == curs_pos[1] - 1)
			{
				dprintf(2, "âsse");
				caps->x_lines[0] = x;
			}
			else if (y == curs_pos[1])
				caps->x_lines[1] = x;
			else if (y == curs_pos[1] + 1)
				caps->x_lines[2] = x;
			x = 0;
			y++;
		}
		else
			x++;
	}
	dprintf(2, "passe ici ");
	dprintf(2, "1 %d - 2: %d - 3 %d\n", caps->x_lines[0], caps->x_lines[1], caps->x_lines[2]);
}

/*
*** - Aim of the function :
*** - Returns the position in the window of the cursor after the last char of the string
*** - Function also returns the last_char of the string (on the same y that the cursor)
*/
void		position_char_in_window_print_inside_string(int pos, t_tcap *caps, int end)
{
	int x;
	int y;

	x = caps->curs_pos[0] - 1; // - 1 
	y = caps->curs_pos[1];
	caps->last_char = 0;
	pos = pos - caps->size_prompt - 1;
			// dprintf(2, "yep: char : %c - pos : %d\n", caps->str[0][pos], pos);
	dprintf(2, "\n\n");
	while (++pos < (end - caps->size_prompt))
	{
		// dprintf(2, "yep: char : %c - pos : %d\n", caps->str[0][pos], pos);
		if (caps->str[0][pos] == '\n' || (x == caps->window_size[1]))
		{
			x = 0;
			y++;
		}
		else
			x++;
		dprintf(2, "debug: x-|%d| - y-|%d| - |%d| - |%d| - |%c|\n", x, caps->window_size[1], y, caps->curs_pos[1], caps->str[0][pos]);
		if (x == caps->window_size[1] && y == caps->curs_pos[1])
		{
			dprintf(2, "entre debug: |%d| - |%d| - |%d| - |%d| - |%c|\n", x, caps->window_size[1], y, caps->curs_pos[1], caps->str[0][pos]);
			// dprintf(2, "debug: |%d| - |%d| - |%c|", y, caps->curs_pos[1], caps->str[0][pos]);
				caps->last_char = caps->str[0][pos + 1];
		}
	}
	caps->char_pos[0] = x;
	caps->char_pos[1] = y;
}


/*
*** - Aim of the function :
*** - Initialize the array of pointers function
*/
t_tab		*tab_termcaps(void)
{
	static t_tab ttab[8] = {
		{&left_key, 27, 91, 68, 0, 0, "le"},
		{&right_key, 27, 91, 67, 0, 0, "nd"},
		{&del_key, 127, 0, 0, 0, 0, "del"},
		{&home_key, 27, 91, 72, 0, 0, "home"}, // verify if keyboard at school has the same numbers for home
		{&end_key, 27, 91, 70, 0, 0, "end"},// verify if keyboard at school has the same numbers for end
		{&alt_up_key, 27, 27, 91, 65, 0, "alt_up"},
		{&alt_down_key, 27, 27, 91, 66, 0, "alt_down"},
		// {&alt_right_key, 27, 27, 91, 67, "alt_right"},
		// {&alt_left_key, 27, 27, 91, 68, "alt_left_key"},
		{NULL, 0, 0, 0, 0, 0, NULL}
	};

	return ((t_tab*)ttab);
}

int 		get_line_term(char **res, char *str)
{
  	// t_term		term;
	t_tcap		caps;
	t_tab		*ttab;
	t_tab		*tmp_tab;
	int			ret;
	// int 		i;
// //Initialisation du termios
// 	terminal_data(&term);
//   	modify_terminos(&term);
	
	//compteur pour \n seul -> eviter segfault
	i = 0;
// Initialisation du tableau de pointeurs sur fonction
	ttab = tab_termcaps();
// Initialisation de la struct caps
	initialize_caps(&caps, str);
//inclure un printf de prompt pour voir
	// ft_putstr_fd(str, 1);
// Itérer sur infini
	while (42)
	{
		tmp_tab = (ttab - 1);
		ft_bzero(caps.buf, 5);
		if ((ret = read(0, caps.buf, 4) < 0))
			return (1);
		if (caps.buf[0] == 10 &&
				caps.buf[1] == 0
				&& caps.buf[2] == 0 && caps.buf[3] == 0
				&& caps.buf[4] == 0)
		// {
			// dprintf(2, "i: %d\n", i);
			// if (i == 0)
			// 	*res = ft_strdup(" ");
			break ;
		// }
		while ((++tmp_tab)->cmd)
		{
			if (caps.buf[0] == tmp_tab->key0 &&
				caps.buf[1] == tmp_tab->key1
				&& caps.buf[2] == tmp_tab->key2 && caps.buf[3] == tmp_tab->key3
				&& caps.buf[4] == tmp_tab->key4)
			{
				tmp_tab->ptr(&caps);
				break;
			}
		}
		if (!tmp_tab->cmd)
			print_normal_char(&caps);
		// i++;
	}
	*res = caps.str[0];
	// reset_termios(&term);
return (0);
}