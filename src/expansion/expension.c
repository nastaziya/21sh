#include "../../inc/sh.h"

char	*ft_strdup_without_char(char *str, char c)
{
	char	*dest;
	int		i;
	int		j;

	i = -1;
	j = 0;
	while (str[++i])
		if (str[i] != c)
			j++;
	if ((dest = (char*)malloc(sizeof(char) * (j + 1))) == NULL)
		return (0);
	i = -1;
	j = 0;
	while (str[++i])
	{
		if (str[i] != c)
		{
			dest[j] = str[i];
			j++;
		}
	}
	dest[j] = '\0';
	return (dest);
}
/*
char	*ft_strdup_without_quotes(char *av, int len, int boolean)
{
	char	*ret;
	char	*tmp;

	ret = NULL;
	if (ft_strlen(av) == (size_t)len)
		ret = ft_strdup(av);
	else
	{
		if (ft_strchr(av + len, '"'))
			ret = ft_strdup_without_char(av, '"');
		else
			ret = ft_strdup(av);
	}
	if (boolean == 1 && !ft_strchr(ret, '='))
	{
		tmp = ret;
		ret = ft_strjoin(tmp, "=");
		free(tmp);
	}
	return (ret);
}*/

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
    while (tmp[i] != ' ' && i != ft_strlen(tmp))
    {
        size++;
        i++;
    }
    return (size);
}


void treat_value_env(char **temp,  t_env_tools env)
{
    int i;
    char *tmp_var;
    char *tmp_var2;
    char *val_env;
    char *val_expended;

    i = -1;
    while(++i < ft_strlen(*temp))
    {

        if (ft_strchr(*temp, '$'))
        {
            tmp_var = ft_strsub(*temp, 0, ft_strlen(*temp) - ft_strlen(ft_strchr(*temp, '$')));
            i = i + ft_strlen(*temp) - ft_strlen(ft_strchr(*temp, '$'));
        }
        if (temp[0][i] == '$')
        {
            i++;
            val_env = ft_strsub(*temp, i, size_env_var(*temp, i));
            val_expended = env_var(env,val_env, size_env_var(*temp, i));
            tmp_var2 = ft_strjoin(tmp_var, val_expended);
            i = i + size_env_var(*temp, i);
            free(val_env);
            free(val_expended);
            val_expended = ft_strsub(*temp, i, ft_strlen(*temp) - ft_strlen(tmp_var));
            val_env = ft_strjoin(tmp_var2, val_expended);
            free(*temp);
            free(tmp_var2);
            *temp = val_env;
            i = -1;
        }
    }
}

char **expense_cmd(t_command cmd, t_env_tools env, int i)
{
    int j;
    char **res;
    char *temp;

    res = (char**)malloc(sizeof(char*) * (cmd.command[i].used_space));
    j = -1;
    while (++j < cmd.command[i].used_space)
    {
        if (ft_strchr(cmd.command[i].cmd_simple[j],'\"'))
        {
            temp = ft_strdup_without_char(cmd.command[i].cmd_simple[j], '\"');
            if (ft_strchr(temp,'$'))
               treat_value_env(&temp, env);
        }
        else
            temp = ft_strdup(cmd.command[i].cmd_simple[j]);
        res[j] = ft_strdup(temp);
    }
    return (res);
}