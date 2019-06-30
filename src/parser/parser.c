/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/24 16:57:23 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/expansion.h"
#include "../../inc/builtin.h"

void	assign_tok(t_command *cmd, int *j, int val_tok)
{
	simple_cmd_assign(cmd, *cmd->command);
	(*j)++;
	tab_init(&cmd->command[*j]);
	tab_red_init(&cmd->command[*j].redirection);
	cmd->command[*j].tok = val_tok;
}

void	tab_io_assign(t_red *redir, t_lexer lex, int j)
{
	int	*temp;
	int	i;

	i = -1;
	temp = redir->fd;
	if (!(redir->fd = (int *)malloc(sizeof(int) * redir->av_space + 1)))
		return ;
	if (redir->fd == NULL)
		exit(EXIT_FAILURE);
	while (++i < redir->used_space)
		redir->fd[i] = temp[i];
	free(temp);
	if (j >= 0 && lex.tokens[j].type == T_IO_NUMB)
		redir->fd[redir->used_space] = ft_atoi(lex.tokens[j].content);
	else
		redir->fd[redir->used_space] = 1;
}

/*
*** - add element to the dynamic table of simple commands according to
*** - existing operators
*** - res of this function:
*** - a dynamic array with operands assigned but without commands yet
*** - ex: ls | echo hei && diff blabla
*** - [0]{tok = | ; cmd_simple = NULL}
*** - [1]{tok = && ; cmd_simple = NULL}
*** - [2]{tok = -1 ; cmd_simple = NULL}
*/

void	add_token_val(t_command *cmd, t_lexer lex, int *j)
{
	int i;

	i = -1;
	*j = -1;
	while (++i < lex.used_size)
	{
		if (lex.tokens[i].type == T_SEMI)
			assign_tok(cmd, j, T_SEMI);
		if (lex.tokens[i].type == T_PIPE)
			assign_tok(cmd, j, T_PIPE);
		else if (lex.tokens[i].type == T_DBLAND)
			assign_tok(cmd, j, T_DBLAND);
		else if (lex.tokens[i].type == T_DBLOR)
			assign_tok(cmd, j, T_DBLOR);
		else if (lex.tokens[i].type == T_AND)
			assign_tok(cmd, j, T_AND);
		else if (i + 1 == lex.used_size)
			assign_tok(cmd, j, -1);
	}
}

/*
*** - (1) and (2) below take dynamic array created in "add_token_val" function
*** - and complete array of commands giving values to cmd_simple
*** - and redirections if existes
*** - (1)
*/

void	complete_simple_command_and_red(t_command *cmd, t_lexer lex, int i,
			int *j)
{
	if (i == 0 && (lex.tokens[i].type == T_WORD))
		tab_assign(&cmd->command[*j], lex, i);
	else if (lex.tokens[i].type == T_WORD && !is_red(lex, i - 1) &&
		lex.tokens[i - 1].type != T_IO_NUMB)
		tab_assign(&cmd->command[*j], lex, i);
	else if (is_op(lex, i))
		(*j)++;
	else if (is_red(lex, i) && lex.tokens[i + 1].type == T_WORD)
	{
		tab_io_assign(&cmd->command[*j].redirection, lex, i - 1);
		tab_red_assign(&cmd->command[*j].redirection, lex, i, i + 1);
	}
	else if (lex.tokens[i + 1].type != T_WORD && is_op(lex, i))
		*cmd->command[*j + 1].cmd_simple = NULL;
}

/*
*** - (2)
*/

void	add_simple_command(t_command *cmd, t_lexer lex, t_dlist **history,
			char ***heredoc)
{
	int	size_simple_cmd;
	int	i;
	int	j;

	i = -1;
	j = 0;
	size_simple_cmd = 0;
	ft_manage_heredoc(&lex, heredoc, history);
	if (parse_errors(lex, -1))
	{
		add_token_val(cmd, lex, &size_simple_cmd);
		while (++i < lex.used_size && j <= size_simple_cmd)
		{
			complete_simple_command_and_red(cmd, lex, i, &j);
		}
	}
	else
		free_the_op_content_array_token_after_error(&lex);
}
