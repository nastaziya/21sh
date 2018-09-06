#include "../../inc/sh.h"

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
	if (j >= 0 && lex.tokens[j].type == T_IO_NUMB)
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
		if (lex.tokens[i].type == T_SEMI)
		{
			assign_tok(cmd, lex, j, T_SEMI);
		}
		if (lex.tokens[i].type == T_PIPE)
			assign_tok(cmd, lex, j, T_PIPE);
		else if(lex.tokens[i].type == T_DBLAND)
			assign_tok(cmd, lex, j, T_DBLAND);
		else if(lex.tokens[i].type == T_DBLOR)
			assign_tok(cmd, lex, j, T_DBLOR);
		else if(lex.tokens[i].type == T_AND)
			assign_tok(cmd, lex, j, T_AND);
		else if (i + 1 == lex.used_size)
			assign_tok(cmd, lex, j, -1);
	}

}
//a mettre a la norme

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
		parse_errors(lex, i);
		if (i == 0 && lex.tokens[i].type == T_WORD)
			tab_assign(&cmd->command[j], lex, i);
		else if (lex.tokens[i].type == T_WORD && !is_red(lex, i - 1) &&
			lex.tokens[i - 1].type != T_IO_NUMB)
			tab_assign(&cmd->command[j], lex, i);
		else if (is_red(lex, i) && lex.tokens[i + 1].type == T_WORD)
		{
			tab_io_assign(&cmd->command[j].redirection, lex, i - 1);//envoyer struct vide ?
			tab_red_assign(&cmd->command[j].redirection, lex, i, i + 1);
		}
		else if(lex.tokens[i + 1].type != T_WORD && is_op(lex,i))
			*cmd->command[j + 1].cmd_simple = NULL;
		else if (!is_red(lex, i) && lex.tokens[i].type != T_IO_NUMB &&
			lex.tokens[i].type != T_WORD)
			j++;
		
	}
}