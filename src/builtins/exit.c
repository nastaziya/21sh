/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   exit.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/09 17:36:12 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 17:39:02 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include "../../inc/termcap.h"

void	ft_free_all(t_env_tools *env)
{
	free_the_op_content_array_token(env->p.lex);
	free_struct(env->p.cmd);
	if (env->env_cpy != NULL)
		free_str(env->env_cpy);
	ft_dlstdel(env->p.history);
	if (env->paths)
		ft_free_av(env->paths);
	if (env->home)
		free(env->home);
	ft_free_av(*(env->p.cmd_expended));
	if (*(env->p.heredoc))
	{
		ft_free_av(*(env->p.heredoc));
		*(env->p.heredoc) = NULL;
	}
	if (env->t)
		delete_hash_table(&(env->t));
}

int		ft_print_usage_error_exit(char *btwn)
{
	ft_putstr_fd(": exit: ", 2);
	ft_putstr_fd(btwn, 2);
	ft_putendl_fd(": numeric argument required", 2);
	exit(255);
	return (255);
}

int		ft_check_num(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (!ft_isdigit(str[i]))
			return (1);
	return (0);
}

int		ft_builtin_exit(char **cmd, t_env_tools *env)
{
	int	argc;

	if (((argc = ft_len_array_char(cmd)) > 2))
		return (ft_int_error("exit: Too many arguments.\n", 2, 1));
	else if (argc == 2 && ft_check_num(cmd[1]))
		return (ft_print_usage_error_exit(cmd[1]));
	else if (argc == 1)
	{
		ft_free_all(env);
		exit(0);
	}
	else if (argc == 2)
	{
		ft_free_all(env);
		exit(atoi(cmd[1]) % 256);
	}
	return (0);
}
