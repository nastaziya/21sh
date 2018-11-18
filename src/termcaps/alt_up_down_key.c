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

static void	alt_up_key_norm(t_tcap *caps, int curs_pos[2])
{
	tputs(tgetstr("vi", NULL), 1, ft_outc);
	tputs(tgetstr("up", NULL), 1, ft_outc);
	caps->cursor = caps->cursor - (curs_pos[0] + (caps->x_lines[0] - curs_pos[0]) + 1);
	tputs(tgetstr("ve", NULL), 1, ft_outc);
}

int			alt_up_key(t_tcap *caps)
{
	int		curs_pos[2];
	int		i;

	i = -1;
	size_windows(caps);
	cursor_position(curs_pos);
	position_char_in_window_left_alt_keys(caps->cursor, caps, curs_pos);
	if (curs_pos[1] > caps->y_prompt)
	{
		if (curs_pos[0] <= caps->size_prompt && curs_pos[1] == (caps->y_prompt == 0 ? caps->y_prompt + 1 : caps->y_prompt) + 1) // si x inférieur x prompt, le terner est pour gérer quand y == 0
			home_key(caps);
		else if (curs_pos[0] > caps->x_lines[0]) // si x supérieur x char d'au dessus
		{
			tputs(tgetstr("vi", NULL), 1, ft_outc);
			while (++i < curs_pos[0])
				left_key(caps);
			tputs(tgetstr("ve", NULL), 1, ft_outc);
		}
		else // le reste
			alt_up_key_norm(caps, curs_pos);
	}
	return (0);
}


int			alt_down_key(t_tcap *caps)
{
	int		curs_pos[2];
    int     i;
	
	i = -1;
	size_windows(caps);
	cursor_position(curs_pos);
	position_char_in_window_left_alt_keys(caps->cursor, caps, curs_pos);
	position_char_in_window_print_inside_string(caps->cursor, caps, caps->sz_str, 1);
	if (curs_pos[1] == caps->char_pos[1] - 1 && curs_pos[0] > caps->x_lines[2] && caps->x_lines[2] != -1) // si x curseur dépasse x ligne suivante et que derniere ligne
		end_key(caps);
	else if (curs_pos[0] > caps->x_lines[2] && caps->x_lines[2] != -1) // si x curseur dépasse x ligne suivante en général
	{
		tputs(tgetstr("vi", NULL), 1, ft_outc);
		while (++i < (caps->x_lines[1] - curs_pos[0]) + caps->x_lines[2] + 2)
			right_key(caps);
		tputs(tgetstr("ve", NULL), 1, ft_outc);
	}
	else if (curs_pos[1] < caps->char_pos[1]) // si le curseur n'est pas sur la derniere ligne
	{
		tputs(tgetstr("vi", NULL), 1, ft_outc);
		while (++i < (caps->x_lines[1] - curs_pos[0]) + curs_pos[0] + 1)
			right_key(caps);
		tputs(tgetstr("ve", NULL), 1, ft_outc);
	}
	return (0);
}