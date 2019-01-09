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
#include "../../inc/termcap.h"

int		ft_print_usage_error_exit(char *btwn)
{
	ft_putstr_fd("bash: exit: ", 2);
	ft_putstr_fd(btwn, 2);
	ft_putendl_fd(": numeric argument required", 2);
    // ft_free_tout_ce_qu_il_y_a_a_free
    exit(255);
	return (255);
}

int             ft_check_num(char *str)
{
    int i;

    i = -1;
    while (str[++i])
        if (!ft_isdigit(str[i]))
            return (1);
    return (0);
}

int             ft_builtin_exit(char **cmd, t_env_tools *env)
{
    int argc;

    dprintf(3, "builtin_exit: |%s|\n", cmd[1]);
    if (((argc = ft_len_array_char(cmd)) > 2))
        return (ft_int_error("exit: Too many arguments.\n", 2, 1));
    else if (argc == 2 && ft_check_num(cmd[1])) // FAUT - IL EXIT QUAND MEME OU NON ???
        return (ft_print_usage_error_exit(cmd[1]));
    else if (argc == 1)
    {
        // free(caps.str[0]);
        // free(caps.str);
        // ft_free_tout_ce_qu_il_y_a_a_free
        dprintf(3, "passe par lexit youhooooou\n");
        exit(0);
    }
    else if (argc == 2)
    {
        // ft_free_tout_ce_qu_il_y_a_a_free
        exit(atoi(cmd[1]) % 256);
    }
    // else if (argc == 2)
    return (0);
}