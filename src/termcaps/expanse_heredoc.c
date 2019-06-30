/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 22:39:06 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/expansion.h"

void	dquote_heredoc(char **str, int *j, t_dynamic_array *final_array)
{
	int	size_var_env;

	size_var_env = 0;
	while (str[0][*j] != '\"' && *j < ft_strlen(*str))
	{
		if (str[0][*j] == '\\')
		{
			(*j)++;
			add_char_to_array(final_array, str[0][*j]);
			(*j)++;
		}
		else
		{
			add_char_to_array(final_array, str[0][*j]);
			(*j)++;
		}
	}
}

void	treat_expansion_cases_heredoc(char **str, int i,
			t_dynamic_array *final_array)
{
	t_norm_exp	n;

	n.j = 0;
	n.i = i;
	tab_expansion_init(final_array);
	while (n.j < ft_strlen(str[n.i]))
	{
		if (str[n.i][n.j] == '\'')
			manage_sq(str, final_array, &n.j, n.i);
		else if (str[n.i][n.j] == '\"' && ++n.j)
		{
			dquote_heredoc(&str[n.i], &n.j, final_array);
			n.j++;
		}
		else if (str[n.i][n.j] == '\\')
			treat_backslash(str, n.i, &n.j, final_array);
		else
		{
			add_char_to_array(final_array, str[n.i][n.j]);
			n.j++;
		}
	}
}

void	expanded_dynamic_table_heredoc(char **str, int i)
{
	t_dynamic_array array_without_backslash0;

	treat_expansion_cases_heredoc(str, i, &array_without_backslash0);
	free(str[i]);
	str[i] = ft_strdup(array_without_backslash0.array_expanded);
	free(array_without_backslash0.array_expanded);
}

int		ft_clean(void *s, size_t n)
{
	ft_bzero(s, n);
	return (0);
}
