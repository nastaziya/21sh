/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/12 21:59:12 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"

/*
*** - Function to manage :
*** - cd alone
*** - cd -
*** - ft_find_path_and_cd2 => Manage the errors
*** - p == 0 for cd -P - ; otherwise p == 1
*/

int		ft_find_path_and_cd(char c, char ***c_env, t_env_tools *env,
					t_norm_cd *n)
{
	int		count;
	char	*tmp;
	int		ret;

	count = 0;
	while ((*c_env)[count] && ft_strncmp((*c_env)[count],
				(c == '~' ? "HOME=" : "OLDPWD="), (c == '~' ? 5 : 7)))
		count++;
	if ((*c_env)[count])
	{
		tmp = ft_strdup(ft_strchr((*c_env)[count], '=') + 1);
		n->dash = 1;
		ret = ft_change_dir_and_pwds(&tmp, c_env, env, n);
		if (c == '-' && !ret)
			ft_putendl_fd(tmp, 1);
		free(tmp);
	}
	else
		return (ft_find_path_and_cd2(c));
	return (0);
}

/*
*** - Aim of the function :
*** - Print usage error and returns the proper int
*/

int		ft_usage_error(char *command, char *btwn, char *after, int i)
{
	ft_putstr_fd(command, 2);
	ft_putstr_fd(btwn, 2);
	ft_putstr_fd(command, 2);
	ft_putendl_fd(after, 2);
	return (i);
}

/*
*** - Aim of the function :
*** - Print usage error and returns the proper int
*/

int		ft_usage_error_env(char *command, char *btwn, char *after, int i)
{
	ft_putstr_fd(command, 2);
	ft_putstr_fd(btwn, 2);
	ft_putendl_fd(after, 2);
	return (i);
}

/*
*** - Aim of the function :
*** - Norm ft_builtin_env function
*/

void	ft_norm_env(char **av, int i, t_norm_env *t, char ***c_env)
{
	if (ft_strchr(av[i], '='))
	{
		ft_builtin_setenv_env_builtin(av[i], &(t->cp_c_env));
		if (!av[i + 1])
			ft_print_env(&(t->cp_c_env));
	}
	else if (av[i] && av[i][0] == '-' && !ft_usage_is_good("i", av[i])
		&& (i > 1 ? (!ft_strcmp(av[i - 1], "env")
			|| av[i - 1][0] == '-') : 1))
		ft_manage_option_i_env(&(t->cp_c_env), *c_env);
	else if (av[i] && av[i][0] == '-' && ft_usage_is_good("i", av[i])
		&& (t->ret = 1))
	{
		ft_usage_error_env("env: illegal option -- ", av[i],
		"\nusage: env [-i] [name=value ...] [utility [argument ...]]",
		1);
	}
	else if (i == t->argc - 1 && !ft_strcmp(av[i], "env"))
		ft_print_env(&(t->cp_c_env));
	else if (ft_strcmp(av[i], "env") && !t->stop)
	{
		t->stop = 1;
		t->ret = ft_builtin_env2(av, t->cp_c_env, i, 0);
	}
}

/*
*** - Aim of the function :
*** - Replace the env with the new string
*** - And we return the new proper str
*** - that we will split afterward
*/

char	*ft_array_char_to_str_replace_env(char **c_env, int avoid,
			char *av)
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
