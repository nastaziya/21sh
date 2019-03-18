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

// FOR DEBUG
// #include <math.h>
# include "stdio.h"

/*
*** - Aim of the function :
*** - Delete a previously allocated Hashed element
*/

void        delete_element(t_hitem* el)
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

void        delete_hash_table(t_htable* table)
{
    int         i;
    
    i = 0;
    while (++i < table->size)
    {
        if (table->items[i] != NULL)
            delete_element(table->items[i]);
    }
    free(table->items);
    free(table);
    table = NULL;
}

char        *search_element(t_htable* table, char* key)
{
    int         index;
    t_hitem     *item;
    int         i;

    index = ft_get_hash(key, table->size, 0);
    item = table->items[index];
    i = 1;
    while (item != NULL)
    {
        if (strcmp(item->key, key) == 0)
            return (item->value);
        index = ft_get_hash(key, table->size, i);
        item = table->items[index];
        i++;
    }
    return (NULL);
}

int         ft_builtin_hash(char **cmd, t_env_tools *env)
{
    int			i;
    int         len;

    i = -1;
    len = ft_len_array_char(cmd);
    if (len == 1)
        ft_print_hash(env->t);
    else
    {
        
    }
    
    // (void)cmd;
    (void)env;
    // env->t = new_hash_table();
    // insert_element(env->t, "ls", "/usr/bin/ls");
    // insert_element(env->t, "lsi", "/usr/bin/ls1");
    // insert_element(env->t, "lso", "/usr/bin/ls2");
    // insert_element(env->t, "lsa", "/usr/bin/ls3");
    // insert_element(env->t, "lse", "/usr/bin/ls4");
    // insert_element(env->t, "lsp", "/usr/bin/ls5");
    // insert_element(env->t, "lsuu", "/usr/bin/ls6");
    // insert_element(env->t, "lsu33", "/usr/bin/ls7");
    // insert_element(env->t, "lsu3355", "/usr/bin/ls8");
    // dprintf(1, "retrieve: |%s|\n", search_element(env->t, "ls"));
    // dprintf(1, "retrieve: |%s|\n", search_element(env->t, "lsi"));
    // dprintf(1, "retrieve: |%s|\n", search_element(env->t, "lso"));
    // dprintf(1, "retrieve: |%s|\n", search_element(env->t, "lsa"));
    // dprintf(1, "retrieve: |%s|\n", search_element(env->t, "lse"));
    // dprintf(1, "retrieve: |%s|\n", search_element(env->t, "lsp"));
    // dprintf(1, "retrieve: |%s|\n", search_element(env->t, "lsuu"));
    // dprintf(1, "retrieve: |%s|\n", search_element(env->t, "lsu33"));
    // dprintf(1, "retrieve: |%s|\n", search_element(env->t, "lsu3355"));
    // dprintf(1, "retrieve: |%s|\n", search_element(env->t, "ooooooo"));
    // dprintf(1, "hash_i: %d\n", ft_get_hash("/usr/bin/vim", 53, 0));
    // dprintf(1, "hash_LOL: %d\n", ht_hash("/usr/bin/vim", 53, 0));
    // delete_hash_table(env->t);
    return (0);
}

// A FAIRE -> MODIFIER


// si changement de PATH, remettre à 0
// si "-r" , remettre à 0
// si exécution de commande, utiliser fonction search element
// + incrémenter le compteur, sinon si ne fait pas partie de la table, 
// faire exécution normale avec le PATH TROUVE

// si hash -> afficher les infos, si NULL => "hash: hash table empty"
// hash + nom d'une commande == j'ajoute si je trouve
// hash ls pwd tee, boucle et lance jusqu'à la fin, après les "-" sinon erreur
// 