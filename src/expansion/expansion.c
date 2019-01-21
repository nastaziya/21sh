/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/09/07 17:58:18 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/expansion.h"

/*
expension treatement
*/

		
// echo $ USER / echo $\\$USER
//this function add the return of bin commands/builtins and add to final expanded array
void	add_return_from_env(char *ret_nr, t_dynamic_array *array_without_backslash,
			int *j, int add)
{
	int i;

	i = 0;
	while (i < ft_strlen(ret_nr))
	{
		add_char_to_array(array_without_backslash, ret_nr[i]);
		i++;
	}
	*j = *j + add;
}

//add char from an expended array
void  add_env_var_to_final_array(t_dynamic_array *array_without_backslash,
char *str)
{
	int i;

	i = 0;
	if (str != NULL)
	{
		while (i < ft_strlen(str))
		{
			add_char_to_array(array_without_backslash, str[i]);
			i++;
		}
	}
	
}

void	treat_expansion_cases(char **str, int i, t_dynamic_array *final_array, t_env_tools env)
{
	s_norm_exp	n;

	n.j = 0;
	n.i = i;
	tab_expansion_init(final_array);
	while (n.j < ft_strlen(str[n.i]))
	{
		if (str[n.i][n.j] == '\'')
			manage_sq(str, final_array, &n.j, n.i);
		else if (str[n.i][n.j] == '\"' && ++n.j)
		{
			dquote(&str[n.i], &n.j, final_array, env);
			n.j++;
		}
		else if (str[n.i][n.j] == '~') //envoyer la copie du home
			home_var_expand(str, &n, final_array, env.home);
		else if (str[n.i][n.j] == '\\') 
			treat_backslash(str, n.i, &n.j, final_array);
		else if (str[n.i][n.j] == '$')
			dollar_expand(str, &n.j, final_array, env);	
		else
		{
			add_char_to_array(final_array, str[n.i][n.j]);
			n.j++;
		}	
	}
}

void expanded_dynamic_table(char **str,  t_env_tools env, int i)
{
	t_dynamic_array array_without_backslash0;
	treat_expansion_cases(str, i, &array_without_backslash0, env);
	free(str[i]);
	str[i] = ft_strdup(array_without_backslash0.array_expanded);
	free(array_without_backslash0.array_expanded);
}

char **expense_cmd(t_command cmd, t_env_tools env, int i)
{
	int j;
	char **res;
	char *temp;
	
	if (!(res = (char**)malloc(sizeof(char*) * (cmd.command[i].used_space + 1))))
		return (NULL);
	j = -1;
	temp = NULL;
	res[cmd.command[i].used_space] = NULL;
	while (++j < cmd.command[i].used_space)
	{
		temp = ft_strdup(cmd.command[i].cmd_simple[j]);
		expanded_dynamic_table(&temp, env, 0);
		res[j] = ft_strdup(temp);
		free(temp);
	}
	return (res);
}

char **expense_red_files(t_command cmd, t_env_tools env,  int i)
{
	int j;
	char **res;
	char *temp;
	
	if (!(res = (char**)malloc(sizeof(char*) * (cmd.command[i].redirection.used_space + 1))))
		return (NULL);
	j = -1;
	temp = NULL;
	// res[cmd.command[i].redirection.used_space] = NULL;
	while (++j < cmd.command[i].redirection.used_space)
	{
		temp = ft_strdup(cmd.command[i].redirection.file[j]);
		expanded_dynamic_table(&temp, env, 0);
		res[j] = ft_strdup(temp);
		free(temp);
	}
	res[j] = NULL;
	return (res);
}