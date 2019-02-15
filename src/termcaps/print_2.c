/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/15 18:29:11 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

int				check_if_scroll(t_tcap *caps, char *str, int i)
{
	int	curs[2];
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

static int		print_middle_line_norm(t_tcap *caps, char **string, char **tmp,
					char **tmp2)
{
	tputs(tgetstr("sc", NULL), 1, ft_outc);
	*tmp2 = ft_strdup(caps->str[0] + (caps->cursor - caps->size_prompt));
	*tmp = ft_strsub(caps->str[0], 0, (caps->cursor - caps->size_prompt));
	free(caps->str[0]);
	caps->str[0] = ft_strjoin(*tmp, *string);
	free(*tmp);
	free(*string);
	*tmp = caps->str[0];
	caps->str[0] = ft_strjoin(*tmp, *tmp2);
	free(*tmp);
	tputs(tgetstr("cd", NULL), 1, ft_outc);
	cursor_position(caps->curs_pos);
	return (0);
}

/*
*** - ------- FONCTIONNE PAS MAIS BEAU -------
*/

void			print_middle_line(t_tcap *caps, char *string, char *tmp,
					char *tmp2)
{
	int tst[2];
	int boul;

	boul = print_middle_line_norm(caps, &string, &tmp, &tmp2);
	if (check_if_scroll(caps, tmp2, -1) && (boul = 1))
	{
		cursor_position(tst);
		if (tst[1] != caps->window_size[0])
			tputs(tgoto(tgetstr("cm", NULL), tst[0] - 1, caps->window_size[0]),
				1, ft_outc);
		tputs(tgetstr("sf", NULL), 1, ft_outc);
		caps->y_prompt--;
		tputs(tgoto(tgetstr("cm", NULL), tst[0] - 1, tst[1] - 2), 1, ft_outc);
	}
	write(1, caps->buf, 3);
	if (caps->buf[0] == '\n' && check_if_scroll(caps, tmp2, -1))
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
