/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   unsetenv.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/07/22 05:44:32 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/07/22 06:05:25 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"

void	ft_builtin_unsetenv_2(char *av, char ***c_env, char ***paths, t_env_tools *env)
{
	int		i;
	char	*ret;

	i = 0;
	ret = NULL;
	while ((*c_env)[i] && ft_strncmp((*c_env)[i], av, ft_strlen(av)))
		i++;
	if ((!ft_strcmp(av, "PATH") || !ft_strcmp(av, "PATH="))
		&& !ft_free_av(*paths))
		*paths = ft_strsplit(" ", ':');
	if ((!ft_strcmp(av, "HOME") || !ft_strcmp(av, "HOME="))
		&& !ft_free(env->home))
		env->home = ft_strdup(getenv("HOME"));
	if ((*c_env)[i])
	{
		ft_exchange_chars(*c_env, ' ', (char)255);
		ret = ft_array_char_to_str(*c_env, i);
		ft_free_av(c_env[0]);
		c_env[0] = ft_strsplit(ret, ' ');
		ft_exchange_chars(*c_env, (char)255, ' ');
		free(ret);
	}
}

int		ft_builtin_unsetenv(char **av, char ***c_env, char ***paths, t_env_tools *env)
{
	int		i;
	int		len;

	i = 0;
	len = ft_len_array_char(av);
	if (len == 1 && !ft_int_error("unsetenv: Too few arguments\n", 2))
		return (1);
	while (len != 1 && av[++i])
		ft_builtin_unsetenv_2(av[i], c_env, paths, env);
	return (0);
}