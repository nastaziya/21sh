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

static void		norm_position_char_window_left_alt_keys(t_tcap *caps)
{
	caps->x_lines[0] = -1;
	caps->x_lines[1] = -1;
	caps->x_lines[2] = -1;
}

/*
*** - Aim of the function :
*** - Returns the position in the window, in the form of X (0) and Y (1)
*** - For any char in the string
*** - The function receives the global struct + the position of the char you
*** - are looking to get info for (position of the cursor)
*/

void			position_char_in_window_left_alt_keys(int pos, t_tcap *caps, int curs_pos[2])
{
	int 	x;
	int 	y;
	int		i;

	i = -1;
	x = caps->size_prompt;
	// -> problem with y when indice == 0 solved by incrementing of 1
	y = caps->y_prompt == 0 ? caps->y_prompt + 1 : caps->y_prompt;
	norm_position_char_window_left_alt_keys(caps);
	while (++i < caps->sz_str - caps->size_prompt)
	{
		if (caps->str[0][i] == '\n' || (x + 1 == caps->window_size[1]))
		{
			if (y == curs_pos[1] - 1)
				caps->x_lines[0] = x;
			else if (y == curs_pos[1])
				caps->x_lines[1] = x;
			else if (y == curs_pos[1] + 1)
				caps->x_lines[2] = x;
			x = 0;
			y++;
		}
		else
			x++;
	}
}

/*
*** - Aim of the function :
*** - Returns the position in the window of the cursor after the last char of the string
*** - Function also returns the last_char of the line (on the same y that the cursor)
*/

int				position_char_in_window_print_inside_string(int pos, t_tcap *caps, int end, int bulean)
{
	int 	x;
	int 	y;
	int 	curs[2];

	cursor_position(curs);
	size_windows(caps);
	x = curs[0] - 1;
	y = curs[1];
	caps->last_char = 0;
	pos = pos - caps->size_prompt - 1;
	while (++pos < (end - caps->size_prompt))
	{
		if (caps->str[0][pos] == '\n'|| (bulean == 1 ? (x == caps->window_size[1]) : (x + 1 == caps->window_size[1])))
		{
			x = 0;
			y++;
		}
		else
			x++;
		if (x == caps->window_size[1] && y == caps->curs_pos[1])
				caps->last_char = caps->str[0][pos + 1];
	}
	caps->char_pos[0] = x;
	caps->char_pos[1] = y;
	return (1);
}