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
*** - checks if type belongs to the ALL set
*** - ALL = ARG + UNARY + BINARY + EXPANSED
*/

int             is_all(t_args_tok *t)
{
    if (t->type == T_ARGS || t->type == T_UNARY ||
        t->type == T_BINARY || t->type == T_EXPANSED)
        return (1);
    return (0);
}

/*
*** - Aim of the function :
*** - checks if type belongs to the ANY set
*** - ANY = ALL + NOT + EXPANSED
*/

int             is_any(t_args_tok *t)
{
    if (t->type == T_ARGS || t->type == T_UNARY ||
        t->type == T_BINARY || t->type == T_NOT
        || t->type == T_EXPANSED)
        return (1);
    return (0);
}

/*
*** - Aim of the function :
*** - checks if type belongs to the S_BIN set
*** - S_BIN = BINARY + UNARY + NOT + EXPANSED
*/

int             is_s_bin(t_args_tok *t)
{
    if (t->type == T_UNARY || t->type == T_NOT || T_ARGS || 
        t->type == T_EXPANSED)
        return (1);
    return (0);
}

/*
*** - Aim of the function :
*** - Manage the 2 args situation
*/

int             ft_test_2_args(t_args_tok  **t, int boul)
{
    int ret;

    ret = 0;
    if (t[0]->type == T_UNARY)
        ret = process_unary(t[0]->tok, t[1]->op);
    else if (t[0]->type == T_NOT)
        ret = process_exclam(1, 0);
    else
    {
        if (boul == 0)
            dprintf(1, "bash: test: %s: unary operator expected\n", t[0]->op);
        ret = 2;
    }
    return (ret);
}

/*
*** - Aim of the function :
*** - Manage the 3 args situation
*/

int             ft_test_3_args(t_args_tok  **t, int boul)
{
    int ret;

    ret = 0;
    if (is_any(t[0]) && t[1]->type == T_BINARY && is_any(t[2]))
        ret = process_binary(t[0]->op, t[1]->tok, t[2]->op);
    else if (t[0]->type == T_NOT && t[1]->type == T_UNARY && is_any(t[2]))
        ret = process_exclam(1, process_unary(t[1]->tok, t[2]->op));
    else
    {
        if (t[0]->type == T_NOT && t[1]->type == T_NOT && is_any(t[2])
            && (ret = 2) && boul == 0)
            dprintf(1, "bash: test: too many arguments\n");
        else if (is_all(t[0]) && is_any(t[1]) && is_any(t[2]) && (ret = 2)
            && boul == 0)
            dprintf(1, "bash: test: %s: binary operator expected\n", t[1]->op);
        else if (t[0]->type == T_NOT && (t[1]->type == T_ARGS
            || t[1]->type == T_EXPANSED) && is_any(t[2]) && (ret = 2)
                && boul == 0)
            dprintf(1, "bash: test: %s: unary operator expected\n", t[1]->op);
    }
    return (ret);
}