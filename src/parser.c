#include "../inc/sh.h"

void    tab_init(t_simp_com *simple_cmd)
{
	simple_cmd->used_space = 0;
	simple_cmd->tok = 0;
	simple_cmd->av_space = TAB_INITIAL_CAPACITY;
	simple_cmd->cmd_simple = malloc(sizeof(char*) * simple_cmd->av_space);
}

void    tab_red_init(t_red *redir)
{
	redir->used_space = 0;
	redir->av_space = TAB_INITIAL_CAPACITY;
	redir->red = malloc(sizeof(char) * redir->av_space);
	redir->file = malloc(sizeof(char) * redir->av_space);
	redir->fd = malloc(sizeof(int) * redir->av_space);	
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

void print_array(int size, char **cmd)
{
	int i;

	i = 0;
	while(i < size)
	{
		printf("%s\n",cmd[i]);
		i++;
	}
}

void assign_tok(t_command *cmd, t_lexer lex, int *j, int val_tok)
{
	simple_cmd_assign(cmd, *cmd->command);
	(*j)++;
	tab_init(&cmd->command[*j]);
	cmd->command[*j].tok = val_tok;
}
void create_redirections_array(t_command *cmd, t_lexer lex, int i)
{
	tab_red_init(cmd->command->redirection);
	tab_red_assign(cmd->command->redirection, lex, i, i + 1);
	print_array(cmd->command->redirection->used_space, cmd->command->redirection->file);

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
		else if(lex.tokens[i].type == 7)
			assign_tok(cmd, lex, j, 7);
		else if(lex.tokens[i].type == 8)
			assign_tok(cmd, lex, j, 8);
		else if(lex.tokens[i].type == 11)
			assign_tok(cmd, lex, j, 11);
		else if(lex.tokens[i].type == 16)
		{
			create_redirections_array(cmd, lex, i);
			printf("gooooo\n");
		}
		else if (i + 1 == lex.used_size)
			assign_tok(cmd, lex, j, -1);
	}

}
void    add_simple_command(t_command *cmd, t_lexer lex)
{
	int size_simple_cmd;
	int i;
	int j;

	i = -1;
	j = 0;
	size_simple_cmd = 0;
	add_token_val(cmd, lex, &size_simple_cmd);
	while(++i < lex.used_size && j < size_simple_cmd)
	{
		if (lex.tokens[i].type == 28)
		{
			tab_assign(&cmd->command[j], lex, i);
		}
		else
			j++;
	}
}

void print_struct(t_command cmd)
{
	int i = 0;
	while (i < cmd.used_space)
	{
		print_array(cmd.command[i].used_space,cmd.command[i].cmd_simple);
		printf("tok : %d\n", cmd.command[i].tok);
		i++;
	}
}

void    command_init(t_command *cmd)
{
	cmd->used_space = 0;
	cmd->av_space = TAB_INITIAL_CAPACITY;
	cmd->command = malloc(sizeof(t_simp_com) * cmd->av_space);
}

int main()
{
	t_lexer lex;
	t_command cmd;
	int i =  -1;
	lex = final_tokens();
	print(&lex);
	command_init(&cmd);
	add_simple_command(&cmd, lex);
	print_struct(cmd);
}
