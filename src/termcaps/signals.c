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

// void			norm_check_curs_window(int *x, int *y, int curs[2])
// {
// 	*x = curs[0];
// 	*y = curs[1];
// }

int				check_curs_window(t_tcap *caps, char *str, int i)
{
	// int	curs[2];
	int	x;
	// int	y;
	
	size_windows(caps);
	// home_key(caps);
	// cursor_position(curs);
	// norm_check_curs_window(&x, &y, curs);
	x = caps->size_prompt + 1;
	while (str && str[++i])
	{
		if (x == caps->window_size[1] || str[i] == '\n')
		// {
			// y++;
			x = 1;
		// }
		else
			x++;
	}
	dprintf(3, "-check_curs_window: x |%d| w_x |%d|\n", x, caps->window_size[1]); // , x_curs-pos: [%d]
	return (x);
}

/*
** Handles the resizing of the terminal window
** do not forget to free the copy of the prompt when exit termcaps
** TO MANAGE -> when the process is running, or quit the canonical mode,
*** - do nothing (return)
*/

void			win_resize(int sig)
{
	// int ret = 0;
	int	curs[2];
	
	// ret = check_if_scroll(&g_caps, g_caps.str[0], -1);
	// if ()
	// dprintf(3, "return_ret: %d\n", ret);
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
	{
		// dprintf(3, "debug_x: [%d], window_size: [%d]\n", g_caps.char_pos[0], g_caps.window_size[1]);//, x_curs-pos: [%d]
		if (g_caps.sz_str > g_caps.size_prompt)
			ft_putstr_fd(g_caps.str[0], 1);
		cursor_position(curs);
		if (check_curs_window(&g_caps, g_caps.str[0], -1) == 1 && g_caps.sz_str > g_caps.size_prompt + 1)
			tputs(tgoto(tgetstr("cm", NULL), 0, curs[1]), 1, ft_outc);

		// cursor_position(curs);
		// if (curs[0] == g_caps.window_size[1])
		
		
	}
	else
	{
		tputs(tgoto(tgetstr("cm", NULL), 0, 0), 0, ft_outc);
		tputs(tgetstr("cd", NULL), 1, ft_outc);
		ft_putstr_fd("make a larger screen", 1);
	}
	g_caps.y_prompt = 0;
	g_caps.cursor = g_caps.sz_str;
}

/*
*** - Aim of the function : int function to use putchar_fd inside if
*/

static int		ft_putc_if(char c)
{
	ft_putchar_fd(c, 1);
	return (0);
}

static void		ctrl_c_norm(void)
{
	if (ft_strcmp(g_caps.prompt, "bash > ") && (g_caps.sz_str = 10))
	{
		free(g_caps.str[0]);
		g_keeprun = 2;
		if (!ft_strcmp(g_caps.prompt, "dquote > "))
			g_caps.str[0] = ft_strdup("\"");
		else if (!ft_strcmp(g_caps.prompt, "squote > "))
			g_caps.str[0] = ft_strdup("\'");
		else if (!ft_strcmp(g_caps.prompt, "Missing arguments > "))
			g_caps.str[0] = ft_strdup("oui");
		else if (!ft_strcmp(g_caps.prompt, "\nHeredoc > "))
			g_caps.str[0] = ft_strdup("pppppp");
		else if (!ft_strcmp(g_caps.prompt, "Heredoc > "))
			g_caps.str[0] = ft_strdup("pppppp");
	}
}

/*
*** - Aim of the function : manage the ctrl_c signal
*** - Handles the resizing of the terminal window.
*** - do not forget to free the copy of the prompt when exit termcaps
*/

void			ctrl_c(int sig)
{
	char c;

	(void)sig;
	c = 10;
	if (g_keeprun != 3 && !ft_putc_if('\n'))
		return ;
	g_keeprun = 1;
	end_key(&g_caps);
	if (g_caps.str[0])
		free(g_caps.str[0]);
	g_caps.str[0] = ft_memalloc(1);
	g_caps.sz_str = ft_strlen(g_caps.prompt);
	while (g_caps.history[0]->prev)
		g_caps.history[0] = g_caps.history[0]->prev;
	ctrl_c_norm();
	ioctl(0, TIOCSTI, &c);
}

/*
*** Handles the resizing of the terminal window and the ctrl_c signal
*/

int				initialize_signals(void)
{
	signal(SIGWINCH, win_resize);
	signal(SIGINT, ctrl_c);
	return (1);
}
