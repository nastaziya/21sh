/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 22:11:12 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

int				is_op(t_lexer lex, int i)
{
	if (lex.tokens[i].type >= T_DBLAND && lex.tokens[i].type <= T_AND)
		return (1);
	return (0);
}

static void		ft_initialize_heredoc_norm(char ***heredoc, int nb_to_malloc)
{
	int	i;

	i = -1;
	if (!(heredoc[0] = (char**)malloc(sizeof(char*) * (nb_to_malloc + 1))))
		return ;
	while (++i <= nb_to_malloc)
		heredoc[0][i] = NULL;
}

/*
*** - Aim of the function :
*** - Function that counts the number of commands that have an heredoc
*** - then mallocs it
*** - If a other_command != previous command, it means that we met a ; || &&
*** - so we have to malloc the heredoc for the new command. The first if is for
*** - the initialization the first time we meet a <<
*/

int				ft_initialize_heredoc(t_lexer *lexer, char ***heredoc,
					int other_command, int previous_command)
{
	int		nb_to_malloc;
	int		i;

	nb_to_malloc = 0;
	i = -1;
	if (lexer->used_size > 0)
	{
		while (++i < lexer->used_size)
		{
			if (lexer->tokens[i].type == T_DBL_LESS
			&& other_command == 0 && (other_command++) && (nb_to_malloc++))
				previous_command++;
			else if ((lexer->tokens[i].type == T_DBLOR
				|| lexer->tokens[i].type == T_SEMI
					|| lexer->tokens[i].type == T_DBLAND))
				other_command++;
			else if (lexer->tokens[i].type == T_DBL_LESS
			&& (other_command != previous_command)
				&& (previous_command = other_command))
				nb_to_malloc++;
		}
		ft_initialize_heredoc_norm(heredoc, nb_to_malloc);
	}
	return (0);
}

/*
*** - Aim of the function :
*** - Function that finds the end of the command and counts the number of
*** - heredocs (the number of <<)
*** - And stores the index of each keyword to match to quit each heredoc
*/

int				ft_find_end_command_and_nb_kewyords(t_hdoc *h, t_lexer *lexer)
{
	ft_memset(h->words, -1, 50);
	h->i = h->j - 1;
	while (lexer->tokens[h->j].type != T_DBLOR
		&& lexer->tokens[h->j].type != T_DBLAND
			&& lexer->tokens[h->j].type != T_SEMI
				&& h->j < ft_parse_error_for_heredoc(*lexer))
		++(h->j);
	h->i_words = 0;
	while (++(h->i) < h->j)
		if (lexer->tokens[h->i].type == T_DBL_LESS
			&& h->words[h->i_words] == -1)
		{
			h->words[h->i_words] = h->i + 1;
			(h->i_words)++;
		}
	h->k = 0;
	return (0);
}

/*
*** - Aim of the function :
*** - Function that reallocs the heredoc for the proper command
*/

int				realloc_heredoc(t_hdoc *h, char ***heredoc)
{
	char	*tmp;

	tmp = heredoc[0][h->command];
	heredoc[0][h->command] = ft_strjoin(tmp, h->cmd);
	free(tmp);
	free(h->cmd);
	tmp = heredoc[0][h->command];
	if (g_keeprun != 4)
		heredoc[0][h->command] = ft_strjoin(tmp, "\n");
	else
		heredoc[0][h->command] = ft_strdup(tmp);
	free(tmp);
	return (1);
}
