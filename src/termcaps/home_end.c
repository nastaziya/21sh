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

int			home_key(t_tcap *caps)
{
	tputs(tgetstr("vi", NULL), 1, ft_outc);
	while (caps->cursor > caps->size_prompt)
		left_key(caps);
	tputs(tgetstr("ve", NULL), 1, ft_outc);
	return (0);
}

int			end_key(t_tcap *caps)
{
	tputs(tgetstr("vi", NULL), 1, ft_outc);
	while (caps->cursor < caps->sz_str)
		right_key(caps);
	tputs(tgetstr("ve", NULL), 1, ft_outc);
	return (0);
}
