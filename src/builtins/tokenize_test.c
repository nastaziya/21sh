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

// void        tokenize_check_expansion_first(t_args_tok  ***t, char **cmd, char **raw_cmd, int i, int diff)

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
        if (is_real_str_after_expansion(cmd[i]))
        {
            if  (!ft_strcmp(raw_cmd[i], "\"\"") || !ft_strcmp(raw_cmd[i], "\'\'"))
            {
                ((*t)[i - diff - 1])->op = ft_strdup(" ");
                ((*t)[i - diff - 1])->size = 0;
                ((*t)[i - diff - 1])->tok = T_QUOTE;
                ((*t)[i - diff - 1])->type = T_EXPANSED;
            }
            else if (ft_strchr(raw_cmd[i], '\"') || ft_strchr(raw_cmd[i], '\''))
            {
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