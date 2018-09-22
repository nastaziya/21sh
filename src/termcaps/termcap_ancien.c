#include "../../inc/sh.h"
// #include "../../inc/termcap.h"

/*
*** - Aim of the function :
*** - Putstr from index i to j
*/
void	ft_putstr_i_to_j(char *str, int i, int j, int fd)
{
	--i;
	while (++i < j)
		ft_putchar_fd(str[i], fd);
}

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

void		initialize_caps(t_tcap *caps)
{
	caps->sz_str = 0;
	caps->cursor = 0;
	if (!(caps->str = (char**)malloc(sizeof(char*))))
		return ;
}

/*
*** - Aim of the function : Collect at a given moment the
***	- position of the cursor - I use it to manage the size of 
*** - the cursor at any given time + at the beginning to 
*** - manage the history (after)
*/

void		cursor_position(char curs_pos[2])
{
	char	ret[9];
	int		i;
	
	i = 0;
	ft_bzero(ret, 9);
	write(0, "\033[6n", 4);
	read(0, ret, 9);
	curs_pos[1] = ft_atoi(ret + 2) - 1;
	while (ret[i] && ret[i] != 59)
		i++;
	curs_pos[0] = ft_atoi(ret + i + 1) - 1;
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
	caps->window_size[0] = w->ws_row - 1;
	caps->window_size[1] = w->ws_col - 1;
	free(w);
}

int 		left_key(t_tcap *caps)
{
	// dprintf(1, "cursor: %d", caps->cursor);
	if (caps->cursor > 0)
	{
		cursor_position(caps->curs_pos);
		size_windows(caps);
		if (caps->curs_pos[0] == 0)
		{
			// tputs(tgetstr("sf", NULL), 1, ft_outc);
			tputs(tgoto(tgetstr("cm", NULL), caps->window_size[1], caps->curs_pos[1] - 1), 1, ft_outc);
		}
		else
		{
			if ((caps->res = tgetstr("le", NULL)) == NULL)
				return (-1);
			tputs(caps->res, 1, ft_outc);
		}
		caps->cursor--;
	}
	return (0);
}

int 		right_key(t_tcap *caps)
{
	if (caps->cursor < caps->sz_str)
	{
		cursor_position(caps->curs_pos);
		size_windows(caps);
		if (caps->curs_pos[0] == caps->window_size[1])
		{
			tputs(tgoto(tgetstr("cm", NULL), 0, caps->curs_pos[1] + 1), 1, ft_outc);
		}
		//TO DO : si cursor_position_X == size_window_col, alors monter d'une ligne
		// + aller ligne du dessous à gauche
		// sinon, aller à droite
		else
		{
			if ((caps->res = tgetstr("nd", NULL)) == NULL)
				return (-1);
			tputs(caps->res, 1, ft_outc);
		}
		caps->cursor++;
	}
	return (0);
}

int			right_key_print(t_tcap *caps)
{
	char test[2];

	if (caps->cursor < caps->sz_str)
	{
		cursor_position(test);
		size_windows(caps);
		if (caps->curs_pos[0] == caps->window_size[1] && caps->curs_pos[1] == caps->window_size[0] && caps->curs_pos[1] == test[1])
		{
			// dprintf(1, "infos: [%d - %d - %d]", test[1], caps->window_size[0], test[1]);
			tputs(tgetstr("sf", NULL), 1, ft_outc);
			tputs(tgoto(tgetstr("cm", NULL), caps->curs_pos[0] + 1, caps->curs_pos[1] + 1), 1, ft_outc);
			// tputs(tgetstr("up", NULL), 1, ft_outc);

		}
		//TO DO : si cursor_position_X == size_window_col, alors monter d'une ligne
		// + aller ligne du dessous à gauche
		// sinon, aller à droite
		else
		{
			if ((caps->res = tgetstr("nd", NULL)) == NULL)
				return (-1);
			tputs(caps->res, 1, ft_outc);
		}
		caps->cursor++;
	}
	return (0);
}

