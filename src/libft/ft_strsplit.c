/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strsplit.c                                    .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:21 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/19 18:02:22 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static int		isnotlegit(char c, char delimitor)
{
	if (c && c != delimitor)
		return (1);
	else
		return (0);
}

static int		ft_count_words(char *str, char delimitor)
{
	int		count_words;

	count_words = 0;
	while (*str)
	{
		while (*str == delimitor)
			str++;
		if (*str)
			count_words++;
		while (*str != delimitor && *str)
			str++;
	}
	return (count_words);
}

static char		**ft_intab(char *src, char **dest, int operator, char delimitor)
{
	int		i;

	i = 0;
	while (isnotlegit(src[i], delimitor))
		i++;
	if ((dest[operator] = (char *)malloc(sizeof(char) * (i + 1))) == NULL)
		return (NULL);
	i = 0;
	while (isnotlegit(src[i], delimitor))
	{
		dest[operator][i] = src[i];
		i++;
	}
	dest[operator][i] = '\0';
	return (dest);
}

char			**ft_strsplit(char const *str, char delimitor)
{
	int		i;
	int		j;
	char	**dest;
	int		count_words;

	i = 0;
	j = 0;
	if (!str)
		return (NULL);
	count_words = ft_count_words((char*)str, delimitor);
	if ((dest = (char**)malloc(sizeof(char*) * (count_words + 1))) == NULL)
		return (NULL);
	while (i < count_words)
	{
		while ((char*)str && !isnotlegit(str[j], delimitor))
			j++;
		ft_intab((char*)str + j, dest, i, delimitor);
		while ((char*)str && isnotlegit(str[j], delimitor))
			j++;
		i++;
	}
	dest[i] = NULL;
	return (dest);
}
