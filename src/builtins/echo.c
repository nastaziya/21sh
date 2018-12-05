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

static void	ft_builtin_echo2(char **av, int i)
{
		ft_putstr_fd(av[i], 1);
	if (av[i + 1])
		ft_putstr_fd(" ", 1);
}

/*
*** - Aim of the function :
*** - Copy the behavior of the echo builtin
*** - Manage the option -n
*/

int			ft_builtin_echo(char **av, char ***c_env)
{
	int		i;
	int		len;

	len = ft_len_array_char(av);
	if (len == 1)
		ft_putchar('\n');
	else if (len > 1)
	{
		i = (!ft_strcmp(av[1], "-n") ? 1 : 0);
		while (av[++i])
			ft_builtin_echo2(av, i);
		if (ft_strcmp(av[1], "-n"))
			ft_putstr_fd("\n", 1);
	}
	return (0);
}