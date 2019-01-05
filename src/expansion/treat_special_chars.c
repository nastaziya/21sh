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

void	dquote(char **str, int *j, t_dynamic_array *final_array, t_env_tools env)
{
	char *val_env;
	int size_var_env;
	
	size_var_env = 0;
	while (str[0][*j] != '\"' && *j < ft_strlen(*str))
	{
		if (str[0][*j] == '$')
		{
			//expand var from env and keep size of to increment final_tab index
			size_var_env = environement_var_call(str[0], env, *j, &val_env);
			//add env var to the final array
			add_env_var_to_final_array(final_array, val_env);
			free(val_env);
			//jump to the required index
			(*j) += size_var_env + 1;
		}
		else if (str[0][*j] == '\\') 
		{
			//skip backslash
			(*j)++;
			//add backslash
			add_char_to_array(final_array, str[0][*j]);
			//skip
			(*j)++;
		}
		else
		{
			//add otherwise
			add_char_to_array(final_array, str[0][*j]);
			(*j)++;
		}
	}
}
//sq add all without treating 
void	manage_sq(char **str,  t_dynamic_array *final_array, int *j, int i)
{
	(*j)++;
	while(str[i][*j] != '\'' && *j < ft_strlen(str[i]))
	{
		add_char_to_array(final_array, str[i][*j]);
		(*j)++;
	}
	(*j)++;
}

// void	home_var_expand(char **str, int i, int *j, t_dynamic_array *final_array)//, char *home_cpy
void	home_var_expand(char **str, s_norm_exp	*n, t_dynamic_array *final_array, char *home)//, char *home_cpy
{
	//treat directory ~/
	if (str[n->i][n->j] == '~' && (n->j == 0 && str[n->i][n->j + 1] == '/'))
		add_return_from_env(home, final_array, &n->j, 1);
	//treat ~+ == PWD
	 if (n->j < ft_strlen(str[n->i]) && str[n->i][n->j] == '~' && (str[n->i][n->j + 1] == '+'
	|| str[n->i][n->j + 1] == '0'))
	{
		add_return_from_env(getenv("PWD"), final_array, &n->j, 1);
		(n->j)++;
	}
	else if (str[n->i][n->j] == '~' && n->j == 0 && (str[n->i][n->j + 1] == ' '
	|| str[n->i][n->j + 1] == '\0'))
	{
		//// Ici, j'expansionne sur la copie de l'env HOME
		add_return_from_env(home, final_array, &n->j, 1);
	}
	else
	{
		add_char_to_array(final_array, str[0][n->j]);
		(n->j)++;
	}
}

// void	home_var_expand(char **str, int i, int *j, t_dynamic_array *final_array)//, char *home_cpy
// {
// 	//treat directory ~/
// 	if (str[i][*j] == '~' && (*j == 0 && str[i][*j + 1] == '/'))
// 		add_return_from_env(getenv("HOME"), final_array, j, 1);
// 	//treat ~+ == PWD
// 	 if (*j < ft_strlen(str[i]) && str[i][*j] == '~' && (str[i][*j + 1] == '+'
// 	|| str[i][*j + 1] == '0'))
// 	{
// 		add_return_from_env(getenv("PWD"), final_array, j, 1);
// 		(*j)++;
// 	}
// 	else if (str[i][*j] == '~' && *j == 0 && (str[i][*j + 1] == ' '
// 	|| str[i][*j + 1] == '\0'))
// 	{
// 		//// Ici, j'expansionne sur la copie de l'env HOME
// 		add_return_from_env(getenv("HOME"), final_array, j, 1);
// 	}
// 	else
// 	{
// 		add_char_to_array(final_array, str[0][*j]);
// 		(*j)++;
// 	}
// }

void dollar_expand(char **str, int *j, t_dynamic_array *final_array, t_env_tools env)
{
	int size_var_env;  
	char *val_env;

	size_var_env = 0;
	if (str[0][*j] == '$' && (str[0][*j + 1] == 0 || str[0][*j + 1] == '\\'))
	{
		add_char_to_array(final_array, str[0][*j]);
		(*j)++;
	}
	else if (str[0][*j] == '$' && str[0][*j + 1] == '?')
		add_return_from_env(ft_itoa(env.g_return_value), final_array, j, 2);
	else if (str[0][*j] == '$')
	{
		size_var_env = environement_var_call(str[0], env, *j, &val_env);
		add_env_var_to_final_array(final_array, val_env);
		free(val_env);
		*j += size_var_env + 1;
	}
}

void treat_backslash(char **str, int i, int *j, t_dynamic_array *final_array)
{
	(*j)++;
	add_char_to_array(final_array, str[i][*j]);
	(*j)++;
}