/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/19 18:02:22 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"

int             ft_check_num(char *str)
{
    while (str && *str && str++)
        if (ft_isdigit(*str))
            return (1);
    return (0);
}

int             ft_builtin_exit(char **cmd, t_env_tools *env)
{
    int argc;

    if (((argc = ft_len_array_char(cmd)) > 2))
        return (ft_int_error("exit: Too many arguments.\n", 2, 1));
    else if (argc == 2 && ft_check_num(cmd[1])) // FAUT - IL EXIT QUAND MEME OU NON ???
        return (ft_usage_error("bash: exit: ", cmd[1], ": numeric argument required", 255));
    return (0);
}