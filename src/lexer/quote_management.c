/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 21:46:31 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

/*
*** - Aim of the function : checks if the number of backslash before a quote
*** - is pair or impair. If pair, returns 0 ; otherwise, returns 1
*/

int			manage_back_quote(const char *s, const char *begin)
{
	int			i;
	int			diff;
	int			count;
	const char	*dup;

	i = -1;
	count = 0;
	dup = s;
	diff = dup - begin;
	while (++i < diff && --dup)
	{
		if (*dup == '\\')
			count++;
		else if (*dup != '\\')
			break ;
	}
	if (count % 2 != 0)
		return (1);
	return (0);
}

/*
*** - part of the string_to_lexer_quote_function
*/

void		ft_find_closing_quote(const char **s, t_norm *nm)
{
	nm->type_quote = **s;
	while (**s && ++(*s))
		if ((**s == nm->type_quote && nm->type_quote != '\'' &&
			!manage_back_quote(*s, nm->start))
				|| (nm->type_quote == '\'' && **s == '\''))
			break ;
}

void		ft_tokenize_quote_management(const char **s, t_lexer *lexer,
				t_norm *nm)
{
	if (nm->prev != *s && ++(*s))
		add_token_to_lexer(lexer, nm->prev, *s - nm->prev, T_WORD);
	nm->quote_done = 1;
}

void		ft_string_to_lexer_quote_management(const char **s, t_lexer *lexer,
				t_norm *nm)
{
	while (42)
	{
		ft_find_closing_quote(s, nm);
		if (*s < nm->end && (*(*s + 1) == '"' || *(*s + 1) == '\'') && ++(*s))
			continue ;
		else if (**s && *(*s + 1) && !(*(*s + 1) >= 8 && *(*s + 1) <= 13)
			&& *(*s + 1) != 32)
		{
			++(*s);
			while (**s && ((**s == '\n' && *(*s - 1) == '\\') ? ++(*s) : *s) &&
				ft_isalnum(*(*s + 1)) && *(*s + 1) != '"' && *(*s + 1) != '\'')
				++(*s);
			if (((*(*s + 1) == '"') || (*(*s + 1) == '\'')) && ++(*s))
				continue ;
			else if ((*(*s + 1) == 32 || (*(*s + 1) >= 8 && *(*s + 1) <= 13)))
				++(*s);
		}
		else if (*s < nm->end && (*(*s + 1) == 32 || (*(*s + 1) >= 8 &&
			*(*s + 1) <= 13)))
			++(*s);
		if (((**s >= 8 && **s <= 13) || **s == 32 || !**s) &&
			(*s = ((**s >= 8 && **s <= 13) || **s == 32 || !**s) ? --(*s) : *s))
			break ;
	}
	ft_tokenize_quote_management(s, lexer, nm);
}

/*
*** - Aim of the function :
*** - The aim is to check if impair number of quotes :
*** -  " ' (impair or pair number of dquotes or squotes)
*** - if everything's allright, returns 0
*** - Otherwise, returns the corresponding char
*** - And checks at the end if the string ends by \ -> if yes, returns \
*/

char		ft_count_quote(char *str)
{
	char	type_quote;
	char	*start;

	type_quote = 0;
	start = str;
	while (str && *str)
	{
		if ((*str == '"' || *str == '\'')
				&& !manage_back_quote(str, start))
		{
			type_quote = *str;
			while (++str && *str)
				if ((*str == type_quote && !manage_back_quote(str, start)
					&& type_quote != '\'') ||
						(type_quote == '\'' && *str == '\''))
					break ;
			if (!*str)
				return (type_quote);
			else
				++str;
		}
		else
			++str;
	}
	return (!*str && manage_back_quote(str, start) ? '\\' : 0);
}
