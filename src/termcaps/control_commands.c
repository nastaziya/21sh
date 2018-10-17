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
    // (void)i;
    if (caps->sz_str - caps->size_prompt > 0)
        free(caps->str[0]);
    caps->sz_str = caps->size_prompt;
    // *i = 0;
    // tputs(tgetstr("vi", NULL), 1, ft_outc);
    tputs(tgoto(tgetstr("cm", NULL), 0, 0), 1, ft_outc);
    // sleep(4);//
    tputs(tgetstr("cd", NULL), 1, ft_outc);
    // tputs(tgetstr("ve", NULL), 1, ft_outc);
    return (0);
}