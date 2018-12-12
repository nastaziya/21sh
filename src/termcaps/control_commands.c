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

int			ctrl_l(t_tcap *caps)
{
    int i;

    i = -1;
    if (caps->y_prompt > 0)
    {
        size_windows(caps);
        home_key(caps);
        tputs(tgoto(tgetstr("cm", NULL), 0, caps->window_size[0]), 0, ft_outc);
        while (--caps->y_prompt > 0)
            tputs(tgetstr("sf", NULL), 1, ft_outc);
        tputs(tgoto(tgetstr("cm", NULL), caps->size_prompt, 0), 0, ft_outc);
        end_key(caps);
    }
    return (0);
}

// int			ctrl_d(t_tcap *caps)
// {
//     if (((caps->sz_str - caps->size_prompt) == 0))
//     {
//         // if (caps->str[0])
//         // {
//             free(caps->str[0]);
//             caps->str[0] = ft_strdup("exit");
//         // }
//     }
//     else
//         tputs(tgetstr("bl", NULL), 1, ft_outc);
//     return (0);
// }

// int 		debug(t_tcap *caps)
// {
//     return (0);
// }