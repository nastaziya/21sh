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
 	char 	buf[32];
 	int		i;
 	int 	j;
	
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

// #define ANSI_Q_CURSORXY			"\033[6n"
// #define ANSI_Q_CURSORXY_SIZE	 sizeof("\033[6n") - 1

// void		cursor_position(int curs_pos[2])
// {
// 	char				buf[16];
// 	size_t				i;

// 	// caps__get_context(&context);
// 	if (write(0, ANSI_Q_CURSORXY,
// 		ANSI_Q_CURSORXY_SIZE) != ANSI_Q_CURSORXY_SIZE)
// 		return ;
// 	if (read(0, buf, sizeof(buf)) == sizeof(buf))
// 		return ;
// 	i = sizeof("\033[") - 1;
// 	curs_pos[1] = ft_atoi(buf + i);
// 	while (ft_isdigit(buf[i]))
// 		i++;
// 	i += sizeof(";") - 1;
// 	curs_pos[0] = ft_atoi(buf + i);
// }

// bool	caps__cursor_getxy(int *out_x, int *out_y)
// {
// 	int		x;
// 	int		y;

// 	if (out_x != NULL)
// 		*out_x = 0;
// 	if (out_y != NULL)
// 		*out_y = 0;
// 	if (!s_caps__cursor_getxy(&x, &y))
// 		return (false);
// 	if (out_x)
// 		*out_x = x;
// 	if (out_y)
// 		*out_y = y;
// 	return (true);
// }