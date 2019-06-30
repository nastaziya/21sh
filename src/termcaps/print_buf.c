/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/03/14 15:12:34 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

/*
*** - For the cmd + c || cmd + v management ! I modified the size of the buf
*** - to 2048. And manages checks if ft_strlen == 1 (normal char) or not
*/

int		print_buf(t_tcap *caps, char *buf)
{
	int		i;
	char	*str;

	i = -1;
	str = ft_strdup(buf);
	if (ft_strlen(str) == 1 && str[0] != 9)
		print_normal_char(caps);
	free(str);
	return (0);
}
