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
*** - Heart of the tokenization
*** - We first iterate on the array of commands to see if there is a match
*** - If yes, we fill the infos according to the match
*** - If nothing was found, it's an argument, we also add it
*/

void            tokenize_test(char **cmd, t_args_tok  ***t, char **raw_cmd)
{
    int                 i;
    int                 j;
    int                 diff;

    i = 0;
    diff = 0;
    while (cmd[++i])
    {
        // dprintf(1, "TOKENIZE_TEST: |%s|\n", cmd[i]);
        if (is_real_str_after_expansion(cmd[i]))
        {
            // manage -> \" or \' expansion
            if  (!ft_strcmp(raw_cmd[i], "\"\"") || !ft_strcmp(raw_cmd[i], "\'\'"))
            {
                // dprintf(1, "passé-ici, dans les quotes |%s|%d\n", cmd[i], i - diff - 1);
                ((*t)[i - diff - 1])->op = ft_strdup(" ");
                ((*t)[i - diff - 1])->size = 0;
                ((*t)[i - diff - 1])->tok = T_QUOTE;
                ((*t)[i - diff - 1])->type = T_EXPANSED;
            }
            // manage -> \" or \' expansion but when filled with spaces
            else if (ft_strchr(raw_cmd[i], '\"') || ft_strchr(raw_cmd[i], '\''))
            {
                // dprintf(1, "debug-quotes non vide, remplies despaces:|%s|-|%s|\n", cmd[i], raw_cmd[i]);
                ((*t)[i - diff - 1])->op = ft_strjoin(cmd[i], " ");
                ((*t)[i - diff - 1])->size = ft_strlen(cmd[i]);
                ((*t)[i - diff - 1])->tok = T_WORDS;
                ((*t)[i - diff - 1])->type = T_ARGS;
            }
            else
                diff++;
        }
        else
        {
            j = -1;
            while (g_args_tok[++j].op)
            {
                if (ft_strcmp(cmd[i], g_args_tok[j].op) == 0)
                {
                    ((*t)[i - diff - 1])->op = ft_strdup(g_args_tok[j].op);
                    ((*t)[i - diff - 1])->size = g_args_tok[j].size;
                    ((*t)[i - diff - 1])->tok = g_args_tok[j].tok;
                    ((*t)[i - diff - 1])->type = g_args_tok[j].type;
                    break ;
                }
            }
            if (!g_args_tok[j].op)
            {
                ((*t)[i - diff - 1])->op = ft_strdup(cmd[i]);
                ((*t)[i - diff - 1])->size = ft_strlen(cmd[i]);
                ((*t)[i - diff - 1])->tok = T_WORDS;
                ((*t)[i - diff - 1])->type = T_ARGS;
            }
        }
    }
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