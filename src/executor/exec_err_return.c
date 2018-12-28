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
#include "../../inc/expansion.h"

static int	manage_sig_term_ret_2(int ret)
{
	int new_ret;

	new_ret = 0;
	if(ret == 11)
	{
		ft_putendl_fd("Segmentation fault: 11", 2);
		new_ret = 128 + 11;
	}
	else if(ret == 13)
	{
		ft_putendl_fd("Broken pipe: 13", 2);
		new_ret = 128 + 13;
	}
	else if(ret == 16)
	{
		ft_putendl_fd("Stack fault: 16", 2);
		new_ret = 128 + 16;
	}
	return (new_ret);
}


int	    manage_sig_term_ret_1(int ret)
{
	int new_ret;

	new_ret = 0;
	if (ret == 6)
	{
		ft_putendl_fd("Abort trap: 6", 2);
		new_ret = 128 + 6;
	}
	else if(ret == 7)
	{
		ft_putendl_fd("Bus error: 7", 2);
		new_ret = 128 + 7;
	}
	else if(ret == 8)
	{
		ft_putendl_fd("Floating-point exception: 8", 2);
		new_ret = 128 + 8;
	}
	else
 		new_ret = manage_sig_term_ret_2(ret);
	return (new_ret);
}

void    print_errors(char *error, char *file_name)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(file_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error, 2);
}

// int		error_exit(t_env_tools *env_struct, char **str, char *path, int res)
// {
// 	struct stat s;

// 	if (lstat(str[0], &s) != -1 && S_ISDIR(s.st_mode))
// 	{
// 		print_errors("is a directory", str[0]);
// 		return (126);
// 	}
// 	else if (lstat(str[0], &s) == -1 && str[0] && ft_strchr(str[0], '/'))
// 	{
// 		print_errors("No such file or directory", str[0]);
// 		return (127);
// 	}
// 	else if (lstat(str[0], &s) == -1 && res == 0)
// 	{
// 		print_errors("command not found", str[0]);
// 		return (127);
// 	}
// 	else if(access(path, X_OK) == -1)
// 	{
// 		print_errors("Permission denied", str[0]);
// 		return (126);
// 	}	
// 	else
// 		return (exec(path, str, env_struct->env_cpy));
// 	return(0);
// }