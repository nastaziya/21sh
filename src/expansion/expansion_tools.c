#include "../../inc/expansion.h"

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


void	treat_value_env(char **temp,  t_env_tools env, int i)
{
	char *tmp_var;

	if (ft_strchr(*temp, '$'))
	{
		tmp_var = ft_strsub(*temp, 0, ft_strlen(*temp) - ft_strlen(ft_strchr(*temp, '$')));
	}
	if (temp[0][i] == '$')
		join_text_between_quotes(temp, env, &i, tmp_var);
}
