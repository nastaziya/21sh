/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/03/22 14:20:42 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include "../../inc/hash_table.h"
#include "stdio.h"

int			ft_check_arguments(int *i, char **cmd, int *boul, t_env_tools *env)
{
	while (cmd[*i] && cmd[*i][0] == '-')
	{
		dprintf(3, "je passe\n");
		if (ft_usage_is_good("r", cmd[*i]))
		{
			dprintf(1, "bash: hash: %s: invalid option\n", cmd[*i]);
			dprintf(1, "hash: usage: hash [-r] [name ...]\n");
			return (2);
		}
		*boul = TO_RESET;
		(*i)++;
	}
	if (--(*i) && *boul == TO_RESET && env->t)
		delete_hash_table(&(env->t));
	return (0);
}

void		add_binary_to_hash(int *i, char **cmd, t_env_tools *env,
				char **path)
{
	while (cmd[++(*i)])
	{
		if (ft_strchr(cmd[*i], '/'))
			;
		else
		{
			if (env->paths)
			{
				check_path(env->paths, path, &(cmd[*i]));
				if (!(*path))
				{
					dprintf(1, "%s", "bash: hash:");
					dprintf(1, "%s: not found\n", cmd[*i]);
				}
				else
				{
					if (!env->t)
						env->t = new_hash_table();
					insert_element(env->t, cmd[*i], *path);
				}
			}
		}
	}
}

int			ft_builtin_hash(char **cmd, t_env_tools *env)
{
	int			i;
	int			len;
	int			boul;
	char		*path;

	i = 1;
	path = NULL;
	len = ft_len_array_char(cmd);
	boul = NO_RESET;
	if (len == 1)
		ft_print_hash(env);
	else
	{
		if (ft_check_arguments(&i, cmd, &boul, env))
			return (2);
		add_binary_to_hash(&i, cmd, env, &path);
		if (path != NULL)
			free(path);
		path = NULL;
	}
	return (0);
}
