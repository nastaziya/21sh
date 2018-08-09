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

void			ft_replace_spaces_inside_quotes(char *str, char to_replace,
					char in_what)
{
	int		i;
	int		nb_quote;
	char	to_replace2;
	char	in_what2;

	i = 0;
	nb_quote = 0;
	to_replace2 = ((to_replace == ' ' ||
		to_replace == (char)255) ? '\t' : (char)254);
	in_what2 = (to_replace == (' ' ||
		to_replace == (char)255) ? (char)254 : '\t');
	while (str[i])
	{
		if (str[i] == '"' && nb_quote == 0)
			nb_quote++;
		else if (nb_quote == 1 && str[i] == to_replace)
			str[i] = in_what;
		else if (nb_quote == 1 && str[i] == to_replace2)
			str[i] = in_what2;
		else if (str[i] == '"' && nb_quote == 1)
			nb_quote--;
		i++;
	}
}
