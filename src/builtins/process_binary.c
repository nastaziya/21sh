
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
*** - Check if it's a proper number
*/

int             is_proper_number(char *str)
{
    if (str && *str == '-')
        str++;
    while (str && *str)
    {
        if (!ft_isdigit(*str))
            break ;
        str++;
    }
    if (str && *str)
        return (1);
    else
        return (0);
}

/*
*** - Aim of the function :
*** - Check wether str1 and str2 and numbers
*** - if not, print the proper error message
*** - if yes, assign the int to the correspondant variable
*/

int             ft_atoi_checker(char *str1, char *str2, int *one, int *two)
{
    if (!is_proper_number(str1))
        *one = ft_atoi(str1);
    else if (dprintf(1, "bash: test: %s: integer expression expected\n",
        str1))
        return (2);
    if (!is_proper_number(str2))
        *two = ft_atoi(str2);
    else if (dprintf(1, "bash: test: %s: integer expression expected\n",
        str2))
        return (2);
    return (0);
}

/*
*** - Aim of the function :
*** - Norm of process_binary
*/

void             norm_process_binary(t_test_tok tok, int one, int two,
                    int *ret)
{
    if (tok == T_EQ)
        *ret = (one == two ? 0 : 1);
    else if (tok == T_NE)
        *ret = (one != two ? 0 : 1);
    else if (tok == T_GE)
        *ret = (one >= two ? 0 : 1);
    else if (tok == T_LT)
        *ret = (one < two ? 0 : 1);
    else if (tok == T_LE)
        *ret = (one <= two ? 0 : 1);
}

/*
*** - Aim of the function :
*** - Process the Binary test
*/

int             process_binary(char *str1, t_test_tok tok, char *str2)
{
    int ret;
    int one;
    int two;
    
    ret = 0;
    if (tok == T_SEQ)
        ret = (!ft_strcmp(str1, str2) ? 0 : 1);
    else if (tok == T_SNE)
        ret = (ft_strcmp(str1, str2) ? 0 : 1);
    else
    {
        if (ft_atoi_checker(str1, str2, &one, &two) == 2)
            return (2);
        norm_process_binary(tok, one, two, &ret);
    }
    return (ret);
}