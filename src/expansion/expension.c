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
   expension treatement
   */

int *char_position(char *str, char c)
{
	char * pch;
	int i;
	int *tab_index;
	int j;
	tab_index  = malloc(sizeof(int) * count_char(str, c));
	i = 0;
	j = 0;
	while (str[j] != '\0')
	{
		if (str[j] == '\'')
		{
			if (i < count_char(str,'\''))
			{
				tab_index[i] = j;
				i++;
			}
		}
		j++;
	}
	return (tab_index);
}

char    *cut_str_between_chars(char *str, int *tab_index, char c,t_env_tools env)
{
	int i;
	int j;
	char *new_str;
	char *res;
	char *tmp_squotes;

	i = 0;
	j = 0;
	new_str = NULL;
	res = NULL;
	tmp_squotes = NULL;
	while(str[i] != '\0')
	{
		if (str[i] != '\'')
		{
			if(j == count_char(str, '\''))
			{
				new_str = ft_strsub(str, i, ft_strlen(str) - i);
				i = ft_strlen(str) -1;
				if (ft_strchr(str, '$')  &&
						!(ft_strchr(new_str, '$') && ft_strlen(new_str) == 1))
					treat_value_env(&new_str, env);
			}
			else
			{
				//'lolo'$USER'lolo" test fail loloanastasialolo

				if ( tab_index[j] - i >= 0 && tab_index[j]  - i <= ft_strlen(str))
				{
						printf("size : %d\n", tab_index[j] - i);
						new_str = ft_strsub(str, i, tab_index[j] - i);
						i = tab_index[j];
						if (ft_strchr(str, '$')  &&
						!(ft_strchr(new_str, '$') && ft_strlen(new_str) == 1))
						treat_value_env(&new_str, env);
				}
			}
		}
		if((j + 1 < count_char(str, '\'')) && i == tab_index[j])
		{
			i++;
			tmp_squotes = ft_strsub(str,i,tab_index[j + 1] - i);
			i = tab_index[j + 1];
			j = j + 2;
		}
		i++;
		new_str = ft_strjoin(new_str, tmp_squotes);
		res = ft_strjoin(res, new_str);
		if (tmp_squotes != NULL)
			ft_strdel(&tmp_squotes);
		if (new_str != NULL)
			ft_strdel(&new_str);
	}
	return (res);
}

char *treatement_quotes_2(char **str, t_env_tools env)
{
	char *res = NULL;
	int i;
	char *temp;
	int *tab_index;

	i = -1;
	char temp2;
	while (++i < size_str(str) - 1)
	{
		if (ft_strchr(str[i], '$') && !ft_strchr(str[i], '\'') &&
				!(ft_strchr(str[i], '$') && ft_strlen(str[i]) == 1))
		{
			             treat_value_env(&str[i], env);
		}
		if (ft_strchr(str[i + 1], '$')  && !ft_strchr(str[i + 1], '\'') &&
				!(ft_strchr(str[i + 1], '$') && ft_strlen(str[i + 1]) == 1))
		{
			treat_value_env(&str[i + 1], env);
		}
		if (ft_strchr(str[i], '\'') )
		{
			tab_index = char_position(str[i], '\'');
			str[i] = cut_str_between_chars(str[i], tab_index, '\'', env);
		}
		if (ft_strchr(str[i + 1], '\''))
		{
			tab_index = char_position(str[i + 1], '\'');
			str[i + 1] = cut_str_between_chars(str[i + 1], tab_index, '\'', env);
		}
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
			tab_index = char_position(str[i], '\'');
			temp = cut_str_between_chars(str[i], tab_index, '\'', env);
			res = ft_strjoin(res, temp);
		}
		else
			res = ft_strjoin(res, str[size_str(str) - 1]);
	}
	return (res);
}

int		check_consec(char *str, char c1, char c2)
{
	int i;

	i = 1;
	while (i < ft_strlen(str))
	{
		if (str[i] == c1 && str[i - 1] == c2)
			return (1);
		i++;
	}
	return (0);
}

char **expense_cmd(t_command cmd, t_env_tools env, int i)
{
	int j;
	char **res;
	char *temp;
	char **without_quotes;
	char *chr;

	without_quotes = NULL;
	res = (char**)malloc(sizeof(char*) * (cmd.command[i].used_space));
	j = -1;
	while (++j < cmd.command[i].used_space)
	{
		temp = ft_strdup(cmd.command[i].cmd_simple[j]); 
			chr = ft_strchr(temp, '\\');
		if (ft_strchr(temp,'\"') || ft_strchr(temp,'\'') )
		{
			if (temp[0] == '\"' && temp[ft_strlen(temp) -1] == '\"' &&
					count_char(temp, '\"') == 2)
			{
				temp = ft_strdup_without_char(cmd.command[i].cmd_simple[j], '\"');
				if (ft_strchr(temp,'$'))
					treat_value_env(&temp, env);
			}
			else if (temp[0] == '\'' && temp[ft_strlen(temp) -1] == '\'' &&
					count_char(temp, '\'') == 2)
			{
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
		printf("temp %s\n", temp);
		res[j] = ft_strdup(temp);
		//ft_strdel(&temp);
		free(temp);
	}
	if (without_quotes != NULL)
		free_str(without_quotes);
	return (res);
}
