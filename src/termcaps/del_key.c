/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 22:37:42 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

/*
*** - Aim of the function : Managing of the del key
*** - Plus realloc properly the string
*/

int			del_key(t_tcap *caps)
{
	char *tmp2;
	char *tmp;

	if (caps->cursor > caps->size_prompt)
	{
		left_key(caps);
		tputs(tgetstr("sc", NULL), 1, ft_outc);
		tmp2 = ft_strdup(caps->str[0] + (caps->cursor - caps->size_prompt + 1));
		tmp = ft_strsub(caps->str[0], 0, (caps->cursor - caps->size_prompt));
		free(caps->str[0]);
		caps->str[0] = ft_strjoin(tmp, tmp2);
		free(tmp);
		tputs(tgetstr("cd", NULL), 1, ft_outc);
		write(1, tmp2, caps->sz_str - caps->cursor);
		tputs(tgetstr("rc", NULL), 1, ft_outc);
		free(tmp2);
		caps->sz_str--;
	}
	return (0);
}
