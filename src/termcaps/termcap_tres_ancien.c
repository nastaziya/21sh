#include "../../inc/sh.h"
// #include "../../inc/termcap.h"

void	ft_putstr_i_to_j(char *str, int i, int j, int fd)
{
	while (i < j)
	{
		ft_putchar_fd(str[i], fd);
		i++;
	}
}

// Obligatoire pour utiliser tputs, car putchar est un void* et il attend une fonction en int
int			ft_outc(int c)
{
  ft_putchar(c);
  return (0);
}

//compiler avec le avec -lncurses
// Collecter les infos du terminal
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

//Modifie le controle sur le terminal en question
int     modify_terminos(t_term *term)
{
  term->c_lflag &= ~(ICANON);
  term->c_lflag &= ~(ECHO);
  term->c_cc[VMIN] = 1;
  term->c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSADRAIN, term) == -1)
    return (-1);
  return (0);
}

int main()
{
  //char   *name_term;
  // t_term term;
	struct termios term;
	char *res;
  char buf[3];
	char test[1000];
	char *travail;
	travail = test;
  terminal_data(&term);
  modify_terminos(&term);
  while(1)
  {
  	  ft_bzero(buf, 3);
  	  // ft_bzero(res, ft_strlen(res));
      read(0, buf, 3);
      // printf("%s", buf);
      if (buf[0] != 127 && buf[0] != 64 && buf[0] != 27 && buf[0] != 38)//buf[0] != 27 && 
      {
				ft_putstr_i_to_j(buf, 0, 3, 1);
				*travail++ = buf[0];
				dprintf(1, "debug : %s", test);
			}
      // ft_putstr_i_to_j(buf, 0, 3, 1);
      //left arrow
      if (buf[0] == 27 && buf[1] == 91 && buf[2] == 68)
	  {
	  		if ((res = tgetstr("le", NULL)) == NULL)
				return (-1);
			tputs(res, 1, ft_outc);
			// ft_bzero(res, ft_strlen(res));
	  }//saves cursor position - key : del -> code : sc
	  else if (buf[0] == 127 && buf[1] == 0 && buf[2] == 0)
	  {//sc
	  		if ((res = tgetstr("cd", NULL)) == NULL)
				return (-1);
			tputs(res, 1, ft_outc);
			// ft_bzero(res, ft_strlen(res));
	  }//right arrow
	  else if (buf[0] == 27 && buf[1] == 91 && buf[2] == 67)
	  {
	  		if ((res = tgetstr("nd", NULL)) == NULL)
				return (-1);
			tputs(res, 1, ft_outc);
			// ft_bzero(res, ft_strlen(res));
	  }//return to the saved cursor position key : @
	  else if (buf[0] == 64 && buf[1] == 0 && buf[2] == 0)
	  {
	  		if ((res = tgetstr("rc", NULL)) == NULL)
				return (-1);
			// ft_putendl(res);
			tputs(res, 1, ft_outc);
	  }//launches insert mode, to add new things// key : &
	  else if (buf[0] == 38 && buf[1] == 0 && buf[2] == 0)
	  {
	  		if ((res = tgetstr("im", NULL)) == NULL)
				return (-1);
			tputs(res, 1, ft_outc);
	  }
	  else if (buf[0] == 34 && buf[1] == 0 && buf[2] == 0)
	  {
	  		if ((res = tgetstr("ei", NULL)) == NULL)
				return (-1);
			tputs(res, 1, ft_outc);
	  }


///////////INFO REFERENCE : http://web.mit.edu/~jik/sipbsrc/sun4m_53/micrognu/tty.c
      //ft_putnstr(buf, 3);
      // printf("%s\n", buf);
      // printf(" %d %d %d \n", buf[0], buf[1], buf[2]);
  }
  //if (tcgetattr(0, &term) == -1)
    //return (-1);
  term.c_lflag = (ICANON | ECHO);
  if (tcsetattr(0, TCSADRAIN, &term) == -1)
    return (-1);

}