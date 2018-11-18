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

int 		left_key(t_tcap *caps)
{
	int  curs_pos[2];
	if (caps->cursor > caps->size_prompt)
	{
		cursor_position(curs_pos);
		size_windows(caps);
		if (curs_pos[0] == 1)
		{
			position_char_in_window_left_alt_keys(caps, curs_pos);
			tputs(tgoto(tgetstr("cm", NULL), caps->x_lines[0], curs_pos[1] - 2), 1, ft_outc);
		}
		else
			tputs(tgetstr("le", NULL), 1, ft_outc);
		caps->cursor--;
	}
	return (0);
}

int 		right_key(t_tcap *caps)
{
	int  curs_pos[2];
	
	if (caps->cursor < caps->sz_str)
	{
		cursor_position(curs_pos);
		size_windows(caps);
		if (curs_pos[0] == caps->window_size[1] || caps->str[0][caps->cursor - caps->size_prompt] == '\n')
			tputs(tgoto(tgetstr("cm", NULL), 0, curs_pos[1]), 1, ft_outc);
		else
			tputs(tgetstr("nd", NULL), 1, ft_outc);
		caps->cursor++;
	}
	return (0);
}