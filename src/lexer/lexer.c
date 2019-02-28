/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/24 16:54:59 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

/*
*** - I replaced the for loop into a while one, need to check if it is
*** - ++i inside the while or ++i after as I did, I think it's ok that way
*** - Checked, it's ok now;
*/

void		print(const t_lexer *lexer)
{
	int	i;

	i = 0;
	while (i < lexer->used_size)
	{
		printf("{ |%s| (%i) } ", lexer->tokens[i].content,
			lexer->tokens[i].type);
		++i;
	}
	printf("\n");
}

static void	ft_manage_string_to_lexer_realloc_arguments(t_lexer *lexer,
				t_dlist **history)
{
	char	*cmd;
	char	*tmp;

	ft_get_entire_line(&cmd, "Missing arguments > ", history);
	if (cmd && ft_strlen(cmd) > 0)
		if (!string_to_lexer(cmd, lexer))
			ft_putendl_fd("error !", 1);
	if (!g_caps.keeprun)
	{
		tmp = (*history)->content;
		(*history)->content = ft_strjoin(tmp, " ");
		free(tmp);
		tmp = (*history)->content;
		(*history)->content = ft_strjoin(tmp, cmd);
		free(tmp);
		free(cmd);
	}
}

/*
*** - Aim of the function :
*** - Function that manages if the lexing ends up with && || |
*** - if that's the case, we need to show a new prompt, collect a new string
*** -  and lex it
*** - Also manages the "cat << &&" error <= Should not enter in the
*** - Missing argument prompt
*** - with this line : 	&& (lexer->used_size > 1 ?
*** - lexer->tokens[lexer->used_size - 2].type != T_DBL_LESS : 1))
*** - Also manages the heredoc
*** - Function also adds history
*/

int			ft_manage_string_to_lexer(const char *s, t_lexer *lexer,
				t_dlist **history)
{
	if (!string_to_lexer(s, lexer))
		return (0);
	while (history[0]->prev)
		history[0] = history[0]->prev;
	if (!g_caps.keeprun)
		ft_dlstadd(history, ft_dlstnew(s));
	while (42)
	{
		if (lexer->used_size > 0 && lexer->tokens[lexer->used_size - 1].content
		&& ((lexer->tokens[lexer->used_size - 1].type == T_PIPE)
			|| (lexer->tokens[lexer->used_size - 1].type == T_DBLAND)
			|| (lexer->tokens[lexer->used_size - 1].type == T_DBLOR))
			&& (lexer->used_size > 1 ?
				lexer->tokens[lexer->used_size - 2].type == T_WORD : 1)
				&& (lexer->used_size == 1 ? 0 : 1)
				&& lexer->tokens[0].type == T_WORD)
			ft_manage_string_to_lexer_realloc_arguments(lexer, history);
		else
			break ;
	}
	return (1);
}

/*
*** - Aim of the function :
*** - Function that manages first the collect of the line : ft_get_entire_line
*** - then initialize the lexer : lexer_init
*** - and fills it : string_to_lexer
*/

t_lexer		final_tokens(t_dlist **history)
{
	char	*cmd;
	t_lexer	lexer;

	ft_get_entire_line(&cmd, "bash > ", history);
	lexer_init(&lexer);
	if (cmd && ft_strlen(cmd) > 0)
		if (!ft_manage_string_to_lexer(cmd, &lexer, history))
			ft_putendl_fd("error !", 1);
	free(cmd);
	return (lexer);
}
