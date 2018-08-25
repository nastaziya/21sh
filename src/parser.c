#include "../inc/sh.h"


void    command_init(t_command *cmd)
{
	cmd->used_space = 0;
	cmd->av_space = TAB_INITIAL_CAPACITY;
	cmd->command = malloc(sizeof(t_simp_com) * cmd->av_space);
}

void assign_tok(t_command *cmd, t_lexer lex, int *j, int val_tok)
{
	simple_cmd_assign(cmd, *cmd->command);
	(*j)++;
	tab_init(&cmd->command[*j]);
	tab_red_init(&cmd->command[*j].redirection);
	cmd->command[*j].tok = val_tok;
}

void    tab_io_assign(t_red *redir, t_lexer lex, int j)
{
	int *temp;
	int i;

	i = -1;	
		temp = redir->fd;
		redir->fd = (int *)malloc(sizeof(int) * redir->av_space + 1);
		if (redir->fd == NULL)
			exit(EXIT_FAILURE);
		while(++i < redir->used_space)
			redir->fd[i] = temp[i];
		free(temp);
	if (lex.tokens[j].type == 22)
		redir->fd[redir->used_space] =  ft_atoi(lex.tokens[j].content);
	else
		redir->fd[redir->used_space] = 1;
}

void    add_token_val(t_command *cmd, t_lexer lex, int *j)
{
	int i;
	
	i = -1;
	*j = -1;
	while (++i < lex.used_size)
	{
		if (lex.tokens[i].type == 3)
			assign_tok(cmd, lex, j, 3);
		if (lex.tokens[i].type == 9)
			assign_tok(cmd, lex, j, 9);
		else if(lex.tokens[i].type == 7)
			assign_tok(cmd, lex, j, 7);
		else if(lex.tokens[i].type == 8)
			assign_tok(cmd, lex, j, 8);
		else if(lex.tokens[i].type == 11)
			assign_tok(cmd, lex, j, 11);
		else if (i + 1 == lex.used_size)
			assign_tok(cmd, lex, j, -1);
	}

}

//28 linii
void    add_simple_command(t_command *cmd, t_lexer lex)
{
	int size_simple_cmd;
	int i;
	int j;

	i = -1;
	j = 0;
	size_simple_cmd = 0;
	add_token_val(cmd, lex, &size_simple_cmd);
	while(++i < lex.used_size && j <= size_simple_cmd)
	{
		if (i == 0 && lex.tokens[i].type == 28)
			tab_assign(&cmd->command[j], lex, i);
		else if (lex.tokens[i].type == 28 && (lex.tokens[i - 1].type != 16 && lex.tokens[i - 1].type != 22 &&
				lex.tokens[i - 1].type != 18 && lex.tokens[i - 1].type != 19 &&
				lex.tokens[i - 1].type != 17))
			tab_assign(&cmd->command[j], lex, i);
		else if ((lex.tokens[i].type == 16 || lex.tokens[i].type == 18 ||
				lex.tokens[i].type == 19 || lex.tokens[i].type == 17) &&
				lex.tokens[i + 1].type == 28)
				{
					tab_io_assign(&cmd->command[j].redirection, lex, i - 1);
					tab_red_assign(&cmd->command[j].redirection, lex, i, i + 1);
				}
		else if ((lex.tokens[i].type != 16 && lex.tokens[i].type != 22 && lex.tokens[i].type != 18 &&
				lex.tokens[i].type != 19 && lex.tokens[i].type != 17) &&
				lex.tokens[i].type != 28)
			j++;
	}
}