// a gerer, comportement quand print mais que pos curseur != fin
int			print_normal_char(t_tcap *caps)
{
	char		*tmp;
	char		*tmp2;
	char		*string;
	static int	i = 0;
	// char		cursxy[2];

	if (caps->buf[0] >= 0 && caps->buf[0] <= 127 && caps->buf[1] == 0)
	{
		string = ft_strndup(caps->buf, 1);
		// dprintf(1, "string: |%s|", string);
		if (i == 0) // Initialization of the str, the first time
		{
			caps->str[0] = string;
			++i;
			caps->sz_str++;
			caps->cursor++;
			ft_putstr_i_to_j(caps->buf, 0, 3, 1);
		}
		else if (caps->cursor < caps->sz_str) // MANAGE CAPS-> cursor and size because gets mess up
		{
			// dprintf(1, "[debugdeb: %d - %d]", caps->sz_str, caps->cursor);
			// saves cursor position
			// if ((tmp = tgetstr("sc", NULL)) == NULL)
			// 	return (-1);
			tputs(tgetstr("sc", NULL), 1, ft_outc);
			// cursor_position(cursxy);
			// manages substrings
			
			tmp2 = ft_strdup(caps->str[0] + caps->cursor);
			tmp = ft_strsub(caps->str[0], 0, caps->cursor);
			free(caps->str[0]);
			caps->str[0] = ft_strjoin(tmp, string);
			free(tmp);
			free(string);
			tmp = caps->str[0];
			caps->str[0] = ft_strjoin(tmp, tmp2);
			free(tmp);
			// free(tmp2);
			// clears the terminal
			if ((tmp = tgetstr("cd", NULL)) == NULL)
				return (-1);
			tputs(tmp, 1, ft_outc);

			// cursor_position(cursxy);
			// size_windows(caps);
			// int	curs = caps->cursor;
			// int res = ((caps->sz_str - curs) - (caps->window_size[1] - cursxy[0]));
			// sleep(2);
			// //prints the new characters
			// tputs(tgetstr("im", NULL), 1, ft_outc);
			ft_putstr_i_to_j(caps->buf, 0, 3, 1);
			// tputs(tgetstr("ei", NULL), 1, ft_outc);
			// sleep(2);
			// cursor_position(caps->curs_pos);
			// size_windows(caps);
			// int diff = caps->window_size[0] - caps->curs_pos[1] + 1;
			// int diff2 = diff;
			// while (--diff > 0)
			// {
			// 	dprintf(1, "pase par la");
				// tputs(tgetstr("sr", NULL), 1, ft_outc);
			// }
			// char *tmp3 = tmp2 - 1;
			// while (++tmp3 && *tmp3)
			// {
			// 	// right_key_print(caps);
			// 	// left_key(caps);
			// 	// sleep(1);
			// 	ft_putchar(*tmp3);
			// 	// caps->cursor--;
			// }
			write(1, tmp2, ft_strlen(tmp2));
			// ft_putstr_i_to_j(caps->buf, 0, 3, 1);
			//print the old part of the string - weird test
			caps->sz_str++;
			// caps->cursor++;
			// dprintf(1, "[findeb: %d - %d]", caps->sz_str, caps->cursor);
			// char *tmp3 = tmp2 - 1;
			// char test[2];
			// int	curs = caps->cursor;
			// size_windows(caps);
			tputs(tgetstr("rc", NULL), 1, ft_outc);
			// while (--diff2 > 0)
				// tputs(tgetstr("up", NULL), 1, ft_outc);
			right_key(caps);
			//((sz_str - curs)-> combien restants + (window_size - test)-> combien sur cette ligne)
			// DIVISE par window_size 
			// => DONNE LE BON NOMBRE DE LIGNE A DESCENDRE

			// int jo = (((caps->sz_str - curs) - (caps->window_size[1] - cursxy[0])) / caps->window_size[1]);
			// dprintf(1, "[1: %d- 2. %d- col: %d - nb lignes: %d]", caps->sz_str - curs, caps->window_size[1] - cursxy[0], caps->window_size[1], jo);

			// if (test[0] + (curs - caps->cursor + 1) == caps->window_size[1])//test[0] == caps->window_size[1]
			// {
			// 	tputs(tgetstr("sf", NULL), 1, ft_outc);
			// 	tputs(tgoto(tgetstr("cm", NULL), 0, test[1] + 1), 1, ft_outc);
			// }
			// while (++tmp3 && *tmp3)
			// {
			// 	// size_windows(caps);
			// 	cursor_position(test);
				
			// 	//faire bonne condition de mise a la ligne -> quand le bout de la ligne touche la fin de la fenêtre, ajouter une ligne yiha
			// 	// REGLER PROBLEME FLECHE GAUCHE -> peut pas remonter jusqu'au prompt quand passage de ligne
			// 	ft_putchar(*tmp3);
			// 	curs++;
			// }


			// ft_putstr_fd(tmp2, 1);
			free(tmp2);
			// goes back to the old position + goes one right
			// if ((tmp = tgetstr("rc", NULL)) == NULL)
			// 	return (-1);
			// tputs(tmp, 1, ft_outc);
			// tputs(tgoto(tgetstr("cm", NULL), cursxy[0], cursxy[1]), 1, ft_outc);
			// right_key(caps);
			//
			// dprintf(1, "[debugfin: %d - %d]", caps->sz_str, caps->cursor);
			
			// NEED TO MANAGE PROPERLY THE ++ of the size of string
			// && cursor for the <- arrow
			// caps->sz_str++;
			// caps->cursor++;
			// caps->cursor++;
		}
		// faire else if (les compteurs sont differents)
		else // all the next times
		{
			// cursor_position(caps->curs_pos);
			// size_windows(caps);
			// if (caps->curs_pos[0] == caps->window_size[1])
			// {
			// 	tputs(tgetstr("sf", NULL), 1, ft_outc);
			// 	tputs(tgoto(tgetstr("cm", NULL), 0, caps->curs_pos[1] + 1), 1, ft_outc);
			// }
			tmp = caps->str[0];
			caps->str[0] = ft_strjoin(tmp, string);
			free(tmp);
			free(string);
			caps->sz_str++;
			caps->cursor++;
			ft_putstr_i_to_j(caps->buf, 0, 3, 1);
		}
		// ft_putstr_i_to_j(caps->buf, 0, 3, 1);
		// printf(1, "debug: |%s|", caps->str[0]);
	}
	// dprintf(1, "came jere - %s\n", caps->str[0]);
	return (0);
}

