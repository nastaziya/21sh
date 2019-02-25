/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   parser.h                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/06 00:54:31 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/06 00:54:34 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"
# include "../libft/libft.h"

# define TAB_INITIAL_CAPACITY 60

typedef struct		s_red
{
	int				*fd;
	t_token_type	*red;
	char			**file;
	int				used_space;
	int				av_space;
}					t_red;

typedef struct		s_simp_com
{
	char		**cmd_simple;
	int			av_space;
	int			used_space;
	int			tok;
	t_red		redirection;
}					t_simp_com;

typedef struct		s_command
{
	t_simp_com	*command;
	int			av_space;
	int			used_space;
}					t_command;

/*
*** - struct only built for the norm of
*** - the heredoc
*/

typedef struct		s_hdoc
{
	int			i;
	int			j;
	int			command;
	int			words[50];
	int			i_words;
	int			k;
	char		*cmd;
	int			obool;
}					t_hdoc;

void				print_array(int size, char **cmd);
void				print_struct(t_command cmd);
int					tab_init(t_simp_com *simple_cmd);
void				tab_red_init(t_red *redir);
void				tab_assign(t_simp_com *simple_cmd, t_lexer lex, int j);
void				simple_cmd_assign(t_command *cmd, t_simp_com simple_cmd);

/*
*** - utils_parser.c
*/

void				command_init(t_command *cmd);
void				tab_red_assign(t_red *redir, t_lexer lex, int j, int k);
void				add_simple_command(t_command *cmd, t_lexer lex,
						t_dlist **history, char ***heredoc);
int					is_red(t_lexer lex, int i);
int					is_op(t_lexer lex, int i);
int					parse_errors(t_lexer lex, int i);
int					check_path(char **path_env, char **path, char **str);
int					is_built_in(char **cmd);
int					ft_parse_error_for_heredoc(t_lexer lex);

#endif
