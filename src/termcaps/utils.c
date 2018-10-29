/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/19 18:02:22 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

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
*** - Initialize the cap struct
*/

void		initialize_caps(t_tcap *caps, char *prompt)
{
	int 	tmp[2];
	caps->size_prompt = ft_strlen(prompt);
	caps->prompt = ft_strdup(prompt);
	caps->sz_str = caps->size_prompt;
	caps->cursor =  caps->size_prompt;
	cursor_position(tmp);
	caps->y_prompt = tmp[1];
	caps->i = 0;
	caps->tmp_str = NULL;
	caps->copy_str = NULL;
	caps->ct_arrow = 0;
	if (!(caps->str = (char**)malloc(sizeof(char*))))
		return ;
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