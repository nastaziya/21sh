/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_memcmp.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:17 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/19 18:02:17 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_dlist	*ft_dlstnew(void const *content)
{
	t_dlist *list;

	if (!(list = (t_dlist *)malloc(sizeof(t_dlist))))
		return (NULL);
	if (!content)
		list->content = NULL;
	else
        list->content = ft_strdup(content);
	list->next = NULL;
    list->prev = NULL;
	return (list);
}
