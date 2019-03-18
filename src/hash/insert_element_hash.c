/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/24 16:34:15 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include "../../inc/hash_table.h"

void        insert_element(t_htable* table, char *key, char *value)
{
    t_hitem     *item;
    t_hitem     *cur_item;
    int         index;
    int         i;
    int         load;
    
    load = table->count * 100 / table->size;
    if (load > 70)
        resize_table_up(table);
    item = new_element(key, value);
    index = ft_get_hash(item->key, table->size, 0);
    cur_item = table->items[index];
    i = 1;
    while (cur_item != NULL)
    {
        if (!ft_strcmp(cur_item->key, key))
        {
            delete_element(cur_item);
            table->items[index] = item;
            return ;
        }
        index = ft_get_hash(item->key, table->size, i);
        cur_item = table->items[index];
        i++;
    } 
    table->items[index] = item;
    table->count++;
}