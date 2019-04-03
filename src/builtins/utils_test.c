/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 17:44:41 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"
# include "stdio.h"

/*
*** - Aim of the function :
*** - Initialize the array of token according to the len sent
*/

t_args_tok      **initialize_arr_tok(int len)
{
    int         i;
    t_args_tok  **t;

    if (!(t = ((t_args_tok **)malloc(sizeof(t_args_tok*) * (len + 1)))))
        return (NULL);
    t[len] = NULL;
    i = -1;
    while (++i < len)
    {
        if (!(t[i] = ((t_args_tok *)malloc(sizeof(t_args_tok)))))
            return (NULL);
        ft_bzero(t[i], sizeof(t[i]));
    }
    return (t);
}

/*
*** - Aim of the function :
*** - Check is the expansion didn't turn the str into a full ' ' str
*** - < "" $DOESNT_EXIST > ==> <      > (becomes str full of spaces)
*/

int             is_real_str_after_expansion(char *cmd)
{
    int i;

    i = -1;
    while (cmd[++i] == ' ')
        ;
    if (!cmd[i])
        return (1);
    return (0);
}

/*
*** - Aim of the function :
*** - Free everything related to the tokens
*/

int             free_tokens(t_args_tok  ***t, int len)
{
    int     i;

    i = 0;
    while (i++ < len)
    {
        if (*t && (*t)[i] && ((*t)[i])->op)
            free(((*t)[i])->op);
        if (*t && (*t)[i])
            ((*t)[i])->op = NULL;
        if ((*t)[i])
            free((*t)[i]);
        (*t)[i] = NULL;
    }
    free(*t);
    *t = NULL;
    return (0);
}

/*
*** - Aim of the function :
*** - Count the number of arguments in av
*** - However, we skip strings that contain only spaces
*/

int             ft_len_array_char_test(char **av, char **raw_av)
{
	int	i;
    int j;
    int count;

	i = -1;
    count = 0;
	while (av[++i])
    {
        j = -1;
        while (av[i][++j] == ' ')
            ;
        if (av[i][j] || (!av[i][j] && (ft_strchr(raw_av[i], '\'')
            || ft_strchr(raw_av[i], '\"'))))
            count++;
    }
	return (count);
}