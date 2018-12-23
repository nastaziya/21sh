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

void    tab_init(t_simp_com *simple_cmd)
{
	simple_cmd->used_space = 0;
	simple_cmd->tok = 0;
	simple_cmd->av_space = TAB_INITIAL_CAPACITY;
	if (!(simple_cmd->cmd_simple =(char**)malloc(sizeof(char*)
		* (simple_cmd->av_space + 1))))
		return;
}

void    tab_red_init(t_red *redir)
{
	redir->used_space = 0;
	redir->av_space = TAB_INITIAL_CAPACITY;
	if (!(redir->red = malloc(sizeof(char*) * redir->av_space)))
		return ;
	if (!(redir->file = malloc(sizeof(char*) * redir->av_space)))
		return ;
	if (!(redir->fd = (int *)malloc(sizeof(int) * redir->av_space)))
		return ;
}

void    tab_assign(t_simp_com *simple_cmd, t_lexer lex, int j)
{
	char **temp;
	int i;

	i = -1;
	if (simple_cmd->used_space == simple_cmd->av_space)
	{
		temp = simple_cmd->cmd_simple;
		simple_cmd->av_space = (simple_cmd->used_space * 3) / 2 + 1;
		if (!(simple_cmd->cmd_simple = malloc(sizeof(char*)
			* (simple_cmd->av_space + 1))))
			return ;
		if (simple_cmd->cmd_simple == NULL)
			exit(EXIT_FAILURE);
		while(++i < simple_cmd->used_space)
			simple_cmd->cmd_simple[i] = ft_strdup(temp[i]);
		free(temp);
	}
	simple_cmd->cmd_simple[simple_cmd->used_space] = lex.tokens[j].content;
	++simple_cmd->used_space;
	simple_cmd->cmd_simple[simple_cmd->used_space] =  NULL;
}

void simple_cmd_assign(t_command *cmd, t_simp_com simple_cmd)
{
	t_simp_com *temp;
	int i;

	i = -1;
	if (cmd->used_space == cmd->av_space)
	{
		temp = cmd->command;
		cmd->av_space = (cmd->used_space * 3) / 2 + 1;
		if (!(cmd->command = malloc(sizeof(t_simp_com) * cmd->av_space + 1)))
			return ;
		if (cmd->command == NULL)
			exit(EXIT_FAILURE);
		while(++i < cmd->used_space)
			cmd->command[i] = temp[i];
		free(temp);
	}
	cmd->command[cmd->used_space] = simple_cmd;
	++cmd->used_space;
}
