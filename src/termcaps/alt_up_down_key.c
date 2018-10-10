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

int			alt_up_key(t_tcap *caps)
{
	int		curs_pos[2];
	
	size_windows(caps);
	cursor_position(curs_pos);
	position_char_in_window_left_alt_keys(caps->cursor, caps, curs_pos);
	if (curs_pos[1] > caps->y_prompt) // && curs_pos[1] < ((caps->y_prompt) + (caps->sz_str / (caps->window_size[1] - 1)))
	{
		if (curs_pos[0] <= caps->size_prompt && curs_pos[1] == caps->y_prompt + 1)
			home_key(caps);
		else if (curs_pos[0] > caps->x_lines[0])
		{
			tputs(tgetstr("vi", NULL), 1, ft_outc);
			int i = -1;
			while (++i < curs_pos[0])// + 1
				left_key(caps);
			// tputs(tgetstr("up", NULL), 1, ft_outc);
			// caps->cursor = caps->cursor - caps->window_size[1];
			tputs(tgetstr("ve", NULL), 1, ft_outc);
		}
		else
		{
			tputs(tgetstr("vi", NULL), 1, ft_outc);
			tputs(tgetstr("up", NULL), 1, ft_outc);
			caps->cursor = caps->cursor - (curs_pos[0] + (caps->x_lines[0] - curs_pos[0]) + 1);//caps->cursor - 
			tputs(tgetstr("ve", NULL), 1, ft_outc);
		}
	}
	return (0);
}


int			alt_down_key(t_tcap *caps)
{
	int		curs_pos[2];
	
	size_windows(caps);
	cursor_position(curs_pos);
	position_char_in_window_left_alt_keys(caps->cursor, caps, curs_pos);
	if (caps->x_lines[2] != -1) // && curs_pos[1] < ((caps->y_prompt) + (caps->sz_str / (caps->window_size[1] - 1)))
	{
		dprintf(2, "miaouh");
		position_char_in_window_print_inside_string(caps->cursor, caps, caps->sz_str);
		// dprintf(2, "curs_pos[1]: %d, calc_complet_y: [%d], calc_alt_down: [%d]\n", curs_pos[1], ((caps->y_prompt) + (caps->sz_str / (caps->window_size[1] - 1))), (caps->sz_str % (caps->window_size[1])));
		if (curs_pos[1] == caps->char_pos[1] - 1 && curs_pos[0] > caps->x_lines[2]) //curs_pos[0] >= (caps->sz_str % (caps->window_size[1])) && 
		{
			dprintf(2, "ici mon char");
			end_key(caps);
		}
		else if (curs_pos[0] > caps->x_lines[2])
		{
			tputs(tgetstr("vi", NULL), 1, ft_outc);
			int i = -1;
			while (++i < (caps->x_lines[1] - curs_pos[0]) + caps->x_lines[2] + 2)// + 1
				right_key(caps);
			// tputs(tgetstr("up", NULL), 1, ft_outc);
			// caps->cursor = caps->cursor - caps->window_size[1];
			tputs(tgetstr("ve", NULL), 1, ft_outc);
		}
		else
		{
			// dprintf(2, "ici ma poule");
			tputs(tgetstr("vi", NULL), 1, ft_outc);
			// tputs(tgoto(tgetstr("cm", NULL), curs_pos[0] - 1, curs_pos[1]), 1, ft_outc);
			int i = -1;
			while (++i < (caps->x_lines[1] - curs_pos[0]) + curs_pos[0] + 1)
				right_key(caps);
			// caps->cursor = caps->cursor + caps->window_size[1];
			tputs(tgetstr("ve", NULL), 1, ft_outc);
		}
		// dprintf(2, "ma biche");
	}
	return (0);
}