/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/19 18:02:22 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/builtin.h"

int		ft_usage_error(char *command, char *btwn, char *after, int i)
{
	ft_putstr_fd(command, 2);
	ft_putstr_fd(btwn, 2);
	ft_putstr_fd(command, 2);
	ft_putendl_fd(after, 2);
	return (i);
}

// Aim of the function ? Int in order to put it inside if statement
int		ft_int_error(char *str, int fd)
{
	ft_putstr_fd(str,fd);
	return (0);
}

// Aim of the function ? Int in order to put it inside if statement
int		ft_int_print_error(char *str, char *str2, char *str3, int fd)
{
	ft_putstr_fd(str, fd);
	ft_putstr_fd(str2, fd);
	ft_putstr_fd(str3, fd);
	return (0);
}

int		ft_len_array_char(char **av)
{
	int	i;

	i = 0;
	while (av[i])
		i++;
	return (i);
}

int		ft_exec_builtin(t_env_tools *env, char **cmd)
{
	if (!ft_strcmp("echo", cmd[0])) // gérer le cas où VAR $HOME n'existe plus
		return (ft_builtin_echo(cmd, &(env->env_cpy)));
	else if (!ft_strcmp("setenv", cmd[0]))
		return (ft_builtin_setenv(cmd, &(env->env_cpy), &(env->paths), env));
	else if (!ft_strcmp("unsetenv", cmd[0]))
		return (ft_builtin_unsetenv(cmd, &(env->env_cpy), &(env->paths), env));
	else if (!ft_strcmp("cd", cmd[0]))
		return (ft_builtin_cd(cmd, &(env->env_cpy), env));
	else if (!ft_strcmp("env", cmd[0]))
	// OPTIONS POSIX
		return (5);
	else if (!ft_strcmp("exit", cmd[0]))
	// REGARDER COMMENT GERER les frees
		return (5);
	return (0);
}