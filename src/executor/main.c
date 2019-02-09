/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 19:19:18 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include "../../inc/expansion.h"
#include "../../inc/exec.h"

void	check_op(t_command cmd, t_env_tools *env, char ***heredoc)
{
	t_exec_redir	t;
	int				i;

	i = 0;
	t.i_hdoc = 0;
	t.heredoc = heredoc;
	save_original_fd(&t);
	if (cmd.command[i].tok == T_PIPE)
	{
		ft_pipe_exec(env, cmd, &i, &t);
		i--;
	}
	else
	{
		ft_first_exec(env, cmd, 0, &t);
		if (cmd.used_space > 1 && cmd.command[i + 1].tok != T_PIPE)
			i--;
	}
	restore_original_fd(&t);
	while (++i < cmd.used_space && cmd.command[i].tok != -1)
	{
		if (i == cmd.used_space - 1 || cmd.command[i + 1].used_space == 0)
			break ;
		save_original_fd(&t);
		if (cmd.command[i].tok == T_DBLOR)
			ft_or_exec(env, cmd, i + 1, &t);
		else if (cmd.command[i].tok == T_PIPE)
		{
			ft_pipe_exec(env, cmd, &i, &t);
			i--;
		}
		else if (cmd.command[i].tok == T_DBLAND)
			ft_and_exec(env, cmd, i + 1, &t);
		else if (cmd.command[i].tok == T_SEMI)
			ft_semi_exec(env, cmd, i + 1, &t);
		restore_original_fd(&t);
	}
}

void	all_exec(char **environ, char ***heredoc)
{
	t_dlist			*history;
	t_lexer			lex;
	t_command		cmd;
	t_env_tools		env;

	env.env_cpy = copy_env(environ, size_str(environ), &env);
	path_str(env.env_cpy, &env.paths);
	history = ft_dlstnew(NULL);
	g_keeprun = 0;
	while (42)
	{
		lex = final_tokens(&history);
		command_init(&cmd);
		add_simple_command(&cmd, lex, &history, heredoc);
		if (g_keeprun == 1 || g_keeprun == 2)
			env.g_return_value = 1;
		else if (g_keeprun == 5)
			env.g_return_value = 258;
		if (!g_keeprun || g_keeprun == 4)
			check_op(cmd, &env, heredoc);
		if (*heredoc)
		{
			ft_free_av(*heredoc);
			*heredoc = NULL;
		}
		free_the_op_content_array_token(&lex);
		free_struct(&cmd);
	}
	if (env.env_cpy != NULL)
		free_str(env.env_cpy);
	ft_dlstdel(&history);
}

int		main(int argc, char **argv, char **environ)
{
	t_term		term;
	char		**heredoc;

	(void)argc;
	(void)argv;
	heredoc = NULL;
	if (terminal_data(&term) < 0)
	{
		ft_putstr_fd("envoie les bonnes infos ma gueule !!!\n", 2);
		exit(0);
	}
	all_exec(environ, &heredoc);
	return (0);
}
