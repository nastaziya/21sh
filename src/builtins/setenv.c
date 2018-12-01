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

char		*ft_array_char_to_str_replace_env(char **c_env, int avoid, char *av)
{
	char	*ret;
	char	*tmp;
	char	*cp_av;
	int		i;

	i = -1;
	ret = NULL;
	tmp = NULL;
	cp_av = NULL;
	while (c_env[++i])
	{
		if (i != avoid)
			ft_swap_string(&ret, &tmp, &c_env[i]);
		else
		{
			if (!ft_strchr(av, '='))
				cp_av = ft_strjoin(av, "=");
			else
                cp_av = ft_strdup(av);
			ft_swap_string(&ret, &tmp, &cp_av);
			free(cp_av);
		}
	}
	return (ret);
}

static void	ft_builtin_setenv_3(char ***c_env, int i, char *tmp, char ***paths)
{
	char	*ret;
	char	*ret2;

	ret2 = ft_array_char_to_str_replace_env(*c_env, i, tmp);
	ret = ft_strjoin(ret2, tmp);
	free(ret2);
	free(tmp);
	ft_free_av(c_env[0]);
	c_env[0] = ft_strsplit(ret, ' ');
	ft_exchange_chars(*c_env, (char)255, ' ');
	free(ret);
}

static void	ft_builtin_setenv_2_norm(int i, char ***c_env, char *tmp)
{
	char *ret;

	ret = ft_array_char_to_str_replace_env(*c_env, i, tmp);
	ft_free_av(c_env[0]);
	*c_env = ft_strsplit(ret, ' ');
	ft_exchange_chars(c_env[0], (char)255, ' ');
	free(ret);
	free(tmp);
}

//Normer setenv_2
void		ft_builtin_setenv_2(char *av, char ***c_env, char ***paths, t_env_tools *env)
{
	int		len;
	// char	*ret;
	char	*tmp;
	int		i;

	i = 0;

    // dprintf(2, "|||||| YOUHOU JE CHANGE |||||||");
	len = (ft_strchr(av, '=') ? ft_strchr(av, '=') - av : ft_strlen(av));
	tmp = ft_strdup_without_quotes(av);
	ft_exchange_chars(*c_env, ' ', (char)255);
	while ((*c_env)[i] && ft_strncmp((*c_env)[i], av, len))
		i++;
	// Manage cpy env HOME
	if (!ft_strncmp(tmp, "HOME=", 5) && !ft_free(env->home))
		env->home = (ft_strlen(tmp) == 5 ? ft_strdup(getenv("HOME")) : ft_strdup(tmp + 5));
	// modify the copy of the str -> we execute on this copy
	if (!ft_strncmp(tmp, "PATH=", 5) && !ft_free_av(*paths))
		*paths = ft_strsplit(ft_strchr(tmp, '='), ':');
	if ((*c_env)[i]) // if we have found the env
		ft_builtin_setenv_2_norm(i, c_env, tmp);
	else //if the env is not present here
		ft_builtin_setenv_3(c_env, i, tmp, paths);
}

//CSH - HOME
	//// je gère tout au niveau de setenv -> Plus simple

	//1. récupérer valeur de l'env HOME
	//2. Vérifier si Home[0] == / ou non
	// 3. Si == /, alors on prend le / et on modifie la copie pour qu'elle soit == à l'home de l'env
	// 4. Si Home[0] != backslash
		// 4.1 - On regarde si HOME == getenv
			// Si c'est le cas -> on modifie la copie et l'env == HOME
			// et donc echo ~ == au retour de getenv
		// 4.2 - Si ce n'est pas le cas
			// On prend la copie et on met le current working directory + ce qui est écrit dans l'env HOME

//BASH - HOME
	/// Quand modif HOME, ça modifie et la copie, et l'env
	// Mais quand unsetenv HOME -> COPY = getenv(HOME)

int			ft_builtin_setenv(char **av, char ***c_env, char ***paths, t_env_tools *env)
{
	int		len;

	len = ft_len_array_char(av);
	if (len == 1)
		ft_print_env(c_env);
	if (len > 2)
		return (ft_int_error("setenv: Too many arguments.\n", 2, 1));
	ft_exchange_chars(av, ' ', (char)255);
	if (len == 2)
	{
		if (!ft_isalpha(av[1][0]))
            return (ft_int_error("setenv: Variable name must begin with a letter.\n", 2, 1));
        else if (ft_str_is_alpha_setenv(av[1])
			&& !ft_int_error("setenv: Variable name must", 2, 0))
                return (ft_int_error(" contain alphanumeric characters.\n", 2, 1));
        else
			ft_builtin_setenv_2(av[1], &(env)->env_cpy, &(env)->paths, env);// c_env, paths
	}
	return (0);
}
