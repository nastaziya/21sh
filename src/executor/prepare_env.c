/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/24 16:37:34 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

int		size_str(char **environ)
{
	int	i;
	int j;

	i = 0;
	j = 4;
	while (environ[i] != NULL)
		i++;
	return (i);
}

void	free_str(char **str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		free(str[i]);
		i++;
	}
	free(str);
	str = NULL;
}

void	free_str_2(char **str, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (str[i] != NULL)
			free(str[i]);
		i++;
	}
	free(str);
	str = NULL;
}

/*
*** - First, copies the env into the appropriate struct
*** - We just avoid the OLDPWD env in order to copy the
*** - behavior of the bash (not set at the beginning)
*** - This is the reason why we malloc of size
*** - And put the the at the size - 1 position
*** - I also added the Initial copy of the home environment
*** - Bash works with copies (if you unset the HOME),
*** - Because of the copy, the expansion will still work
*** - => same management as the PATH (works with copies)
*** - Added later : clicolor=1 -> to have a nice "ls"
*/

char	**copy_env(char **environ, int size, t_env_tools *envi)
{
	char	**env;
	int		i;
	int		j;

	i = -1;
	j = 0;
	size = !getenv("PATH") ? size + 1 : size;
	dprintf(3, "size: [%d]\n", size);
	if (!(env = (char**)malloc(sizeof(*env) * (size + 1))))
		return (NULL);
	env[size] = NULL;
	env[size - 1] = ft_strdup("CLICOLOR=1");
	j = size == 2 ? 1 : 0;
	while (environ[++i] != NULL)
	{
		if (ft_strncmp(environ[i], "OLDPWD=", 7))
		{
			env[j] = ft_strdup(environ[i]);
			j++;
		}
	}
	env[0] = getenv("PATH") ? env[0] : get_envpath_from_file();
	envi->home = getenv("HOME") ? ft_strdup(getenv("HOME"))
		: ft_strdup("/Users/gurival-");
	return (env);
}
