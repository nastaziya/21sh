#include "../../inc/sh.h"

int     terminal_data ()
{
  char *termtype = getenv ("TERM");
  int res;
  res = -1;

  if (termtype == 0)
    printf("error\n");

  res = tgetent (term_buffer, termtype);
  return (res);
}

int main()
{
    printf("%d\n", terminal_data());
}