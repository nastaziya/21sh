#include "../../inc/sh.h"
//compiler avec le avec -lncurses
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
  t_term term;
  char buf[3];
  terminal_data(&term);
  modify_terminos(&term);
  while(1)
  {
      read(0, buf, 3);
      printf("%s\n", buf);
      printf("%d %d %d \n", buf[0], buf[1], buf[2]);
  }
  //if (tcgetattr(0, &term) == -1)
    //return (-1);
  term.c_lflag = (ICANON | ECHO);
  if (tcsetattr(0, TCSADRAIN, &term) == -1)
    return (-1);

}