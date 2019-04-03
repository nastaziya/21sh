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

/*
*** - Aim of the function :
*** - Tokenize the expansed words
*** - For < "" > string, we assign the T_QUOTE tag, and the " " op
*** - For < "  " > string, we assign the T_WORD tag, and 
*** - the op == str + " " in order to differenciate the "" and " " str after
*** - If the expansion is an empty variable, skip it -> $Dontexist -> skipped
*/

void        tokenize_check_expansion_first(t_args_tok  ***t, t_tok_norm p,
                int *i, int *diff)
{
    if  (!ft_strcmp(p.raw_cmd[*i], "\"\"")
        || !ft_strcmp(p.raw_cmd[*i], "\'\'"))
    {
        ((*t)[*i - *diff - 1])->op = ft_strdup(" ");
        ((*t)[*i - *diff - 1])->size = 0;
        ((*t)[*i - *diff - 1])->tok = T_QUOTE;
        ((*t)[*i - *diff - 1])->type = T_EXPANSED;
    }
    else if (ft_strchr(p.raw_cmd[*i], '\"') || ft_strchr(p.raw_cmd[*i], '\''))
    {
        ((*t)[*i - *diff - 1])->op = ft_strjoin(p.cmd[*i], " ");
        ((*t)[*i - *diff - 1])->size = ft_strlen(p.cmd[*i]);
        ((*t)[*i - *diff - 1])->tok = T_WORDS;
        ((*t)[*i - *diff - 1])->type = T_ARGS;
    }
    else
        (*diff)++;
}

/*
*** - Aim of the function :
*** - First iterates on the array of command to see a pattern
*** - If there's a match, we copy from the corresponding example
*** - If not, this is a normal word, and we assign it the argument tag
*/

void            tokenize_from_pattern(t_args_tok  ***t, t_tok_norm p,
                    int *i, int *diff)
{
    int     j;

    j = -1;
    while (g_args_tok[++j].op)
    {
        if (ft_strcmp(p.cmd[*i], g_args_tok[j].op) == 0)
        {
            ((*t)[*i - *diff - 1])->op = ft_strdup(g_args_tok[j].op);
            ((*t)[*i - *diff - 1])->size = g_args_tok[j].size;
            ((*t)[*i - *diff - 1])->tok = g_args_tok[j].tok;
            ((*t)[*i - *diff - 1])->type = g_args_tok[j].type;
            break ;
        }
    }
    if (!g_args_tok[j].op)
    {
        ((*t)[*i - *diff - 1])->op = ft_strdup(p.cmd[*i]);
        ((*t)[*i - *diff - 1])->size = ft_strlen(p.cmd[*i]);
        ((*t)[*i - *diff - 1])->tok = T_WORDS;
        ((*t)[*i - *diff - 1])->type = T_ARGS;
    }
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
    int                 diff;
    t_tok_norm          p;

    i = 0;
    p.cmd = cmd;
    p.raw_cmd = raw_cmd;
    diff = 0;
    while (cmd[++i])
    {
        if (is_real_str_after_expansion(cmd[i]))
            tokenize_check_expansion_first(t, p, &i, &diff);
        else
            tokenize_from_pattern(t, p, &i, &diff);
    }
}