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

/*
*** - Aim of the function : 
*** - Free with return to include inside
*** - an if statement
*/

int		ft_free_char_char(char **av)
{
	free(av);
	return (0);
}

/*
*** - Aim of the function : 
*** - Free with return to include inside
*** - an if statement
*/

int		ft_free(char *av)
{
	free(av);
	return (0);
}

/*
*** - Aim of the function : 
*** - Name not correct as the string is already expansioned
*** - Manages the "setenv OUI" and "setenv OUI=" by creating a copy
*** - that will always be "OUI=""
*/

char	*ft_strdup_without_quotes(char *av)
{
	char	*ret;
	char	*tmp;

	ret = NULL;
	ret = ft_strdup(av);
	if (!ft_strchr(ret, '='))
	{
		tmp = ret;
		ret = ft_strjoin(tmp, "=");
		free(tmp);
	}
	return (ret);
}

/*
*** - Aim of the function : 
*** - We modify the global env. Used in the unsetenv command
*** - We realloc without the env[avoid]; -> we avoid this variable
*** - because I work with a string that I will split afterward
*/

char	*ft_array_char_to_str(char **c_env, int avoid)
{
	char	*ret;
	char	*tmp;
	int		i;

	i = -1;
	ret = NULL;
	tmp = NULL;
	while (c_env[++i])
		if (i != avoid)
			ft_swap_string(&ret, &tmp, &c_env[i]);
	return (ret);
}