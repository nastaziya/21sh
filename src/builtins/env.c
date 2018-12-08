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

int		ft_print_error_env(char *av)
{
	ft_putstr_fd("env: ", 2);
	ft_putstr_fd(av, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
    return (127);
}

char	**ft_build_exec_path(char **path, char **av)
{
	int		i;
	char	*tmp;
	char	*tmp2;

	i = -1;
	tmp = ft_strdup(av[0]);
	tmp2 = ft_strjoin("/", tmp);
	free(tmp);
	while (path[++i])
	{
		tmp = path[i];
		path[i] = ft_strjoin(tmp, tmp2);
		free(tmp);
	}
	free(tmp2);
	return (path);
}

// char	**ft_find_path_and_split(char **c_env, char **av)
char	**ft_find_path_and_split(char **c_env)
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

void	ft_cp_env(char ***c_env, char **env)
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

static int	ft_builtin_env2(char **av, char **cp_c_env, int i, int ret)
{
	char	**path;
	
    path = ft_find_path_and_split(cp_c_env);
	if (!ft_strchr(av[i], '/'))
        ret = error_exec_or_exec(path, av + i, cp_c_env, 1);
	else
        ret = error_exec_or_exec(path, av + i, cp_c_env, 1);
    ft_free_av(path);
	return (ret);
}

int         ft_usage_env_builtin(char **av, int argc, int *i, char *p)
{
    // int     argc;

    *i = (argc > 1 ? 1 : 0);
    if (argc > 1)
    {
        while (av[*i] && !ft_strcmp(av[*i], "-i") && (*p = 'i'))
            (*i)++;
        // dprintf(2, "[%s]\n", av[*i]);
        if (av[*i] && ft_strcmp(av[*i], "-") && av[*i][0] == '-')
            return (ft_usage_error("env: illegal option -- ", av[*i], "\nusage: env [-i] [name=value ...] [utility [argument ...]]", 1));
        else if (av[*i] && av[*i][0] == '-' && *i == argc)
            return (2);
    }
    dprintf(2, "av[sortie]: |%s|\n", av[*i]);
    return (0);
}

/*
*** - Gérer -> env, env -i
*** - Usage env => tant qu'il y a un "-", on vérifie si option du builtin
*** - sinon, après, on envoie le reste à l'execve
*/

// 1 - passer tous les -i -i -i -i => erreur : usage
// 2 - si -i, prendre un env vide, sinon copie env
// 3 - si "=" après "env -i -i" -> rajouter dans l'env
// 4 - 
int			ft_builtin_env(char **av, char ***c_env, char ***paths, t_env_tools *env)
{
	int		argc;
	char	**cp_c_env;
    int     i;
    int     ret;
    char    p;

    // ret = 0;
    p = 0;
    // i = 0;
    // ajouter fonction gestion de l'usage + commencer au bon endroit
    // -> Après les -i, donc le i est set dans la fonction
	// if (ft_norm_av(&av, &n.c, &n.begin))
    //     return (1);
    argc = ft_len_array_char(av);
    if ((ret = ft_usage_env_builtin(av, argc, &i, &p)))
        return (ret == 2 ? 0 : 1);
    dprintf(2, "builtin env: %d - %c - %d\n", i, p, argc);
    // gérer avec le -i
	ft_cp_env(&cp_c_env, *c_env);
	if (argc == 1)//  || i == argc ------ //i == argc - 1
		ft_print_env(c_env);
	else
    {
        ////////Gérer env -i PIPI=POPO => Affiche PIPI=POPO
    //////// Gérer env PIPI=POPO => Affiche env + PIPI=POPO
    //////// gérer env ls -> ne fonctionne pas
    /////// Gérer option -i => copie vide
        --i;
		while (av[++i] && ret == 0)
		{
			if (ft_strchr(av[i], '='))
			{
				ft_builtin_setenv_2(av[i], &cp_c_env, paths, env);
				if (!av[i + 1])
					ft_print_env(&cp_c_env);
			}
			else if (i != argc - 1 ? ft_strcmp(av[i], "env") : 1)
			    ret = ft_builtin_env2(av, cp_c_env, i, 0);
		}
    }
	ft_free_av(cp_c_env);
	return (ret);
}
