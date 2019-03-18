/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   sh.h                                             .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/06 01:02:47 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/12 22:03:42 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef HASH_TABLE_H
# define HASH_TABLE_H
// # include "sh.h"
# define HT_PRIME_1 151
# define HT_PRIME_2 163
# define INITIAL_SIZE 50

/*
*** - Struct that represents one element (one Hash) of
*** - the Hash Table
*/

typedef struct      s_hitem
{
    char        *key;
    char        *value;
    int         nb_used;
}                   t_hitem;

/*
*** - Struct that contains the Hash Table
*/

typedef struct      s_htable
{
    int         size;
    int         base_size;
    int         count;
    t_hitem     **items;
}                   t_htable;

/*
*** - prime.c
*/

int				ft_find_next_prime(int nb);
/*
*** - hash.c
*/

void        resize_table_up(t_htable* table);
void        insert_element(t_htable* table, char *key, char *value);

/*
*** - new_element.c
*/

t_hitem     *new_element(char *key, char *value);
t_htable    *initalize_hash_table_size(int size);
t_htable    *new_hash_table();

/*
*** - hash_function.c
*/

int         ft_hash_generic(char *str, int prime, int size_table);
int         ft_get_hash(char *str, int size_table, int attempt);

/*
*** - resize_hash.c
*/

void        resize_table_up(t_htable* table);

/*
*** - insert_element_hash.c
*/

void        insert_element(t_htable* table, char *key, char *value);

#endif
