/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 20:16:48 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/expansion.h"

void		tab_expansion_init(t_dynamic_array *arr)
{
	arr->used_size = 0;
	arr->capacity = TAB_INITIAL_CAPACITY;
	if (!(arr->array_expanded = (char*)malloc(sizeof(char)
		* (arr->capacity + 1))))
		return ;
	arr->array_expanded[arr->used_size] = '\0';
}

int			add_char_to_array(t_dynamic_array *arr, char item)
{
	char			*temp;
	int				i;

	i = -1;
	if (arr->used_size == arr->capacity)
	{
		temp = arr->array_expanded;
		arr->capacity = (arr->used_size * 3) / 2 + 1;
		if (!(arr->array_expanded = malloc(sizeof(char) * arr->capacity + 1)))
			return (1);
		if (arr->array_expanded == NULL)
			exit(EXIT_FAILURE);
		while (++i < arr->used_size)
			arr->array_expanded[i] = temp[i];
		free(temp);
	}
	arr->array_expanded[arr->used_size] = item;
	++arr->used_size;
	arr->array_expanded[arr->used_size] = '\0';
	return (0);
}

/*
*** - expension treatement
*** - echo $ USER / echo $\\$USER
*** - this function add the return of bin commands/builtins
*** - and add to final expanded array
*/

void		add_return_from_env(char *ret_nr,
				t_dynamic_array *array_without_backslash, int *j, int add)
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
