/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_atoi.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/17 18:56:52 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/17 18:56:58 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	ft_dlstadd(t_dlist **alst, t_dlist *new)
{
	if (alst != NULL)
	{
		new->next = *alst;
        (*alst)->prev = new;
		*alst = new;
		new->prev = NULL;
	}
}
