/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 18:59:56 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/expansion.h"

static int	manage_sig_term_ret_2(int ret)
{
	if (ret == 11)
		ft_putendl_fd("Segmentation fault: 11", 2);
	else if (ret == 13)
		ft_putendl_fd("Broken pipe: 13", 2);
	else if (ret == 16)
		ft_putendl_fd("Stack fault: 16", 2);
	return (128 + ret);
}

int			manage_sig_term_ret_1(int ret)
{
	if (ret == 6)
		ft_putendl_fd("Abort trap: 6", 2);
	else if (ret == 7 || ret == 0)
		ft_putendl_fd("Bus error: 10", 2);
	else if (ret == 8)
		ft_putendl_fd("Floating-point exception: 8", 2);
	else
		return (manage_sig_term_ret_2(ret));
	return (ret + 128);
}

void		print_errors(char *error, char *file_name)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(file_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error, 2);
}
