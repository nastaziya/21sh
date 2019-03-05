/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 22:57:36 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"

/*
*** - Aim of the function :
*** - Initialize the array of pointers function
*/

t_tab		*tab_termcaps(void)
{
	static t_tab ttab[19] = {
		{&left_key, 27, 91, 68, 0, 0, "le"},
		{&right_key, 27, 91, 67, 0, 0, "nd"},
		{&del_key, 127, 0, 0, 0, 0, "del"},
		{&home_key, 27, 91, 72, 0, 0, "home"},
		{&end_key, 27, 91, 70, 0, 0, "end"},
		{&alt_up_key, 27, 27, 91, 65, 0, "alt_up"},
		{&alt_down_key, 27, 27, 91, 66, 0, "alt_down"},
		{&up_key, 27, 91, 65, 0, 0, "up"},
		{&down_key, 27, 91, 66, 0, 0, "down"},
		{&alt_right_key, 27, 27, 91, 67, 0, "alt_right"},
		{&alt_left_key, 27, 27, 91, 68, 0, "alt_left_key"},
		{&ctrl_l, 12, 0, 0, 0, 0, "ctrl_l"},
		{&alt_x, -30, -119, -120, 0, 0, "alt_x"},
		{&alt_s, -61, -97, 0, 0, 0, "alt_s"},
		{&alt_w, -30, -120, -111, 0, 0, "alt_w"},
		{&alt_p, -49, -128, 0, 0, 0, "alt_p"},
		{&ctrl_dnorm, 4, 0, 0, 0, 0, "ctrl_dnorm"},
		{&ctrl_c, 3, 0, 0, 0, 0, "ctrl_c"},
		{NULL, 0, 0, 0, 0, 0, NULL}
	};

	return ((t_tab*)ttab);
}

int			ft_clean(void *s, size_t n)
{
	ft_bzero(s, n);
	return (0);
}

void		ft_initialize_get_line(t_tab **ttab, char *str, t_dlist **history,
				t_tcap *caps)
{
	*ttab = tab_termcaps();
	initialize_caps(caps, str);
	caps->history = history;
}

int			change_size(t_tcap *caps)
{
	t_tcap buf;

	size_windows(&buf);
	if (caps->window_size[0] == buf.window_size[0] &&
			caps->window_size[1] == buf.window_size[1])
		return (0);
	return (1);
}

int			get_line_term_termcaps(char **res, char *str, t_dlist **history,
				t_tcap *caps)
{
	t_tab		*ttab;
	t_tab		*tmp_tab;

	ft_initialize_get_line(&ttab, str, history, caps);
	while ((tmp_tab = (ttab - 1)) && !ft_clean(caps->buf, 2048)
		&& (read(0, caps->buf, 2047) >= 0))
	{
		if (change_size(caps))
			win_resize(caps);
		if (!new_line(caps) && !end_key(caps)
			&& ((caps->sz_str - caps->size_prompt) == 0)
				&& (*res = NULL) && (caps->str[0] ?
					!ft_free(caps->str[0]) : 1) && (g_keeprun == 3 ?
						0 : g_keeprun) && !ft_free_char_char(caps->str))
			return (2);
		else if (!new_line(caps) && !end_key(caps))
			break ;
		if (!ctrlld(caps) && ctrl_d_management(caps))
			break ;
		while ((++tmp_tab)->cmd)
			if (!equality(caps, tmp_tab) && !(tmp_tab->ptr(caps)))
				break ;
		if (!tmp_tab->cmd)
			print_buf(caps, caps->buf);
	}
	return (0);
}

int			get_term(char **res, char *str, t_dlist **history, t_tcap *caps)
{
	t_term		term;
	int			ret;

	terminal_data(&term);
	modify_terminos(&term);
	g_keeprun = 3;
	if ((ret = get_line_term_termcaps(res, str, history, caps)))
		return (ret);
	*res = caps->str[0];
	if (caps->tmp_str)
		free(caps->tmp_str);
	caps->tmp_str = NULL;
	if (caps->prompt)
		free(caps->prompt);
	caps->prompt = NULL;
	if (caps->copy_str)
		free(caps->copy_str);
	caps->copy_str = NULL;
	free(caps->str);
	caps->str = NULL;
	if (g_keeprun == 3)
		g_keeprun = 0;
	reset_termios(&term);
	initialize_signals();
	return (0);
}
