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
this function replace env var(between quotes) with their values 
ex:
ls $USER $HOME
1. ls 
2. ls canastas $HOME
3. ls canastas /Users/canastas
*/

void	join_text_between_quotes(char **temp,  t_env_tools env, int *i,  char *tmp_var)
{
	char *tmp_var2;
    char *val_env;
    char *val_expended;

	(*i)++;
    val_env = ft_strsub(*temp, *i, size_env_var(*temp, *i));
    val_expended = env_var(env,val_env, size_env_var(*temp, *i));
    tmp_var2 = ft_strjoin(tmp_var, val_expended);
	*i = *i + size_env_var(*temp, *i);
    free(val_env);
	free(val_expended);
    val_expended = ft_strsub(*temp, *i, ft_strlen(*temp) - *i);
    val_env = ft_strjoin(tmp_var2, val_expended);
    free(*temp);
	free(tmp_var2);
    *temp = val_env;
    *i = -1;
}

char	*ft_strdup_without_char_2(char *str, char c, int *j)
{
    char	*dest;
	int		i;

    printf("%c\n", c);
	i = 0;
	*j = 0;
	while (str[++i] && str[i] != c)
			(*j)++;
	if ((dest = (char*)malloc(sizeof(char) * ((*j) + 1))) == NULL)
		return (0);
	i = 0;
	*j = 0;
	while (str[++i] && str[i] != c)
	{
			dest[*j] = str[i];
			(*j)++;
	}
	dest[*j] = '\0';
    (*j)++;
	return (dest);
}

void	treat_value_env(char **temp,  t_env_tools env)
{
    int i;
    char *tmp_var;

    i = -1;
    while(++i < ft_strlen(*temp))
    {

        if (ft_strchr(*temp, '$'))
        {
            tmp_var = ft_strsub(*temp, 0, ft_strlen(*temp) - ft_strlen(ft_strchr(*temp, '$')));
            i = i + ft_strlen(*temp) - ft_strlen(ft_strchr(*temp, '$'));
        }
        if (temp[0][i] == '$')
			join_text_between_quotes(temp, env, &i, tmp_var);
    }
}
int     last_ocurrence_of_char(char *str, char c)
{
    int i;
    int index;
    index = -1;
    i = 0;

    while (i < ft_strlen(str))
    {
        if (str[i] == c)
            index = i;
        i++;   
    }
    return (index + 1);
}
void print_array_2(char **cmd)
{
	int i;

	i = 0;
	while(cmd[i])
	{
		printf("%s\n",cmd[i]);
		i++;
	}
}

int		count_char(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (str)
	{
		while (str[i] != '\0')
		{
			if (str[i] == c)
				count++;
			i++;
		}
	}
	return (count);
}
/*
	exopension treatement
*/

char *treatement_quotes_2(char **str, t_env_tools env)
{
    char *res = NULL;
    int i;
    char *temp;
  
    i = -1;
    char temp2;
    while (++i < size_str(str) - 1)
    {
        if (ft_strchr(str[i], '$') && !ft_strchr(str[i], '\'') &&
        !(ft_strchr(str[i], '$') && ft_strlen(str[i]) == 1))
        {
               treat_value_env(&str[i], env);
        }
        if (ft_strchr(str[i + 1], '$')  && !ft_strchr(str[i], '\'') &&
        !(ft_strchr(str[i + 1], '$') && ft_strlen(str[i + 1]) == 1))
            treat_value_env(&str[i + 1], env);
        if (ft_strchr(str[i], '\'') )
        {
            str[i] = ft_strdup_without_char(str[i], '\'');
        }
        if (ft_strchr(str[i + 1], '\''))
            str[i + 1] = ft_strdup_without_char(str[i + 1], '\'');
        temp = ft_strjoin(str[i],str[i+1]);
        res = ft_strjoin(res, temp);
        free(temp);
        i++;
    }
    if (size_str(str) % 2 != 0)
    {
        if (ft_strchr(str[size_str(str) - 1], '$') && !ft_strchr(str[i], '\'') &&
         !(ft_strchr(str[size_str(str) - 1], '$') &&
         ft_strlen(str[size_str(str) - 1]) == 1))
            treat_value_env(&str[size_str(str) - 1], env);
         if (ft_strchr(str[size_str(str) - 1], '\''))
         {
            str[i] = ft_strdup_without_char(str[size_str(str) - 1], '\'');
          //  temp2 = ft_strchr(str[i],'\'');
            printf("hooo: %s\n", str[i]);
         }
                 res = ft_strjoin(res, str[size_str(str) - 1]);   
    }
    return (res);
}
char **expense_cmd(t_command cmd, t_env_tools env, int i)
{
    int j;
    char **res;
    char *temp;
    char **without_quotes;
 

    res = (char**)malloc(sizeof(char*) * (cmd.command[i].used_space));
    j = -1;
    while (++j < cmd.command[i].used_space)
    {
        temp = ft_strdup(cmd.command[i].cmd_simple[j]); 
        if (ft_strchr(temp,'\"') || ft_strchr(temp,'\'') )
        {
            if (temp[0] == '\"' && temp[ft_strlen(temp) -1] == '\"' &&
            count_char(temp, '\"') == 2)
            {
               temp = ft_strdup_without_char(cmd.command[i].cmd_simple[j], '\"');
               if (ft_strchr(temp,'$'))
                treat_value_env(&temp, env);
            }
            if (temp[0] == '\'' && temp[ft_strlen(temp) -1] == '\'' &&
            count_char(temp, '\'') == 2)
            {
                printf("hei\n");
                temp = ft_strdup_without_char(cmd.command[i].cmd_simple[j], '\'');
            }
            else
            {
                without_quotes = ft_strsplit(temp, '\"');
                print_array_2(without_quotes);
                temp =  treatement_quotes_2(without_quotes, env);
            }
        }
        if (!ft_strchr(cmd.command[i].cmd_simple[j],'\"') &&
        !ft_strchr(cmd.command[i].cmd_simple[j],'\''))
        {
            temp = ft_strdup(cmd.command[i].cmd_simple[j]);   
            if (ft_strchr(temp,'$'))
               treat_value_env(&temp, env);
        }
        res[j] = ft_strdup(temp);
    }
    return (res);
}