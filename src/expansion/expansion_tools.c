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
   this function replace env var(between quotes) with their values 
ex:
ls $USER $HOME
1. ls 
2. ls canastas $HOME
3. ls canastas /Users/canastas
*/

int		recover_env_var(char *temp,  t_env_tools env, int *i,  char **val_expended)
{
	char *val_env;
	int size;

	size = 0;
	(*i)++;
	val_env = ft_strsub(temp, *i, size_env_var(temp, *i));
	*val_expended = env_var(env,val_env, size_env_var(temp, *i));
	if (val_expended != NULL)
		size = ft_strlen(val_env);
	free(val_env);
	return (size);
}


int		environement_var_call(char *temp,  t_env_tools env, int i, char **val_exp)
{
	// char *tmp_var;
	int size = 0;
	if (temp[i] == '$')
		size  = recover_env_var(temp, env, &i, val_exp);
	return size;
}
