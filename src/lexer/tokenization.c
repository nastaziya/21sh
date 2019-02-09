/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/09/07 17:58:18 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

/*
*** - Aim of the function : tokeniwze the part of the string to the assigned
*** - token, regarding the proper size
*/

void		add_token_to_lexer(t_lexer *lexer, const char *text,
				int text_size, t_token_type type)
{
	t_lexer_token	item;
	t_lexer_token	*temp;
	int				i;

	i = -1;
	item.content = ft_strndup(text, text_size);
	item.content[text_size] = 0;
	item.size = text_size;
	item.type = type;
	if (lexer->used_size == lexer->capacity)
	{
		temp = lexer->tokens;
		lexer->capacity = (lexer->used_size * 3) / 2 + 1;
		if (!(lexer->tokens = malloc(sizeof(t_lexer_token) * lexer->capacity + 1)))
			return ;
		if (lexer->tokens == NULL)
			exit(EXIT_FAILURE);
		while (++i < lexer->used_size)
			lexer->tokens[i] = temp[i];
		free(temp);
	}
	lexer->tokens[lexer->used_size] = item;
	++lexer->used_size;
}

/*
*** - Aim of the function : checks if the pointer and the next ones equals one
*** - of the enum, if nothing is found, send an empty struct (not_found);
*/

t_oplist	type_of_token(const char *s)
{
	const t_oplist		*ex_tok;
	t_oplist			not_found;

	ex_tok = g_existing_token;
	not_found = (t_oplist){0, 0, 0};
	while (ex_tok && ex_tok->op)
	{
		if (ft_strncmp(s, ex_tok->op, ex_tok->size) == 0)
			return (*ex_tok);
		++ex_tok;
	}
	return (not_found);
}

void		ft_string_to_lexer_advance(const char **s, t_lexer *lexer,
				t_norm *nm)
{
	*s += nm->current.size;
	if (nm->current.type != T_EAT)
		add_token_to_lexer(lexer, nm->current.op,
				nm->current.size, nm->current.type);
	nm->prev = *s;
	nm->quote_done = 0;
}

void		ft_initialize_nm(const char *s, t_norm *nm)
{
	const char *tmp;

	nm->quote_done = 0;
	nm->prev = s;
	nm->start = s;
	tmp = s;
	while (*tmp)
		++(tmp);
	nm->end = tmp;
}

int			string_to_lexer(const char *s, t_lexer *lexer)
{
	t_norm nm;

	ft_initialize_nm(s, &nm);
	while (s && *s)
	{
		nm.current = type_of_token(s);
		if ((*s == '>' || *s == '<') && (s - nm.start > 0 ?
					ft_isdigit(*(s - 1)) : 0))
			add_token_to_lexer(lexer, nm.prev, s - nm.prev, T_IO_NUMB);
		else if ((*s == '"' || *s == '\'') &&
			!manage_back_quote(s, nm.start))
			ft_string_to_lexer_quote_management(&s, lexer, &nm);
		else if (*s == '\n' && *(s - 1) == '\\' && ++s)
			continue ;
		else if (nm.current.op != 0 && nm.prev != s)
			add_token_to_lexer(lexer, nm.prev, s - nm.prev, T_WORD);
		if (nm.current.op != 0 || nm.quote_done == 1)
			ft_string_to_lexer_advance(&s, lexer, &nm);
		else
			++s;
	}
	if (nm.prev != s)
		add_token_to_lexer(lexer, nm.prev, s - nm.prev, T_WORD);
	return (1);
}