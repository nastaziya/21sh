/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 22:57:36 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

int     new_line(t_tcap *caps)
{
    if (caps->buf[0] == 10 && caps->buf[1] == 0 &&
        caps->buf[2] == 0 && caps->buf[3] == 0 && caps->buf[4] == 0)
        return (0);
    return (1);
}

int     ctrlld(t_tcap *caps)
{
    if (caps->buf[0] == 4 && caps->buf[1] == 0 &&
        caps->buf[2] == 0 && caps->buf[3] == 0 && caps->buf[4] == 0)
        return (0);
    return (1);
}

int     equality(t_tcap *caps, t_tab *tmp_tab)
{
    if (caps->buf[0] == tmp_tab->key0 && caps->buf[1] == tmp_tab->key1
        && caps->buf[2] == tmp_tab->key2 && caps->buf[3] == tmp_tab->key3
            && caps->buf[4] == tmp_tab->key4)
        return (0);
    return (1);
}