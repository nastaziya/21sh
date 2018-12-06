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

#include "../../inc/sh.h"
#include "../../inc/builtin.h"

// void		ft_print_env(char ***c_env)
// {
// 	int	i;

// 	i = -1;
// 	while (c_env[0][++i])
// 	{
// 		ft_putstr(c_env[0][i]);
// 		ft_putchar('\n');
// 	}
// }

// void		ft_print_error_env(char *av)
// {
// 	ft_putstr_fd("env: ", 2);
// 	ft_putstr_fd(av, 2);
// 	ft_putstr_fd(": No such file or directory\n", 2);
// }

// char	**ft_build_exec_path(char **path, char **av)
// {
// 	int		i;
// 	char	*tmp;
// 	char	*tmp2;

// 	i = -1;
// 	tmp = ft_strdup(av[0]);
// 	tmp2 = ft_strjoin("/", tmp);
// 	free(tmp);
// 	while (path[++i])
// 	{
// 		tmp = path[i];
// 		path[i] = ft_strjoin(tmp, tmp2);
// 		free(tmp);
// 	}
// 	free(tmp2);
// 	return (path);
// }

// char	**ft_find_path_and_split(char **c_env, char **av)
// {
// 	int		i;
// 	char	**ret;
// 	char	**res;

// 	i = 0;
// 	while (c_env[i] && ft_strncmp(c_env[i], "PATH=", 5))
// 		i++;
// 	if (c_env[i] && c_env[i][6])
// 	{
// 		ret = ft_strsplit(c_env[i], '=');
// 		res = ft_strsplit(ret[1], ':');
// 		ft_free_av(ret);
// 		res = ft_build_exec_path(res, av);
// 	}
// 	else
// 		res = ft_strsplit("cest=un=test", '=');
// 	return (res);
// }

// void	ft_cp_env(char ***c_env, char **env)
// {
// 	int	i;

// 	i = 0;
// 	while (env[i])
// 		i++;
// 	if (!(c_env[0] = (char **)malloc(sizeof(char *) * (i + 1))))
// 		return ;
// 	i = -1;
// 	while (env[++i])
// 		c_env[0][i] = ft_strdup(env[i]);
// 	c_env[0][i] = NULL;
// }

// static int	ft_builtin_env3(char **av, char **cp_c_env, int i)
// {
// 	int ret;

// 	ret = 0;
// 	if (ft_fork(av + i, av[i], &cp_c_env, 1))
// 		ft_print_error_env(av[i]);
// 	else
// 		ret = 1;
// 	return (ret);
// }
// // int		error_exec_or_exec(char **paths, char *path, char **str,
// // 		char **env)
// // int			ft_fork(char **av, char *path, char ***c_env, int bolean)


// static int	ft_builtin_env2(char **av, char **cp_c_env, int i, int ret)
// {
// 	char	**path;
// 	int		j;

// 	j = 0;
// 	if (!ft_strchr(av[i], '/'))
// 	{
// 		path = ft_find_path_and_split(cp_c_env, av + i);
// 		while (path[++j])
// 			if (ft_fork(av + i, path[j], &cp_c_env, 0))
// 			{
// 				ret = 1;
// 				break ;
// 			}
// 		if (!path[j])
// 			ft_print_error_env(av[i]);
// 		ft_free_av(path);
// 	}
// 	else
// 	{
// 		if (ft_builtin_env3(av, cp_c_env, i))
// 			ret = 1;
// 	}
// 	return (ret);
// }


// int			ft_fork(char **av, char *path, char ***c_env, int bolean)
// {
// 	pid_t		father;
// 	struct stat	buf;

// 	if ((g_glob = 1) && lstat(path, &buf) != -1)
// 	{
// 		father = fork();
// 		if (father == 0)
// 		{
// 			if (S_ISREG(buf.st_mode))
// 			{
// 				if (execve(path, av, *c_env) == -1)
// 					exit(1);
// 			}
// 			else
// 				exit(1);
// 		}
// 		if (father > 0)
// 			wait(NULL);
// 	}
// 	g_glob = 0;
// 	if (bolean == 0 && lstat(path, &buf) != -1)
// 		return (1);
// 	else if (bolean == 1 && lstat(path, &buf) != -1 && S_ISDIR(buf.st_mode))
// 		ft_print_error(av[0], ": Permission denied.\n");
// 	else if (bolean == 1 && lstat(path, &buf) == -1)
// 		ft_print_error_without_char(av[0], ": Command not found.\n");
// 	return (0);
// }

/*
*** - Gérer -> env, env -i
*** - Usage env => tant qu'il y a un "-", on vérifie si option du builtin
*** - sinon, après, on envoie le reste à l'execve
*/

int			ft_builtin_env(char **av, char ***c_env, char ***paths, t_env_tools *env)
{
	// int		argc;
	// char	**cp_c_env;
    // int     i;
    // int     ret;

    // i = 0;
    // ret = 0;
	// argc = ft_len_array_char(av);
	// ft_cp_env(&cp_c_env, *c_env);
	// if (argc == 1)
	// 	ft_print_env(c_env);
	// else
	// 	while (av[++i] && ret == 0)
	// 	{
	// 		if (ft_strchr(av[i], '='))
	// 		{
	// 			ft_builtin_setenv_2(av[i], &cp_c_env, paths, env);
	// 			if (!av[i + 1])
	// 				ft_print_env(&cp_c_env);
	// 		}
	// 		else
	// 		{
	// 			if (ft_builtin_env2(av, cp_c_env, i, 0))
	// 				ret = 1;
	// 		}
	// 	}
	// ft_free_av(cp_c_env);
	return (0);
}
