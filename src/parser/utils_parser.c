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

void    		command_init(t_command *cmd)
{
	cmd->used_space = 0;
	cmd->av_space = TAB_INITIAL_CAPACITY;
	if (!(cmd->command = malloc(sizeof(t_simp_com) * cmd->av_space)))
		return ;
}

static void		norm_tab_red_assign(t_red *redir, t_lexer lex, int j, int k)
{
	redir->red[redir->used_space] = lex.tokens[j].type;
	redir->file[redir->used_space] = lex.tokens[k].content;
	++redir->used_space;
}

void    		tab_red_assign(t_red *redir, t_lexer lex, int j, int k)
{
	e_token_type	*temp;
	char			**temp1;
	int i;

	i = -1;
	if (redir->used_space == redir->av_space && (temp = redir->red))
	{
		temp1 = redir->file;
		redir->av_space = (redir->used_space * 3) / 2 + 1;
		if (!(redir->red = malloc(sizeof(e_token_type) * redir->av_space + 1)))
			return ;
		if (!(redir->file = malloc(sizeof(char*) * redir->av_space + 1)))
			return ;
		if ((redir->red == NULL || redir->file == NULL))
			exit(EXIT_FAILURE);
		while(++i < redir->used_space)
		{
			redir->red[i] = temp[i];
			redir->file[i] = ft_strdup(temp1[i]);
		}
		free(temp);
		free(temp1);
	}
	norm_tab_red_assign(redir, lex, j, k);
}