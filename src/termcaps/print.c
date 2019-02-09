/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 22:50:24 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

void			print_end_line(t_tcap *caps, char *string, char *tmp)
{
	tmp = caps->str[0];
	caps->str[0] = ft_strjoin(tmp, string);
	free(tmp);
	free(string);
	caps->sz_str++;
	caps->cursor++;
	cursor_position(caps->curs_pos);
	write(1, caps->buf, 3);
	if (caps->curs_pos[0] == caps->window_size[1])
	{
		position_char_in_window_print_inside_string(caps->cursor, caps,
			caps->sz_str, 0);
		if (caps->char_pos[0] + 1 == (caps->window_size[1])
				&& caps->char_pos[1] == caps->window_size[0])
		{
			tputs(tgetstr("sf", NULL), 1, ft_outc);
			caps->y_prompt--;
		}
		tputs(tgoto(tgetstr("cm", NULL), 0, caps->curs_pos[1]), 1, ft_outc);
	}
	else if (caps->curs_pos[1] == caps->window_size[0] && caps->buf[0] == '\n')
		caps->y_prompt--;
}

int				check_if_scroll(t_tcap *caps, char *str)
{
	int	curs[2];
	int	i = -1;
	int	x;
	int	y;
	int	count_up;
	int	keeptrack;

	cursor_position(curs);
	size_windows(caps);
	x = curs[0];
	y = curs[1];
	count_up = 0;
	while (str && str[++i])
	{
		if (x == caps->window_size[1] || str[i] == '\n')
		{
			y++;
			x = 1;
		}
		else
			x++;
		if (y > caps->window_size[0] && keeptrack != str[i]
			&& (keeptrack = str[i]))
			count_up++;
	}
	return (count_up > 0 ? 1 : 0);
}

/*
*** - ------- FONCTIONNE PAS MAIS BEAU -------
*/

static void		print_middle_line(t_tcap *caps, char *string, char *tmp,
					char *tmp2)
{
	int tst[2];
	int boul = 0;

	tputs(tgetstr("sc", NULL), 1, ft_outc);
	tmp2 = ft_strdup(caps->str[0] + (caps->cursor - caps->size_prompt));
	tmp = ft_strsub(caps->str[0], 0, (caps->cursor - caps->size_prompt));
	free(caps->str[0]);
	caps->str[0] = ft_strjoin(tmp, string);
	free(tmp);
	free(string);
	tmp = caps->str[0];
	caps->str[0] = ft_strjoin(tmp, tmp2);
	free(tmp);
	tputs(tgetstr("cd", NULL), 1, ft_outc);
	cursor_position(caps->curs_pos);
	if (check_if_scroll(caps, tmp2))
	{
		cursor_position(tst);
		if (tst[1] != caps->window_size[0])
			tputs(tgoto(tgetstr("cm", NULL), tst[0] - 1, caps->window_size[0]),
				1, ft_outc);
		tputs(tgetstr("sf", NULL), 1, ft_outc);
		caps->y_prompt--;
		tputs(tgoto(tgetstr("cm", NULL), tst[0] - 1, tst[1] - 2), 1, ft_outc);
		boul = 1;
	}
	write(1, caps->buf, 3);
	if (caps->buf[0] == '\n' && check_if_scroll(caps, tmp2))
		caps->y_prompt--;
	write(1, tmp2, caps->sz_str - caps->cursor);
	caps->sz_str++;
	if (boul == 0)
		tputs(tgetstr("rc", NULL), 1, ft_outc);
	else if (boul == 1)
		tputs(tgoto(tgetstr("cm", NULL), tst[0] - 1, tst[1] - 2), 1, ft_outc);
	right_key(caps);
	free(tmp2);
}

static void		print_normal_char_initialization_first_passage(t_tcap *caps,
					char *string)
{
	caps->str[0] = ft_strdup(string);
	free(string);
	++(caps->i);
	caps->sz_str++;
	caps->cursor++;
	write(1, caps->buf, 3);
}

/*
*** - NEED TO NORM THE FUNCTION, BUT WORKS PROPERLY
*/

int				print_normal_char(t_tcap *caps)
{
	char *tmp;
	char *tmp2;
	char *string;

	tmp = NULL;
	tmp2 = NULL;
	string = NULL;
	size_windows(caps);
	position_char_in_window_print_inside_string(caps->cursor,
		caps, caps->sz_str, 0);
	string = ft_strndup(caps->buf, 1);
	tmp2 = ft_strjoin(string, caps->str[0] +
		(caps->cursor - caps->size_prompt));
	if (((caps->buf[0] >= 32 && caps->buf[0] <= 127) || caps->buf[0] == 10)
			&& caps->buf[1] == 0 &&
			!(caps->y_prompt == 1 && check_if_scroll(caps, tmp2)))
	{
		free(tmp2);
		tmp2 = NULL;
		if (caps->i == 0)
			print_normal_char_initialization_first_passage(caps, string);
		else if (caps->cursor < caps->sz_str)
			print_middle_line(caps, string, tmp, tmp2);
		else
			print_end_line(caps, string, tmp);
	}
	else if (caps->buf[0] != 9)
		tputs(tgetstr("bl", NULL), 1, ft_outc);
	return (0);
}
