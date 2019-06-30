/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 22:23:18 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include "../../inc/expansion.h"
#include "../../inc/exec.h"

void	save_original_fd(t_exec_redir *t)
{
	int i;

	i = -1;
	while (++i < 3)
		t->fd_orig[i] = dup2(i, 1500 + i);
}

int		ft_or_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t)
{
	char	**cmd_expended;
	int		ret;

	ret = 0;
	cmd_expended = expense_cmd(cmd, *env, i);
	env->p.cmd_expended = &cmd_expended;
	if (env->g_return_value > 0)
	{
		if (cmd.command[i].redirection.used_space > 0)
			ret = process_redirections(t, cmd.command[i], env,
				i - t->count_here);
		if (!ret)
			env->g_return_value = ft_exec_command(env, cmd_expended, 1);
	}
	free_str(cmd_expended);
	return (0);
}

int		ft_and_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t)
{
	char	**cmd_expended;
	int		ret;

	ret = 0;
	cmd_expended = expense_cmd(cmd, *env, i);
	env->p.cmd_expended = &cmd_expended;
	if (env->g_return_value == 0)
	{
		if (cmd.command[i].redirection.used_space > 0)
			ret = process_redirections(t, cmd.command[i], env,
				i - t->count_here);
		if (!ret && cmd.command[i].tok != T_PIPE)
			env->g_return_value = ft_exec_command(env, cmd_expended, 1);
	}
	free_str(cmd_expended);
	return (0);
}

int		ft_semi_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t)
{
	char	**cmd_expended;
	int		ret;

	ret = 0;
	cmd_expended = expense_cmd(cmd, *env, i);
	env->p.cmd_expended = &cmd_expended;
	if (cmd.command[i].redirection.used_space > 0)
		ret = process_redirections(t, cmd.command[i], env, i - t->count_here);
	if (!ret)
		env->g_return_value = ft_exec_command(env, cmd_expended, 1);
	free_str(cmd_expended);
	return (0);
}

int		ft_first_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t)
{
	char	**cmd_expended;
	int		ret;

	ret = 0;
	cmd_expended = NULL;
	if (cmd.used_space > 0)
	{
		cmd_expended = expense_cmd(cmd, *env, 0);
		env->p.cmd_expended = &cmd_expended;
		if (cmd.command[0].redirection.used_space > 0)
			ret = process_redirections(t, cmd.command[i], env,
				i - t->count_here);
		if (cmd_expended != NULL && cmd.used_space > 0 && !ret)
			env->g_return_value = ft_exec_command(env, cmd_expended, 1);
		free_str(cmd_expended);
	}
	return (0);
}
