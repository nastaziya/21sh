/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 22:42:57 by gurival-    ###    #+. /#+    ###.fr     */
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

void			position_char_in_window_left_alt_keys(t_tcap *caps,
					int curs_pos[2])
{
	int		x;
	int		y;
	int		i;

	i = -1;
	x = caps->size_prompt;
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

static void		norm_pos_char_in_window_in_str(t_tcap *caps, int x, int y)
{
	caps->char_pos[0] = x;
	caps->char_pos[1] = y;
}

/*
*** - Aim of the function :
*** - Returns the position in the window of the cursor after the last char
*** - of the string
*** - Function also returns the last_char of the line (on the paragraph
*** - (until the next '\n' that the cursor))
*/

int				pos_char_in_window_in_str(int pos,
					t_tcap *caps, int end, int bulean)
{
	int		x;
	int		y;
	int		curs[2];
	int		count_n;

	cursor_position(curs);
	size_windows(caps);
	x = curs[0] - 1;
	y = curs[1];
	count_n = 0;
	caps->last_char = 0;
	pos = pos - caps->size_prompt - 1;
	while (++pos < (end - caps->size_prompt))
	{
		if ((caps->str[0][pos] == '\n' || (bulean == 1 ?
		(x == caps->window_size[1]) : (x + 1 == caps->window_size[1]))) && y++)
			x = 0;
		else
			x++;
		if (x + 1 == caps->window_size[1]
			&& caps->str[0][pos + 1] == '\n' && count_n == 0)
			caps->last_char = caps->str[0][pos + 1];
	}
	norm_pos_char_in_window_in_str(caps, x, y);
	return (1);
}
