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
char	*ft_strdup_without_char_3(char *str, char c)
{
    char	*dest;
	int		i;
    int j;

	i = 0;
	j = 0;
	while (str[++i] && str[i] != c)
			j++;
	if ((dest = (char*)malloc(sizeof(char) * (j + 1))) == NULL)
		return (0);
	i = 0;
	j = 0;
	while (str[++i] && str[i] != c)
	{
			dest[j] = str[i];
			j++;
	}
	dest[j] = '\0';
    j++;
	return (dest);
}

/*
char	*ft_strdup_without_char_3(char *str, char c,  int i)
{
    char	*dest;
	int		i;

	i = i - 1;
	j = 0;
	while (str[++i] && str[i] != c)
			j++;
	if ((dest = (char*)malloc(sizeof(char) * (j + 1))) == NULL)
		return (0);
	i = i - 1;
	j = 0;
	while (str[++i] && str[i] != c)
	{
			dest[*j] = str[i];
			j++;
	}
	dest[j] = '\0';
   // (*j)++;
	return (dest);
}*/
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

/*
	exopension treatement
*/
char **expense_cmd(t_command cmd, t_env_tools env, int i)
{
    int j;
    char **res;
    char *temp;
    int index;
    char   *temp2;
    char *test;
    int last_quote;
    char *test2;

    index = 0;
    res = (char**)malloc(sizeof(char*) * (cmd.command[i].used_space));
    j = -1;
    while (++j < cmd.command[i].used_space)
    {
        temp = ft_strdup(cmd.command[i].cmd_simple[j]); 
        last_quote = last_ocurrence_of_char(temp, '\"');
        if (last_quote != -1)
            test2 = ft_strsub(temp, last_quote, ft_strlen(temp) - last_quote);
        while (ft_strchr(temp,'\"'))
        {
            printf("temp : %s\n", temp);
            if (temp[0] == '\"' && temp[ft_strlen(temp)] == '\"')
            {
                temp2 = ft_strdup_without_char(temp, '\"');
                if (ft_strchr(temp,'$'))
                    treat_value_env(&temp2, env);
                temp = ft_strdup(temp2);
            }
            else
            {
                temp2 = ft_strdup_without_char_3(ft_strchr(temp,'\"'), '\"');
                if (ft_strchr(temp2,'$'))
                    treat_value_env(&temp2, env);
                if ((int)(ft_strchr(temp,'\"') - temp) == 0)
                {
                    temp++;
                    test =  ft_strchr(temp,'\"') + 1;
                    //free(temp);
                    temp = ft_strjoin(temp2,test);
                    // free(test);
                //free(temp2);
                }
                else
                {
                    test = ft_strsub(temp, 0 , (int)(ft_strchr(temp,'\"') - temp));
                  //  free(temp);
                    temp = ft_strjoin(test, temp2);
                    printf("temp2 : %s\n", temp2);
                    if (test2 != NULL)
                    {
                            temp = ft_strjoin(temp, test2);
                    }
                    free(test);                
                    free(temp2);
                    }
            }
        }
        if (!ft_strchr(cmd.command[i].cmd_simple[j],'\"'))
        {
            temp = ft_strdup(cmd.command[i].cmd_simple[j]);   
            if (ft_strchr(temp,'$'))
               treat_value_env(&temp, env);
        }
        res[j] = ft_strdup(temp);
    }
    return (res);
}