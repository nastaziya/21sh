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
	if (nm->type_quote == '\'' || nm->type_quote == '"')
	{
		while (**s && ++(*s) && *s < nm->end)
		{
			if ((**s == nm->type_quote && nm->type_quote != '\'' &&
				!manage_back_quote(*s, nm->prev))
					|| (nm->type_quote == '\'' && **s == '\''))
				break ;
		}
	}
	if (*s < nm->end)
		++(*s);
}

void		ft_tokenize_quote_management(const char **s, t_lexer *lexer,
				t_norm *nm)
{
	if (nm->prev != *s && **s ? ++(*s) : *s)
		add_token_to_lexer(lexer, nm->prev, *s - nm->prev, T_WORD);
	if (!**s)
		nm->prev = *s;
	nm->quote_done = 1;
}

int			ft_isalnum_back(char c)
{
	if (c == '\\' || ft_isalnum(c))
		return (1);
	return (0);
}

void		ft_string_to_lexer_quote_management(const char **s,
				t_lexer *lexer, t_norm *nm)
{
	// echo ouioooo"         "ffffff\>\>\>\>zxxxxx\"\""cddddd"
	dprintf(3, "debug: |%s|\n", *s);
	while (*s && **s)
	{
		dprintf(3, "char 1: |%c|\n", **s);
		if (**s == ' ' && --(*s))
			break ;
		ft_find_closing_quote(s, nm);
		if (!**s)
			break ;
		dprintf(3, "char 2: |%c|\n", **s);
		while (**s && **s != '\'' && **s != '"' && **s != ' ' && ft_isalnum(**s)) //ft_isalnum_back(**s))
			++(*s);
		dprintf(3, "char 3: |%c|\n", **s);
		dprintf(3, "char de merde: |%c|\n", (*(*s - 1)));
		if (manage_back_quote(*s, nm->prev))
		{
			dprintf(3, "youpi\n");
			++(*s);
		}
		else if ((**s == '<' || **s == '>' || **s == '&' || **s == '|') && ((*(*s - 1)) != '\\')
			&& --(*s))
			break ;
		else if (**s && **s != '\'' && **s != '"')
		{
			// t_oplist tmp  =  type_of_token(*s);
			dprintf(3, "sorti par la mapoule\n");
			while (**s == ' ')
				--(*s);
			while (**s == '\\')
				++(*s);
			break ;
		}
	}
	dprintf(3, "sortie: |%s|\n", *s);
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
