/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/07/22 04:00:43 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/07/22 06:06:32 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"

void		ft_print_env(char ***c_env)
{
	int	i;

	i = -1;
	while (c_env[0][++i])
	{
		ft_putstr(c_env[0][i]);
		ft_putchar('\n');
	}
}

int		ft_free_av(char **av)
{
	int	i;

	i = -1;
	if (av)
		while (av[++i])
			free(av[i]);
	free(av);
	return (0);
}

void	ft_exchange_chars(char **av, char delimitor, char to_replace)
{
	int		i;
	int		j;
	char	delimitor2;
	char	to_replace2;

	i = -1;
	delimitor2 = (delimitor == ' ' ? '\t' : (char)254);
	to_replace2 = (delimitor == ' ' ? (char)254 : '\t');
	while (av[++i])
	{
		j = -1;
		while (av[i][++j])
		{
			if (av[i][j] == delimitor)
				av[i][j] = to_replace;
			else if (av[i][j] == delimitor2)
				av[i][j] = to_replace2;
		}
	}
}

int			ft_str_is_alpha_setenv(char *str)
{
	int		i;
	int		len;

	i = -1;
	len = (ft_strchr(str, '=') ? ft_strchr(str, '=') - str : ft_strlen(str));
	while (str[++i] && i < len)
	{
		if (str[i] != '_' && str[i] != '=')
			if (!ft_isalnum(str[i]))
				return (1);
	}
	return (0);
}

void	ft_swap_string(char **ret, char **tmp, char **c_env)
{
	*tmp = *ret;
	*ret = ft_strjoin(*tmp, *c_env);
	free(*tmp);
	*tmp = *ret;
	*ret = ft_strjoin(*tmp, " ");
	free(*tmp);
}
