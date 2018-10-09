#include "../../inc/sh.h"
/*take take what it is after env var = */ 
char	*env_var(t_env_tools env, char *str, int size)
{
	int		i;
	char	*ret_var;
	int		check_var;
	int		size_env;

	i = 0;
	check_var = 0;
	while (env.env_cpy[i])
	{
		size_env = (int)(ft_strchr(env.env_cpy[i], '=') - env.env_cpy[i]);
		if (!ft_strncmp(env.env_cpy[i], str, size_env) && size == size_env)
		{
			ret_var = ft_strsub(ft_strchr(env.env_cpy[i], '='), 1,
					ft_strlen(ft_strchr(env.env_cpy[i], '=')) - 1);
			check_var = 1;
		}
		i++;
	}
	if (check_var == 0)
		ret_var = NULL;
	return (ret_var);
}

int size_env_var(char *tmp, int i)
{
    int size;

    size = 0;
    while (tmp[i] != ' ' && tmp[i] != '\\' && tmp[i] != '\''  && tmp[i] != '"' && i != ft_strlen(tmp))
    {
        size++;
        i++;
    }
    return (size);
}