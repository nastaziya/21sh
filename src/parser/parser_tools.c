/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   parser_tools.c                                   .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/09 22:03:18 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/24 16:56:48 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

int				is_built_in(char **cmd)
{
	if (cmd && cmd[0])
	{
		if (!ft_strcmp("echo", cmd[0]) ||
		!ft_strcmp("setenv", cmd[0]) ||
		!ft_strcmp("unsetenv", cmd[0]) ||
		!ft_strcmp("cd", cmd[0]) ||
		!ft_strcmp("env", cmd[0]) ||
		!ft_strcmp("exit", cmd[0]))
			return (1);
	}
	return (0);
}

void			free_content_after_error(t_lexer *lex, int i)
{
	if (lex->tokens[i].content != NULL)
	{
		free(lex->tokens[i].content);
		lex->tokens[i].content = NULL;
	}
}

static int		parse_errors_norm(t_lexer lex, int i)
{
	if ((is_op(lex, i) && i == 0) || lex.tokens[i].type == T_DBL_SEMI)
	{
		ft_putstr_fd("bash: syntax error near unexpected token ", 2);
		ft_putendl_fd(lex.tokens[i].content, 2);
		free_content_after_error(&lex, i);
		return (1);
	}
	return (0);
}

int				parse_errors(t_lexer lex, int i)
{
	while (++i < lex.used_size)
	{
		if (parse_errors_norm(lex, i))
			return (0);
		if ((is_red(lex, i) && (is_red(lex, i + 1) || is_op(lex, i + 1))) ||
		(is_op(lex, i) && is_op(lex, i + 1)))
		{
			ft_putstr_fd("bash: syntax error near unexpected token ", 2);
			ft_putendl_fd(lex.tokens[i + 1].content, 2);
			free_content_after_error(&lex, i);
			return (0);
		}
		if (is_red(lex, i) && lex.tokens[i + 1].type != T_WORD)
		{
			free_content_after_error(&lex, i);
			ft_putendl_fd("bash: syntax error near unexpected token `newline'",
				2);
			return (0);
		}
	}
	return (1);
}

/*
*** - Aim of the function :
*** - Function that returns the int up until the heredoc function shall work
*** - This is used to manage these error cases :
*** - ;; cat << EOF -> Shall not work
*** - cat << EOF ;; << PU -> Shall heredoc for the EOF keyword
*** - car << EOF << PU ;; -> Shall heredoc for the EOF and PU keywords
*/

int				ft_parse_error_for_heredoc(t_lexer lex)
{
	int	i;

	i = -1;
	while (++i < lex.used_size)
	{
		if ((is_op(lex, i) && i == 0) || lex.tokens[i].type == T_DBL_SEMI)
			return (i);
		if ((is_red(lex, i) && (is_red(lex, i + 1) || is_op(lex, i + 1))) ||
		(is_op(lex, i) && is_op(lex, i + 1)))
			return (i);
		if (is_red(lex, i) && lex.tokens[i + 1].type != T_WORD)
			return (i);
	}
	return (lex.used_size - 1);
}
