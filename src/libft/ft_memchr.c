/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_memchr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:17 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/19 18:02:17 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*cast_src;

	i = 0;
	cast_src = (unsigned char *)s;
	while (i < n)
	{
		if (cast_src[i] == (unsigned char)c)
			return (cast_src + i);
		i++;
	}
	return (NULL);
}
