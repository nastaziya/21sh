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
*** - Aim of the function : Collect at a given moment the
***	- position of the cursor - I use it to manage the size of 
*** - the cursor at any given time + at the beginning to 
*** - manage the history (after)
*/

void		get_cursor_position(int curs_pos[2])
{
 	char 			buf[32];
 	unsigned long	i;
 	int 			j;
	
	i = 0;
	j = 2;
	ft_bzero(buf, 32);
	if (write(0, "\x1b[6n", 4) != 4)
		return ;
  	while (i < sizeof(buf)-1)
  	{
        if (read(0,buf+i,1) != 1)
        	break;
        if (buf[i] == 'R')
        	break;
        i++;
    }
    buf[i] = '\0';
	if (!ft_isdigit(*(buf + 2)))
		return ;
	curs_pos[1] = ft_atoi(buf + 2);
	while (buf[j] && buf[j] != 59)
		j++;
	curs_pos[0] = ft_atoi(buf + j + 1);
}

/*
*** - Aim of the function : Iterate on the 
*** - get_cursor_position function in order to collect
*** - a proper curs_pos
*/

void			cursor_position(int curs_pos[2])
{
	curs_pos[0] = -1;
	curs_pos[1] = -1;
	while (42)
	{
		get_cursor_position(curs_pos);
		if (curs_pos[0] != -1 && curs_pos[1] != -1)
			break ;
	}
}