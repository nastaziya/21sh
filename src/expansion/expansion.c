#include "../../inc/expansion.h"

/*
expension treatement
*/

		
// echo $ USER / echo $\\$USER
void treat_backslash(char **str, int i, t_dynamic_array *array_without_backslash, t_env_tools env)
{
	int j;
	int size;
	j = 0;
	tab_expansion_init(array_without_backslash);
	size = ft_strlen(str[i]);
	while (j < size)
	{
		printf("%d\n", j);
		if (str[i][j] == '\'')
		{
			j++;
			while(str[i][j] != '\'' && j < size)
			{
				add_char_to_array(array_without_backslash, str[i][j]);
				j++;
			}
			j++;
		}
		else if(str[i][j] == '"')
			j++;
		else if (str[i][j] == '\\') 
		{
			j++;
			add_char_to_array(array_without_backslash, str[i][j]);
			j++;
		}
		/*else if (str[i][j] == '$' && (str[i][j + 1] == 0 || str[i][j + 1] == '\\'))
		{
			printf("hei\n");
			add_char_to_array(array_without_backslash, str[i][j]);
			j++;
		}*/
		else if (str[i][j] == '$')
		{
			treat_value_env(&str[i], env, j);
			size = ft_strlen(str[i]);
		}
		else
		{
			add_char_to_array(array_without_backslash, str[i][j]);
			j++;
		}
			
	}
	
}

void modif_backslash_str(char **str,  t_env_tools env, int i)
{
	t_dynamic_array array_without_backslash0;
	treat_backslash(str, i, &array_without_backslash0, env);
	free(str[i]);
	str[i] = ft_strdup(array_without_backslash0.array_expanded);
}

char **expense_cmd(t_command cmd, t_env_tools env, int i)
{
	int j;
	char **res;
	char *temp;
	char **without_quotes;
	int *tab_index_backslash;
	
	without_quotes = NULL;
	res = (char**)malloc(sizeof(char*) * (cmd.command[i].used_space));
	j = -1;

	while (++j < cmd.command[i].used_space)
	{
		temp = ft_strdup(cmd.command[i].cmd_simple[j]);
		modif_backslash_str(&temp, env, 0);
		res[j] = ft_strdup(temp);
		free(temp);
	}
	return (res);
}