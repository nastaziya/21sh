/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/28 13:50:16 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

int			ft_string_to_lexer_quote_management(const char **s,
				t_norm *nm)
{
	ft_find_closing_quote(s, nm);
	return (0);
}

int			norm_quote_end(const char **s, t_norm *nm)
{
	if (S_QUOTE && !manage_back_quote(*s, nm->prev) &&
			!ft_string_to_lexer_quote_management(s, nm) &&
			**s && (*(*s) + 1) && (*(*s) + 1) != ' ' && (*s)++)
		return (1);
	return (0);
}
