/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/03/22 14:20:42 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include "../../inc/hash_table.h"
#include "stdio.h"

/*
*** - Aim of the function :
*** - Delete a previously allocated Hashed element
*/

void		delete_element(t_hitem *el)
{
	free(el->key);
	free(el->value);
	free(el);
	el = NULL;
}

/*
*** - Aim of the function :
*** - Delete the Hash Table
*/

void		delete_hash_table(t_htable **table)
{
	int	i;

	i = 0;
	while (++i < (*table)->size)
	{
		if ((*table)->items[i] != NULL)
			delete_element((*table)->items[i]);
	}
	free((*table)->items);
	free((*table));
	(*table) = NULL;
}

/*
*** - Aim of the function :
*** - Search and retrieve the value for the key
*/

char		*search_element(t_htable *table, char *key)
{
	int			index;
	t_hitem		*item;
	int			i;

	index = ft_get_hash(key, table->size, 0);
	item = table->items[index];
	i = 1;
	while (item != NULL)
	{
		if (ft_strcmp(item->key, key) == 0 && item->nb_used++)
			return (item->value);
		index = ft_get_hash(key, table->size, i);
		item = table->items[index];
		i++;
	}
	return (NULL);
}

/*
*** - Aim of the function :
*** - count size of nb
*/

int			count_size_number(int nb)
{
	int		i;

	i = 1;
	while (nb /= 10)
		i++;
	return (i);
}

/*
*** - Aim of the function :
*** - Print hash when "hash" command is used
*/

void		ft_print_hash(t_env_tools *env)
{
	int		i;
	int		size;

	i = -1;
	if (!env->t)
		ft_putendl_fd("hash: hash table empty", 1);
	else
	{
		ft_putendl_fd("hits	command", 1);
		while (++i < env->t->size)
		{
			if (env->t->items[i] != NULL)
			{
				size = count_size_number(env->t->items[i]->nb_used);
				while (size++ < 4)
					ft_putchar_fd(' ', 1);
				dprintf(1, "%d	", env->t->items[i]->nb_used);
				ft_putendl_fd(env->t->items[i]->value, 1);
			}
		}
	}
}
