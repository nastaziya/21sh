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

void        delete_hash_table(t_htable** table)
{
	int         i;
	
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
		if (ft_strcmp(item->key, key) == 0)
			return (item->value);
		index = ft_get_hash(key, table->size, i);
		item = table->items[index];
		i++;
	}
	return (NULL);
}

int         count_size_number(int nb)
{
	int i;

	i = 1;
	while (nb /=10)
		i++;
	return (i);
}

void        ft_print_hash(t_env_tools *env)
{
	int     i;
	int     size;

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

int			ft_builtin_hash(char **cmd, t_env_tools *env)
{
	int			i;
	int         len;
	int         boul;
	char        *path;

	i = 1;
	path = NULL;
	len = ft_len_array_char(cmd);
	boul = NO_RESET;
	if (len == 1)
		ft_print_hash(env);
	else
	{
		while (cmd[i] && cmd[i][0] == '-')
		{
			dprintf(3, "je passe\n");
			if (ft_usage_is_good("r", cmd[i]))
			{
				dprintf(1, "bash: hash: %s: invalid option\n", cmd[i]);
				dprintf(1, "hash: usage: hash [-r] [name ...]\n");
				return (2);
			}
			boul = TO_RESET;
			i++;
		}
		if (--i && boul == TO_RESET && env->t) //  && env->t
			delete_hash_table(&(env->t));
		while (cmd[++i])
		{
			if (ft_strchr(cmd[i], '/'))
				;
			else
			{
				if (env->paths)
				{
					check_path(env->paths, &path, &(cmd[i]));
					if (!path)
					{
						dprintf(1, "%s", "bash: hash:");
						dprintf(1, "%s: not found\n", cmd[i]);
					}
					else
					{
						if (!env->t)
							env->t = new_hash_table();
						insert_element(env->t, cmd[i], path);
					}
					// strjoin le path
					// si binaire existe dans le path
					// l'ajouter
					// sinon, error
					// et rajouter lors de l'exécution
					// le search, et l'incrémentation
				}
			}
			if (path != NULL)
				free(path);
			path = NULL;
		}
	}
	
	// (void)cmd;
	// (void)env;
	// if (!env->t)
	//     env->t = new_hash_table();
	// insert_element(env->t, "ls", "/usr/bin/ls");
	// insert_element(env->t, "ls", "/usr/bin/ls");
	// insert_element(env->t, "lsi", "/usr/bin/ls1");
	// insert_element(env->t, "lsi", "/usr/bin/ls1");
	// ft_print_hash(env);
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
	// dprintf(1, "hash_i: %d\n", ft_get_hash("/usr/bin/vim/lld/ooooo/eeee/zzzz/e", 53, 0));
	// dprintf(1, "hash_LOL: %d\n", ht_hash("/usr/bin/vim", 53, 0));
	// delete_hash_table(&(env->t));
	// env->t = NULL;
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