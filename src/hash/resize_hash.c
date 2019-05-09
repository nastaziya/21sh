/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/03/22 14:34:47 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include "../../inc/hash_table.h"

void		resize_table(t_htable *table, int base_size)
{
	int			i;
	t_htable	*new;
	t_hitem		*item;
	int			tmp_size;
	t_hitem		**tmp_items;

	i = -1;
	if (base_size < INITIAL_SIZE)
		return ;
	new = initalize_hash_table_size(base_size);
	while (++i < table->size && (item = table->items[i]))
		if (item != NULL)
			insert_element(new, item->key, item->value);
	table->base_size = new->base_size;
	table->count = new->count;
	tmp_size = table->size;
	table->size = new->size;
	new->size = tmp_size;
	tmp_items = table->items;
	table->items = new->items;
	new->items = tmp_items;
	delete_hash_table(&new);
}

void		resize_table_up(t_htable *table)
{
	int		new_size;

	new_size = table->base_size * 2;
	resize_table(table, new_size);
}
