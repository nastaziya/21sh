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
	return (ret = 0 ? 2 : ret); // modifier le ret je pense
}

int     ft_manage_option_i_env(char	***cp_c_env, char **env)
{
	int i;

	i = -1;
	ft_free_av(*cp_c_env);
    if (!(cp_c_env[0] = (char**)malloc(sizeof(char*) * 2)))
		return (1);
    while (env[++i])
		if (!ft_strncmp(env[i], "PATH=", 5))
			cp_c_env[0][0] = ft_strdup(env[i]);
	// cp_c_env[0][0] = NULL;
    cp_c_env[0][1] = NULL;
    return (0);
}

/*
*** - Gérer -> env, env -i
*** - Usage env => tant qu'il y a un "-", on vérifie si option du builtin
*** - sinon, après, on envoie le reste à l'execve
*/

int			ft_builtin_env(char **av, char ***c_env)
{
	int		argc;
	char	**cp_c_env;
    int     i;
    int     ret;

    ret = 0;
	i = -1;
    argc = ft_len_array_char(av);
	ft_cp_env(&cp_c_env, *c_env);
	if (argc == 1)
		ft_print_env(c_env);
	else
    {
		while (++i < argc && ret == 0)
		{
			dprintf(3, "builtin_env_loop: %d|%s\n", i-1, av[i]);
			// dprintf(3, "av[i]-env: |%s|\n", av[i]);
			// ajoute dans la copie de l'env
			if (ft_strchr(av[i], '='))
			{
				ft_builtin_setenv_env_builtin(av[i], &cp_c_env);
				if (!av[i + 1])
					ft_print_env(&cp_c_env);
			}
			// empty the env_cpy
			else if (av[i] && av[i][0] == '-' && !ft_usage_is_good("i", av[i])
				&& (i > 1 ? (!ft_strcmp(av[i - 1], "env")
					|| av[i - 1][0] == '-') : 1)) //  && (i > 1 ? av[i - 1][0] == '-' : 1)
				ft_manage_option_i_env(&cp_c_env, *c_env);
			// illegal option, stops everything
			else if (av[i] && av[i][0] == '-' && ft_usage_is_good("i", av[i])
				&& (ret = 1))
				ft_usage_error_env("env: illegal option -- ", av[i],
            "\nusage: env [-i] [name=value ...] [utility [argument ...]]", 1);
			// prints the env, if env is at the end
			// else if (!ft_strcmp(av[i], "env") && (ret = 2))
			// 	ft_builtin_env(av + i, &cp_c_env, paths, env);
			else if (i == argc - 1 && !ft_strcmp(av[i], "env"))
				ft_print_env(&cp_c_env);
			else if (ft_strcmp(av[i], "env"))// (i > 1 ? av[i - 1][0] == '-' : 1) && 
				ret = ft_builtin_env2(av, cp_c_env, i, 0);
		}
    }
	if (cp_c_env && *cp_c_env)
		ft_free_av(cp_c_env);
	return (ret == 2 ? 0 : ret);
}


// ma nouvelle logique : 


// Je calcule la taille de l'av
// Je copie l'env complet
// je boucle sur mes av 
// 
// si jamais -i, je checke les option. Si erreur je la print et tout s'arrête
// et si "-i" je free la copie de l'env, et j'en ressort une propre
// 
// si jamais "=", j'ajoue à la copie de l'env
// if après un -i ou un "=" autre chose que env -> execution de la commande, sinon, continuer la boucle
// Demander si, quand on modifie la variable PATH dans l'env, l'exécution doit planter aussi -> je pense que oui


// Modifier cp_c_env pour qu'il prenne n'affiche pas le PATH si path cp_c_env == path c_env
// et quand on envoie dans la fonction le c_env

// Modifier setenv pour qu'il ne prenne pas en compte le changement de la variable paths qui stocke tous les paths

// Problème de l'env -> Après un strchr(av[i], '='), si jamais l'av d'après != "env", alors l'exécuter