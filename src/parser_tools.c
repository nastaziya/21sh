#include "../inc/sh.h"

int		is_red(t_lexer lex, int i)
{
	if ((lex.tokens[i].type == T_GREAT || lex.tokens[i].type == T_LESS ||
		lex.tokens[i].type == T_DBL_GREAT || lex.tokens[i].type == T_DBL_LESS))
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
	if ((is_red(lex, i) && is_red(lex, i + 1) && is_op(lex, i + 1)) ||
	(is_op(lex, i) && is_op(lex, i + 1)))
	{
		ft_putstr_fd("bash: syntax error near unexpected token ", 2);
		ft_putendl_fd(lex.tokens[i + 1].content, 2);
		exit(0);
	}
	if (is_red(lex, i) && lex.tokens[i + 1].type != T_WORD)
	{
		ft_putendl_fd("bash: syntax error near unexpected token `newline'", 2);
		exit(0);
	}
}