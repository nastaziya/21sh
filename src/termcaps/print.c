/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/28 22:49:51 by gurival-    ###    #+. /#+    ###.fr     */
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
		pos_char_in_window_in_str(caps->cursor, caps,
			caps->sz_str, 0);
		if ((caps->char_pos[0] + 1 == (caps->window_size[1])
				&& caps->char_pos[1] == caps->window_size[0])
				&& caps->y_prompt--)
			tputs(tgetstr("sf", NULL), 1, ft_outc);
		tputs(tgoto(tgetstr("cm", NULL), 0, caps->curs_pos[1]), 1, ft_outc);
	}
	else if (caps->curs_pos[1] == caps->window_size[0] && caps->buf[0] == '\n')
		caps->y_prompt--;
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

static void		print_normal_free_no_pr(char *string, char *tmp2)
{
	free(string);
	free(tmp2);
	tputs(tgetstr("bl", NULL), 1, ft_outc);
}

static void		print_normal_char_norm(t_tcap *caps, char **string, char **tmp2)
{
	size_windows(caps);
	pos_char_in_window_in_str(caps->cursor,
		caps, caps->sz_str, 0);
	*string = ft_strndup(caps->buf, 1);
	*tmp2 = ft_strjoin(*string, caps->str[0] +
		(caps->cursor - caps->size_prompt));
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
	print_normal_char_norm(caps, &string, &tmp2);
	if (((caps->buf[0] >= 32 && caps->buf[0] <= 127) || caps->buf[0] == 10)
			&& caps->buf[1] == 0 &&
			!(caps->y_prompt == 1 && check_if_scroll(caps, tmp2, -1)))
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
		print_normal_free_no_pr(string, tmp2);
	return (0);
}
