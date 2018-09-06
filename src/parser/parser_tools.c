#include "../../inc/sh.h"

int		is_red(t_lexer lex, int i)
{
	if ((lex.tokens[i].type == T_GREAT || lex.tokens[i].type == T_LESS ||
		lex.tokens[i].type == T_DBL_GREAT || lex.tokens[i].type == T_DBL_LESS))
		return (1); 
	return (0);
}

int is_built_in(t_command cmd, int i)
{
	if (!ft_strcmp("echo", cmd.command[i].cmd_simple[0]) ||
	!ft_strcmp("setenv", cmd.command[i].cmd_simple[0]) ||
	!ft_strcmp("unsetenv", cmd.command[i].cmd_simple[0]) ||
	!ft_strcmp("cd", cmd.command[i].cmd_simple[0]) ||
	!ft_strcmp("env", cmd.command[i].cmd_simple[0]))
		return (1);
	return (0);
}

int		is_op(t_lexer lex, int i)
{
	if (lex.tokens[i].type >= T_DBLAND && lex.tokens[i].type <= T_AND)
		return (1);
	return(0);
}

void	parse_errors(t_lexer lex, int i)
{
	if ((is_op(lex, i) && i == 0) || lex.tokens[i].type == T_DBL_SEMI)
	{
		ft_putstr_fd("bash: syntax error near unexpected token ", 2);
		ft_putendl_fd(lex.tokens[i].content, 2);
		exit(1);
	}
	if ((is_red(lex, i) && (is_red(lex, i + 1) || is_op(lex, i + 1))) ||
	(is_op(lex, i) && is_op(lex, i + 1)))
	{
		ft_putstr_fd("bash: syntax error near unexpected token ", 2);
		ft_putendl_fd(lex.tokens[i + 1].content, 2);
		exit(1);
	}
	if (is_red(lex, i) && lex.tokens[i + 1].type != T_WORD)
	{
		ft_putendl_fd("bash: syntax error near unexpected token `newline'", 2);
		exit(1);
	}
}
void      free_the_op_content_array_token(t_lexer *lexer)
{
  int i;

  i = -1;
  while (++i < lexer->used_size)
  {
	  if(lexer->used_size == 1)
		break;
	  else if (lexer->tokens[i].type != T_WORD && !is_red(*lexer, i))
	  	free(lexer->tokens[i].content);
  }
  free(lexer->tokens);
}

void	free_struct(t_command *cmd, t_lexer lex)
{
	int i;

	i = 0;
	while(i < cmd->used_space)
	{
		free_str(cmd->command[i].cmd_simple);
		free_str_2(cmd->command[i].redirection.red,
		cmd->command[i].redirection.used_space);
		free_str_2(cmd->command[i].redirection.file, 
		cmd->command[i].redirection.used_space);
		free(cmd->command[i].redirection.fd);
		i++;
	}
	free(cmd->command);



}