/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 22:54:56 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

int				check_curs_window(t_tcap *caps, char *str, int i)
{
	int	x;
	
	size_windows(caps);
	x = caps->size_prompt + 1;
	while (str && str[++i])
	{
		if (x == caps->window_size[1] || str[i] == '\n')
			x = 1;
		else
			x++;
	}
	return (x);
}

/*
*** - Norm function
*/

void			win_resize_norm(void)
{
    int	curs[2];

    if (g_caps.sz_str > g_caps.size_prompt)
        ft_putstr_fd(g_caps.str[0], 1);
    cursor_position(curs);
    if (check_curs_window(&g_caps, g_caps.str[0], -1) == 1
        && g_caps.sz_str > g_caps.size_prompt + 1)
        tputs(tgoto(tgetstr("cm", NULL), 0, curs[1]), 1, ft_outc);
}

/*
** Handles the resizing of the terminal window
** do not forget to free the copy of the prompt when exit termcaps
** TO MANAGE -> when the process is running, or quit the canonical mode,
*** - do nothing (return)
*/

void			win_resize(int sig)
{
	(void)sig;
	end_key(&g_caps);
	tputs(tgoto(tgetstr("cm", NULL), 0, 0), 0, ft_outc);
	tputs(tgetstr("cd", NULL), 1, ft_outc);
	size_windows(&g_caps);
	display_bash(g_caps.prompt);
	g_caps.cursor = g_caps.size_prompt + 1;
	pos_char_in_window_in_str(g_caps.cursor,
		&g_caps, g_caps.sz_str, 0);
	if ((g_caps.char_pos[0] < g_caps.window_size[1] &&
		g_caps.char_pos[1] < g_caps.window_size[0])
		&& g_caps.window_size[1] > g_caps.size_prompt)
        win_resize_norm();
	else
	{
		tputs(tgoto(tgetstr("cm", NULL), 0, 0), 0, ft_outc);
		tputs(tgetstr("cd", NULL), 1, ft_outc);
		ft_putstr_fd("make a larger screen", 1);
	}
	g_caps.y_prompt = 1;
	g_caps.cursor = g_caps.sz_str;
}