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

/*
*** Aim of the function: copy str until the cursor position
*/

int 		alt_x(t_tcap *caps)
{
    if (caps->copy_str)
        free(caps->copy_str);
    caps->copy_str = ft_strndup(caps->str[0], caps->cursor - caps->size_prompt);
    return (0);
}

/*
*** Aim of the function: copy str from the cursor position to the end
*/

int 		alt_s(t_tcap *caps)
{
    if (caps->copy_str)
        free(caps->copy_str);
    caps->copy_str = ft_strdup(caps->str[0] + caps->cursor - caps->size_prompt);
    return (0);
}

/*
*** Aim of the function: copy entire str
*/

int 		alt_w(t_tcap *caps)
{
    if (caps->copy_str)
        free(caps->copy_str);
    caps->copy_str = ft_strdup(caps->str[0]);
    return (0);
}

/*
*** Aim of the function: paste the str
*** --> WILL NEED TO FREE THE CUT STR, NOW OR AFTER ?
*** --> I think when exit of function get_line_term,
*** --> free all the str that need to be free
*/

int 		alt_p(t_tcap *caps)
{
    int i;

    i = -1;
    if (caps->copy_str)
    {
        while (caps->copy_str[++i])
        {
            if (caps->copy_str[i] != '\n')
            {
                ft_bzero(caps->buf, 2048);
                caps->buf[0] = caps->copy_str[i];
                print_normal_char(caps);
            }
        }
    }
    return (0);
}
