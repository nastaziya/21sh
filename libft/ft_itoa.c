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
#include <stdio.h>

char			*ft_itoa(int n)
{
	char	*str;
	long	long_n;
	char	tab[15];
	int		i;
	int		j;

	long_n = (n < 0 ? -(long)n : (long)n);
	i = 0;
	tab[i++] = '0' + (long_n % 10);
	while (long_n /= 10)
		tab[i++] = '0' + (long_n % 10);
	i = (n < 0 ? i + 1 : i);
	if (!(str = ft_strnew(i)))
		return (NULL);
	j = (n < 0 ? 1 : 0);
	i = (n < 0 ? i - 1 : i);
	while (i > 0)
		str[j++] = tab[--i];
	str[0] = (n < 0 ? '-' : str[0]);
	return (str);
}
