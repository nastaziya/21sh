#include "../inc/sh.h"

void    tab_init(t_simp_com *simple_cmd)
{
	simple_cmd->used_space = 0;
	simple_cmd->tok = 0;
	simple_cmd->av_space = TAB_INITIAL_CAPACITY;
	simple_cmd->cmd_simple = malloc(sizeof(char*) * simple_cmd->av_space + 1);
}

void    tab_red_init(t_red *redir)
{
	redir->used_space = 0;
	redir->av_space = TAB_INITIAL_CAPACITY;
	redir->red = malloc(sizeof(char*) * redir->av_space);
	redir->file = malloc(sizeof(char*) * redir->av_space);
	redir->fd = (int *)malloc(sizeof(int) * redir->av_space);
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
		simple_cmd->cmd_simple = malloc(sizeof(char*) * simple_cmd->av_space + 1);
		if (simple_cmd->cmd_simple == NULL)
			exit(EXIT_FAILURE);
		while(++i < simple_cmd->used_space)
			simple_cmd->cmd_simple[i] = ft_strdup(temp[i]);
		free(temp);
	}
	simple_cmd->cmd_simple[simple_cmd->used_space] = lex.tokens[j].content;
	//simple_cmd->cmd_simple[simple_cmd->av_space] = NULL;
	++simple_cmd->used_space;
}

void    tab_red_assign(t_red *redir, t_lexer lex, int j, int k)
{
	char **temp;
	char **temp1;
	int i;

	i = -1;
	if (redir->used_space == redir->av_space)
	{
		temp = redir->red;
		temp1 = redir->file;
		redir->av_space = (redir->used_space * 3) / 2 + 1;
		redir->red = malloc(sizeof(char*) * redir->av_space + 1);
		redir->file = malloc(sizeof(char*) * redir->av_space + 1);
		if (redir->red == NULL || redir->file == NULL)
			exit(EXIT_FAILURE);
		while(++i < redir->used_space)
		{
			redir->red[i] = ft_strdup(temp[i]);
			redir->file[i] = ft_strdup(temp1[i]);
		}
		free(temp);
		free(temp1);
	}
	redir->red[redir->used_space] = lex.tokens[j].content;
	redir->file[redir->used_space] = lex.tokens[k].content;
	++redir->used_space;
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
		cmd->command = malloc(sizeof(t_simp_com) * cmd->av_space + 1);
		if (cmd->command == NULL)
			exit(EXIT_FAILURE);
		while(++i < cmd->used_space)
			cmd->command[i] = temp[i];
		free(temp);
	}
	cmd->command[cmd->used_space] = simple_cmd;
	++cmd->used_space;
}
