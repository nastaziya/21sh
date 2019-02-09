/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   print.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/09 22:04:59 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 22:08:35 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

void	print_array(int size, char **cmd)
{
	int	i;

	i = 0;
	while (i < size)
	{
		printf("%s\n", cmd[i]);
		i++;
	}
}

void	print_array_token(int size, t_token_type *cmd)
{
	int	i;

	i = 0;
	while (i < size)
	{
		printf("%d\n", cmd[i]);
		i++;
	}
}

void	print_array_int(int size, int *cmd)
{
	int i;

	(void)cmd;
	(void)size;
	i = 0;
	while (i < size)
	{
		printf("%d\n", cmd[i]);
		i++;
	}
}

void	print_struct(t_command cmd)
{
	int j;

	(void)cmd;
	j = 0;
	while (j < cmd.used_space)
	{
		printf("COMMAND : \n");
		print_array(cmd.command[j].used_space, cmd.command[j].cmd_simple);
		printf("tok : %d\n", cmd.command[j].tok);
		if (cmd.command[j].redirection.used_space > 0)
		{
			printf("REDIRECTIONS: \n");
			print_array_token(cmd.command[j].redirection.used_space,
				cmd.command[j].redirection.red);
			print_array(cmd.command[j].redirection.used_space,
				cmd.command[j].redirection.file);
			print_array_int(cmd.command[j].redirection.used_space,
				cmd.command[j].redirection.fd);
		}
		j++;
	}
}
