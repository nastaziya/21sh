/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   unsetenv.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/07/22 05:44:32 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 18:20:59 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"

/*
*** - Aim of the function :
*** - allocates the new env environment
*** - If env empty -> allocate a new empty one to avoid segfault
*/

static void		ft_builtin_unsetenv_2_norm(char ***c_env)
{
	if (c_env && *c_env)
		ft_exchange_chars(*c_env, (char)255, ' ');
	else
	{
		if (!(*c_env = (char**)malloc(sizeof(char*) * 1)))
			return ;
		**c_env = ft_strnew(0);
		**c_env = NULL;
	}
}

/*
*** - Aim of the function :
*** - Following of the unsetenv function
*** - We also manage the unsetenv PATH (we modify it's copy)
*** - Same for the HOME
*** - Then we unset the proper variable
*** - So, when we unset PATH, then env -> the env will not
*** - show the PATH and ls will not work
*** - But, if we do the same : uset HOME, then echo ~ -> the expansion works
*** - So, there is a copy of the HOME that the expansion uses
*/

static void		ft_builtin_unsetenv_2(char *av, char ***c_env,
					char ***paths, t_env_tools *env)
{
	int		i;
	char	*ret;

	i = 0;
	ret = NULL;
	while ((*c_env)[i] && ft_strncmp((*c_env)[i], av, ft_strlen(av)))
		i++;
	if ((!ft_strcmp(av, "PATH") || !ft_strcmp(av, "PATH="))
		&& !ft_free_av(*paths))
	{
		*paths = ft_strsplit(" ", ':');
		if (env->t)
			delete_hash_table(&(env->t));
	}
	if ((!ft_strcmp(av, "HOME") || !ft_strcmp(av, "HOME="))
		&& !ft_free(env->home))
		env->home = ft_strdup(getenv("HOME"));
	if ((*c_env)[i])
	{
		ft_exchange_chars(*c_env, ' ', (char)255);
		ret = ft_array_char_to_str(*c_env, i);
		ft_free_av(c_env[0]);
		c_env[0] = ft_strsplit(ret, ' ');
		ft_builtin_unsetenv_2_norm(c_env);
		free(ret);
	}
}

/*
*** - Aim of the function :
*** - Copies the unsetenv builtin
*/

int				ft_builtin_unsetenv(char **av, char ***c_env,
					char ***paths, t_env_tools *env)
{
	int		i;
	int		len;

	i = 0;
	len = ft_len_array_char(av);
	if (len == 1)
		return (ft_int_error("unsetenv: Too few arguments\n", 2, 1));
	while (len != 1 && av[++i])
		ft_builtin_unsetenv_2(av[i], c_env, paths, env);
	return (0);
}