/*
*** - Aim of the function :
*** - Initialize the array of pointers function
*/
t_tab		*tab_termcaps(void)
{
	static t_tab ttab[3] = {
		{&left_key, 27, 91, 68, "le"},
		{&right_key, 27, 91, 67, "nd"},
		{NULL, 0, 0, 0, NULL}
	};

	return ((t_tab*)ttab);
}

int			main(void)
{
  	t_term		term;
	t_tcap		caps;
	t_tab		*ttab;
	t_tab		*tmp_tab;

//Initialisation du termios
	terminal_data(&term);
  	modify_terminos(&term);

// Initialisation du tableau de pointeurs sur fonction
	ttab = tab_termcaps();
// Initialisation de la struct caps
	initialize_caps(&caps);
//inclure un printf de prompt pour voir
	dprintf(1, "bash > ");
// Itérer sur infini
	while (42)
	{
		tmp_tab = (ttab - 1);
		ft_bzero(caps.buf, 3);
		read(0, caps.buf, 3);
		while ((++tmp_tab)->cmd)
		{
			if (caps.buf[0] == tmp_tab->key0 &&
				caps.buf[1] == tmp_tab->key1 && caps.buf[2] == tmp_tab->key2)
			{
				tmp_tab->ptr(&caps);
				break;
			}
		}
		if (!tmp_tab->cmd)
			print_normal_char(&caps);
	}
	reset_termios(&term);



// tester prompt avec printf, puis clear screen pour voir ce qui apparaît
// free mallocs non free








// 	char *res; // dans fonction du poointeur sur fonction
//   char buf[3];
// 	// char test[1000];
// 	// char *travail;
// 	// travail = test;
 
//  // INITIALISATION DU TERMINAL ET DU TERMINOS
//   terminal_data(&term);
//   modify_terminos(&term);
//   while(1)
//   {
//   	  ft_bzero(buf, 3);
//   	  // ft_bzero(res, ft_strlen(res));
//       read(0, buf, 3);
//       // printf("%s", buf);
//       if (buf[0] != 127 && buf[0] != 64 && buf[0] != 27 && buf[0] != 38)//buf[0] != 27 && 
//       {
// 				ft_putstr_i_to_j(buf, 0, 3, 1);
// 				// *travail++ = buf[0];
// 				// dprintf(1, "debug : %s", test);
// 			}
//       // ft_putstr_i_to_j(buf, 0, 3, 1);
//       //left arrow
//       if (buf[0] == 27 && buf[1] == 91 && buf[2] == 68)
// 	  {
// 	  		if ((res = tgetstr("le", NULL)) == NULL)
// 				return (-1);
// 			tputs(res, 1, ft_outc);
// 			// ft_bzero(res, ft_strlen(res));
// 	  }//saves cursor position - key : del -> code : sc
// 	  else if (buf[0] == 127 && buf[1] == 0 && buf[2] == 0)
// 	  {//sc
// 	  		if ((res = tgetstr("cd", NULL)) == NULL)
// 				return (-1);
// 			tputs(res, 1, ft_outc);
// 			// ft_bzero(res, ft_strlen(res));
// 	  }//right arrow
// 	  else if (buf[0] == 27 && buf[1] == 91 && buf[2] == 67)
// 	  {
// 	  		if ((res = tgetstr("nd", NULL)) == NULL)
// 				return (-1);
// 			tputs(res, 1, ft_outc);
// 			// ft_bzero(res, ft_strlen(res));
// 	  }//return to the saved cursor position key : @
// 	  else if (buf[0] == 64 && buf[1] == 0 && buf[2] == 0)
// 	  {
// 	  		if ((res = tgetstr("rc", NULL)) == NULL)
// 				return (-1);
// 			// ft_putendl(res);
// 			tputs(res, 1, ft_outc);
// 	  }//launches insert mode, to add new things// key : &
// 	  else if (buf[0] == 38 && buf[1] == 0 && buf[2] == 0)
// 	  {
// 	  		if ((res = tgetstr("im", NULL)) == NULL)
// 				return (-1);
// 			tputs(res, 1, ft_outc);
// 	  }
// 	  else if (buf[0] == 34 && buf[1] == 0 && buf[2] == 0)
// 	  {
// 	  		if ((res = tgetstr("ei", NULL)) == NULL)
// 				return (-1);
// 			tputs(res, 1, ft_outc);
// 	  }


// ///////////INFO REFERENCE : http://web.mit.edu/~jik/sipbsrc/sun4m_53/micrognu/tty.c
//       //ft_putnstr(buf, 3);
//       // printf("%s\n", buf);
//       // printf(" %d %d %d \n", buf[0], buf[1], buf[2]);
//   }
//   //if (tcgetattr(0, &term) == -1)
//     //return (-1);
//   term.c_lflag = (ICANON | ECHO);
//   if (tcsetattr(0, TCSADRAIN, &term) == -1)
//     return (-1);
return (0);
}