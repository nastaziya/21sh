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

int		ft_free(char *av)
{
	free(av);
	return (0);
}

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