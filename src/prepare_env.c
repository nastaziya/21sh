#include "../inc/sh.h"

int		size_str(char **environ)
{
	int	i;

	i = 0;
	while (environ[i] != NULL)
		i++;
	return (i);
}

void	free_str(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	str = NULL;
}

char	**copy_env(char **environ, int size)
{
	char	**env;
	int		i;

	i = 0;
	if (!(env = (char**)malloc(sizeof(*env) * (size + 1))))
		return (NULL);
	env[size] = NULL;
	while (environ[i] != NULL)
	{
		env[i] = ft_strdup(environ[i]);
		i++;
	}
	return (env);
}
