/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_isdigit.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:16 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/19 18:02:16 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrev(char *str)
{
	int		start;
	int		end;
	char	tmp;

	start = 0;
	end = 0;
	while (str[end])
		end++;
	end--;
	while (start < end)
	{
		tmp = str[end];
		str[end] = str[start];
		str[start] = tmp;
		end--;
		start++;
	}
	return (str);
}
