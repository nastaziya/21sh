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
#include "../../inc/expansion.h"
#include "../../inc/builtin.h"

/*
*** - Aim of the function :
*** - Function that manages the heredoc when we are waiting for the last keyword of the command
*** - => cat << oui << non -> we are in this function when we are expecting the "non" keyword
*** - => The if is for when the user typed the closing word
*** - the else if for when the user typed another keyword -> realloc
*/

static int		ft_manage_last_keyword(t_hdoc *h, t_lexer *lexer, char ***heredoc)
{
	char *tmp;
	char *res;
	// quand c'est le mot clé fermant, on free et c'est tout
	
	tmp = ft_strdup(lexer->tokens[h->words[h->k]].content);
	expanded_dynamic_table_heredoc(&tmp, 0);
	res = ft_strdup(tmp);
	if (ft_strcmp(h->cmd, res) == 0 && !ft_free(h->cmd))
	{
		(h->k)++;
		// free(h->cmd);
		free(tmp);
		free(res);
	}
	// là on realloc
	else if (!ft_free(tmp) && !ft_free(res))
	{
		// dprintf(2, "passe iciavant if\n");
		// free(tmp);
		// free(res);
		// // ici on initialise
		if (heredoc[0][h->command] == NULL)//[0]
		{
			// dprintf(2, "PLUS PRECCISEMENT [%s]\n", h->cmd);
			// if (heredoc[0][h->command])
			// 	free(heredoc[0][h->command]);
			heredoc[0][h->command] = ft_strjoin(h->cmd, "\n");
			free(h->cmd);
		}
		// // ici on realloc
		else //// GERER les leaks, normalement pas bon
		// {
		// 	dprintf(2, "ici ausi\n");
			if (realloc_heredoc(h, heredoc))
				return (1);
		// }
	}
	return (0);
}

/*
*** - Aim of the function :
*** - Function that collects the line, manages when we are not at the last keyword of the heredoc
*** - (cat << oui << non --> when we are expecting the Oui keyword) <= managed in the if
*** - else if -> manage when last keyword
*/

static int		ft_collect_line_and_realloc_heredoc(t_hdoc *h, t_lexer *lexer,
			char ***heredoc, t_dlist **history)
{
	char *tmp;
	// char *res;

	if (h->i_words > 0)
		while (h->k < h->i_words && !keepRunning)
		{
			h->obool > 0 ? ft_putstr_fd("\nHeredoc > ", 1)
				: ft_putstr_fd("Heredoc > ", 1); 
			// if (h->cmd)
			// 	free(h->cmd);
			h->obool > 0 ? get_line_term(&h->cmd, "\nHeredoc > ", history)
				: get_line_term(&h->cmd, "Heredoc > ", history);
			// if (h->cmd)
			// free(h->cmd);
			//si mot clé c'est pas le dernier, free ce que l'on get_line et passer au suivant
			// dprintf(3, "DEBUG HEREDOC_NB: [%d]-[%d]-[%d]\n", h->i_words, h->words[h->k], h->k);
			if (h->k < h->i_words - 1)// && (res = ft_strdup(h->cmd))
			{
				///DEBUG HEREDOC///
				// dprintf(3, "DEBUG HEREDOC: [|%d|]-[|%s|]\n", h->k, tmp);
				// dprintf(3, "DEBUG HEREDOC: [|%d|]\n", h->k);
				//control expansion
				tmp = ft_strdup(lexer->tokens[h->words[h->k]].content);
				expanded_dynamic_table_heredoc(&tmp, 0);
				// res = ft_strdup(tmp);
				// expansionner le lexer->tokens[h->words[h->k]].content sur les " et les /
				if (ft_strcmp(tmp, h->cmd) == 0)
					(h->k)++;
				free(h->cmd);
				free(tmp);
				// free(res);
			}
			// quand on arrive au dernier, on collecte les données dans le char***
			else if (h->k == h->i_words - 1)
				ft_manage_last_keyword(h, lexer, heredoc);
			h->obool++;
		}
	return (0);
}

/*
*** - Aim of the function :
*** - Function that manages the heredoc if necesary, will malloc of the proper size
*** - will first malloc the *** with the proper number of char** (that will be the number of << word with separators)
*** - => cat << non << oui ; ls ; cat << plus gives 2 char** 
*** - => cat << non ; ls ; cat << yo ; cat << plus gives 3 char***
*** -  and builds the proper char***
*** - En premier, compter le nombre de char** à malloc, et ressortir les cases à remplir
*** - puis après, 
*/

int		ft_manage_heredoc(t_lexer *lexer, char ***heredoc, t_dlist **history)
{
	t_hdoc	h;
	h.j = 0;
	h.command = 0;
	h.obool = 0;

	ft_initialize_heredoc(lexer, heredoc, 0, 0);
	
	// loop on the different commands (with separators : && || ;)
	while (h.j < lexer->used_size - 1 && !keepRunning)
	{
		ft_find_end_command_and_nb_kewyords(&h, lexer);
		ft_collect_line_and_realloc_heredoc(&h, lexer, heredoc, history);
		// Go to the next command, and if a << was found in the current command
		// go to the next * of the char*** heredoc
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