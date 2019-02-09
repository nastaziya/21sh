/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 17:28:15 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"

/*
*** - Aim of the function :
*** - Print errors, and return 1
*/

int		ft_find_path_and_cd2(char c)
{
	if (c == '~')
		ft_putstr_fd("No $HOME variable set.\n", 2);
	else
		ft_putstr_fd("No $OLDPWD variable set.\n", 2);
	return (1);
}

/*
*** - Aim of the function :
*** - copies the behavior of the len of
*** - the char** for the cd command
*** - stops when meets '\0' or a '-' char
*/

int		ft_len_array_char_cd(char **av)
{
	int	i;

	i = 0;
	while (av[i] && (i > 0 ? ft_strcmp(av[i - 1], "-") : 1))
		i++;
	return (i);
}

/*
*** - Aim of the function :
*** - returns the str after all the ./././
*** - Example :
*** - cd ./././21LN -> returns 21LN
*/

char	*ft_skip_slash(char *s)
{
	int i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '.')
			i++;
		if (s[i] == '/')
		{
			i++;
			continue ;
		}
		return (s + i);
	}
	return (s);
}

/*
*** - Aim of the function :
*** - prints the error and returns the proper int
*** - Example :
*** - We do : cd Makefile
*** - prints : "Makefile: Not a directory"
*/

int		ft_print_error(char *av, char *str)
{
	ft_putstr_fd(av, 2);
	ft_putstr_fd(str, 2);
	return (1);
}

/*
*** - Aim of the function :
*** - prints the error and returns the proper int
*** - Example :
*** - We do : cd -O -
*** - prints : cd: usage: cd [-L|-P] [dir]
*/

int		ft_print_dir_error(char *command, char *btwn, char *after, int i)
{
	ft_putstr_fd(command, 2);
	ft_putstr_fd(btwn, 2);
	ft_putendl_fd(after, 2);
	return (i);
}
