/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/03/22 14:32:57 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include "../../inc/hash_table.h"

/*
*** - Aim of the function :
*** - Inialise a new Hashed element
*/

t_hitem		*new_element(char *key, char *value)
{
	t_hitem	*item;

	if (!(item = malloc(sizeof(t_hitem))))
		return (NULL);
	item->key = ft_strdup(key);
	item->value = ft_strdup(value);
	item->nb_used = 0;
	return (item);
}

/*
*** - Aim of the function :
*** - Inialise an Hash Table
*/

t_htable	*initalize_hash_table_size(int size)
{
	t_htable	*table;
	int			i;

	i = -1;
	if (!(table = malloc(sizeof(t_htable))))
		return (NULL);
	table->base_size = size;
	table->size = ft_find_next_prime(table->base_size);
	table->count = 0;
	if (!(table->items = malloc(table->size * sizeof(t_hitem*))))
		return (NULL);
	while (++i < table->size)
		table->items[i] = 0;
	return (table);
}

t_htable	*new_hash_table(void)
{
	return (initalize_hash_table_size(INITIAL_SIZE));
}
