/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 22:17:47 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"

char		**ft_find_path_and_split(char **c_env)
{
	int		i;
	char	**ret;
	char	**res;

	i = 0;
	while (c_env[i] && ft_strncmp(c_env[i], "PATH=", 5))
		i++;
	if (c_env[i] && c_env[i][6])
	{
		ret = ft_strsplit(c_env[i], '=');
		res = ft_strsplit(ret[1], ':');
		ft_free_av(ret);
	}
	else
		res = ft_strsplit("cest=un=test", '=');
	return (res);
}

void		ft_cp_env(char ***c_env, char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	if (!(c_env[0] = (char **)malloc(sizeof(char *) * (i + 1))))
		return ;
	i = -1;
	while (env[++i])
		c_env[0][i] = ft_strdup(env[i]);
	c_env[0][i] = NULL;
}

int			ft_builtin_env2(char **av, char **cp_c_env, int i, int ret)
{
	char	**path;

	path = ft_find_path_and_split(cp_c_env);
	ret = error_exec_or_exec(path, av + i, cp_c_env, 2);
	ft_free_av(path);
	return (ret = 0 ? 2 : ret);
}

int			ft_manage_option_i_env(char ***cp_c_env, char **env)
{
	int	i;

	i = -1;
	ft_free_av(*cp_c_env);
	if (!(cp_c_env[0] = (char**)malloc(sizeof(char*) * 6)))
		return (1);
	while (env[++i])
	{
		if (!ft_strncmp(env[i], "PATH=", 5))
			cp_c_env[0][0] = ft_strdup(env[i]);
		else if (!ft_strncmp(env[i], "SHLVL=", 6))
			cp_c_env[0][1] = ft_strdup(env[i]);
		else if (!ft_strncmp(env[i], "PWD=", 4))
			cp_c_env[0][2] = ft_strdup(env[i]);
		else if (!ft_strncmp(env[i], "HOME=", 5))
			cp_c_env[0][3] = ft_strdup(env[i]);
		else if (!ft_strncmp(env[i], "TERM=", 5))
			cp_c_env[0][4] = ft_strdup(env[i]);
	}
	cp_c_env[0][5] = NULL;
	return (0);
}

/*
*** - Gérer -> env, env -i
*** - Usage env => tant qu'il y a un "-", on vérifie si option du builtin
*** - sinon, après, on envoie le reste à l'execve
*/

int			ft_builtin_env(char **av, char ***c_env)
{
	int			i;
	t_norm_env	t;

	t.ret = 0;
	i = -1;
	t.argc = ft_len_array_char(av);
	ft_cp_env(&(t.cp_c_env), *c_env);
	if (t.argc == 1)
		ft_print_env(c_env);
	else
	{
		while (++i < t.argc && t.ret == 0)
			ft_norm_env(av, i, &t, c_env);
	}
	if (t.cp_c_env && *(t.cp_c_env))
		ft_free_av(t.cp_c_env);
	return (t.ret == 2 ? 0 : t.ret);
}
