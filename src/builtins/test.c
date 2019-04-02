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
*** - return the reverse of ret_in (which can be 0 or 1)
*** - regarding the number of T_NOT argument (== number of "!")
*/

int             process_exclam(int nb, int ret_in)
{
    int ret;

    ret = ret_in;
    if (ret_in == 1)
        ret = (nb % 2 == 0 ? 1 : 0);
    else
        ret = (nb % 2 == 0 ? 0 : 1);
    
    return (ret);
}

/*
*** - Aim of the function :
*** - Manage the 4 args situation
*/

int             ft_test_4_args(t_args_tok  **t, int boul)
{
    int ret;

    ret = 0;
    if (t[0]->type == T_NOT && is_any(t[1]) && t[2]->type == T_BINARY
        && is_any(t[3]))
        ret = process_exclam(1, process_binary(t[1]->op, t[2]->tok, t[3]->op));
    else if (t[0]->type == T_NOT && t[1]->type == T_NOT
        && t[2]->type == T_UNARY && is_any(t[3]))
        ret = process_exclam(2, process_unary(t[2]->tok, t[3]->op));
    else
    {
        if (((is_all(t[0]) && is_any(t[1]) && is_any(t[2]) && is_any(t[3])) ||
            (t[0]->type == T_NOT && t[1]->type == T_NOT && t[2]->type == T_NOT
                && is_any(t[3]))) && (ret = 2) && boul == 0)
            dprintf(1, "bash: test: too many arguments\n");
        else if (t[0]->type == T_NOT && t[1]->type == T_NOT
            && (t[2]->type == T_ARGS || t[2]->type == T_EXPANSED) 
                && is_any(t[3]) && (ret = 2) && boul == 0)
            dprintf(1, "bash: test: %s: unary operator expected\n", t[2]->op);
        else if (t[0]->type == T_NOT && is_all(t[1]) && is_s_bin(t[2])
            && is_any(t[3]) && (ret = 2) && boul == 0)
            dprintf(1, "bash: test: %s: binary operator expected\n", t[2]->op);
    }
    return (ret);
}

/*
*** - Aim of the function :
*** - Manage the 5 args situation
*/

int             ft_test_5plus_args(t_args_tok  **t, int boul, char **cmd,
                    char **raw_cmd)
{
    int ret;
    int i;
    int len;

    ret = 0;
    i = 0;
    len = 0;
    while (t[i] && t[i]->type == T_NOT)
        i++;
    len = ft_len_array_char_test(&cmd[i + 1], &raw_cmd[i + 1]);
    if (len > 3 && (ret = 2))
        dprintf(1, "bash: test: too many arguments\n");
    else if (len == 0 && (ret = 2))
        dprintf(1, "bash: test: argument expected\n");
    else if (len == 1)
        ret = 0;
    else if (len == 2 && (ret = process_exclam(i,
        ft_test_2_args(&t[i], boul))))
        ret == 2 ? dprintf(1, "bash: test: too many arguments\n") : 0;
    else if (len == 3 && (ret = process_exclam(i,
        ft_test_3_args(&t[i], boul))))
        ret == 2 ? dprintf(1, "bash: test: too many arguments\n") : 0;
    return (ret);
}

/*
*** - Aim of the function :
*** - Manage the test builtin
*/

int			    ft_builtin_test(char **cmd, char **raw_cmd)
{
    int         len;
    t_args_tok  **t;
    int         ret;

    ret = 0;
    len = ft_len_array_char_test(cmd + 1, raw_cmd + 1);
    t = initialize_arr_tok(len);
    // dprintf(1, "len: %d\n", len);
    tokenize_test(cmd, &t, raw_cmd);
    // dprintf(1, "len: %d-%d\n", len, t[0]->tok);
    if (len == 0 || (len == 1 && t[0]->tok == T_QUOTE))
        ret = 1;
    else if (len == 1)
        ret = 0;
    else if (len == 2)
        ret = ft_test_2_args(t, 0);
    else if (len == 3)
        ret = ft_test_3_args(t, 0);
    else if (len == 4)
        ret = ft_test_4_args(t, 0);
    else if (len > 4)
        ret = ft_test_5plus_args(t, 1, cmd, raw_cmd);
    free_tokens(&t, len);
    return (ret);
}