/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 21:55:20 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/expansion.h"
#include "../../inc/builtin.h"

static void		ft_norm_manage_last_keyword(t_hdoc *h, char *tmp,
					char *res)
{
	(h->k)++;
	free(tmp);
	free(res);
}

/*
*** - Aim of the function :
*** - Function that manages the heredoc when we are waiting
*** - for the last keyword of the command
*** - => cat << oui << non -> we are in this function when
*** - we are expecting the "non" keyword
*** - => The if is for when the user typed the closing word
*** - the else if for when the user typed another keyword -> realloc
*/

static int		ft_manage_last_keyword(t_hdoc *h, t_lexer *lexer,
					char ***heredoc)
{
	char	*tmp;
	char	*res;

	tmp = ft_strdup(lexer->tokens[h->words[h->k]].content);
	expanded_dynamic_table_heredoc(&tmp, 0);
	res = ft_strdup(tmp);
	if (h->cmd && ft_strcmp(h->cmd, res) == 0 && !ft_free(h->cmd))
		ft_norm_manage_last_keyword(h, tmp, res);
	else if (!ft_free(tmp) && !ft_free(res))
	{
		if (heredoc[0][h->command] == NULL)
		{
			if (g_keeprun != 4)
				heredoc[0][h->command] = ft_strjoin(h->cmd, "\n");
			else
				heredoc[0][h->command] = ft_strdup(h->cmd);
			if (h->cmd)
				free(h->cmd);
		}
		else if (realloc_heredoc(h, heredoc))
			return (1);
	}
	return (0);
}

/*
*** - Aim of the function :
*** - Function that collects the line, manages when we are not
*** - at the last keyword of the heredoc
*** - (cat << oui << non --> when we are expecting the Oui keyword) <=
*** - managed in the if
*** - else if -> manage when last keyword
*/

static int		ft_collect_line_and_realloc_heredoc(t_hdoc *h, t_lexer *lexer,
					char ***heredoc, t_dlist **history)
{
	char *tmp;

	if (h->i_words > 0)
		while (h->k < h->i_words && !g_keeprun)
		{
			h->obool > 0 ? display_bash("\nHeredoc > ")
				: display_bash("Heredoc > ");
			h->obool > 0 ? get_term(&h->cmd, "\nHeredoc > ", history, &(h->caps))
				: get_term(&h->cmd, "Heredoc > ", history, &(h->caps));
			if (h->k < h->i_words - 1)
			{
				tmp = ft_strdup(lexer->tokens[h->words[h->k]].content);
				expanded_dynamic_table_heredoc(&tmp, 0);
				if (h->cmd && ft_strcmp(tmp, h->cmd) == 0)
					(h->k)++;
				if (h->cmd)
					free(h->cmd);
				free(tmp);
			}
			else if (h->k == h->i_words - 1)
				ft_manage_last_keyword(h, lexer, heredoc);
			h->obool++;
		}
	return (0);
}

/*
*** - Aim of the function :
*** - Function that manages the heredoc if necesary, will malloc of proper size
*** - will first malloc the *** with the proper number of char**
*** - (that will be the number of << word with separators)
*** - => cat << non << oui ; ls ; cat << plus gives 2 char**
*** - => cat << non ; ls ; cat << yo ; cat << plus gives 3 char***
*** -  and builds the proper char***
*** - En premier, compter le nombre de char** à malloc, et ressortir les
*** - cases à remplir puis après
*/

int				ft_manage_heredoc(t_lexer *lexer, char ***heredoc,
					t_dlist **history)
{
	t_hdoc	h;

	h.j = 0;
	h.command = 0;
	h.obool = 0;
	ft_initialize_heredoc(lexer, heredoc, 0, 0);
	while (h.j < lexer->used_size - 1 && !g_keeprun)
	{
		ft_find_end_command_and_nb_kewyords(&h, lexer);
		ft_collect_line_and_realloc_heredoc(&h, lexer, heredoc, history);
		if (h.j < lexer->used_size - 1)
		{
			(h.j)++;
			if (h.words[0] != -1)
				(h.command)++;
		}
	}
	if (h.obool > 0)
		ft_putstr_fd("\n", 1);
	return (0);
}
