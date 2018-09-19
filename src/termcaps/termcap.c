#include "../../inc/sh.h"
// #include "../../inc/termcap.h"

void	ft_putwchar(wchar_t c)
{
	if (c <= 0x7F)
		ft_putchar(c);
	else if (c <= 0x7FF)
	{
		ft_putchar((c >> 6) | 0xC0);
		ft_putchar((c & 0x3F) | 0x80);
	}
	else if (c <= 0xFFFF)
	{
		ft_putchar((c >> 12) | 0xE0);
		ft_putchar(((c >> 6) & 0x3F) | 0x80);
		ft_putchar((c & 0x3F) | 0x80);
	}
	else if (c <= 0x10FFFF)
	{
		ft_putchar((c >> 18) | 0xF0);
		ft_putchar(((c >> 12) & 0x3F) | 0xE0);
		ft_putchar(((c >> 6) & 0x3F) | 0x80);
		ft_putchar((c & 0x3F) | 0x80);
	}
}

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

void		cursor_position(t_tcap *caps)
{
	// wchar_t c;
	char	ret[10];
	(void)caps;
	int y;
	// int x;
	char *str = "\033[6n";
	int		i = 0;

	// c = '\033[6n';
		ft_bzero(ret, 10);
	printf("%s", str);
	// ft_putwchar(c);
	read(0, ret, 10);
	while (i < 10)
	{
		dprintf(1, "%d-", ret[i]);//+ 48
		i++;
	}
	y = atoi(ret + 2);
	dprintf(1, "y: %d-", y);//+ 48
	// dprintf(1, "|%s|\n", ret);
	// y = ft_atoi(ret);
	// while (ret[i] && ret[i] != 59)
	// 	i++;
	// x = ft_atoi(ret + i + 1);
	// dprintf(1, "|x - y : %d - %d|", x, y);
	// char	pos[20];
	// char	*str;
	// int		i;

	// i = 0;
	// str = "\033[6n";
	// ft_bzero(pos, 20);
	// ft_printf("%s", str);
	// read(0, pos, 20);
	// info->curs_y = ft_atoi(pos + 2);
	// while (pos[i] && pos[i] != 59)
	// 	i++;
	// info->curs_x = ft_atoi(pos + i + 1);
}

int 		left_key(t_tcap *caps)
{
	if (caps->cursor > 0)
	{
		if ((caps->res = tgetstr("le", NULL)) == NULL)
			return (-1);
		tputs(caps->res, 1, ft_outc);
		caps->cursor--;
	}
	return (0);
}

int 		right_key(t_tcap *caps)
{
	if (caps->cursor < caps->sz_str)
	{
		cursor_position(caps);
		if ((caps->res = tgetstr("nd", NULL)) == NULL)
			return (-1);
		tputs(caps->res, 1, ft_outc);
		caps->cursor++;
	}
	return (0);
}
// a gerer, comportement quand print mais que pos curseur != fin
int			print_normal_char(t_tcap *caps)
{
	char		*tmp;
	char		*string;
	static int	i = 0;

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
		}
		// faire else if (les compteurs sont differents)
		else // all the next times
		{
			tmp = caps->str[0];
			caps->str[0] = ft_strjoin(tmp, string);
			free(tmp);
			free(string);
			caps->sz_str++;
			caps->cursor++;
		}
		ft_putstr_i_to_j(caps->buf, 0, 3, 1);
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