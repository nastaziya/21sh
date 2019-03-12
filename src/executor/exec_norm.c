/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   exec_norm.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/24 16:46:01 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/24 16:53:07 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include "../../inc/expansion.h"
#include "../../inc/exec.h"

/*
*** - function tells if the word after the >& && <& is a redir number
*/

int		ft_isnumber_redir(char *str)
{
	int	i;

	i = -1;
	if (!ft_strcmp(str, "-"))
		return (0);
	while (str[++i])
		if (!ft_isdigit(str[i]))
			return (1);
	return (0);
}

void	init_red_struct(t_exec_redir *t_red, char ***heredoc)
{
	t_red->i_hdoc = 0;
	t_red->count_here = 0;
	t_red->file_name = NULL;
	t_red->fdoutred = 0;
	t_red->heredoc = heredoc;
	ft_bzero(t_red->fd_orig, 3);
}

int		expand_filename(t_simp_com cmd, t_exec_redir *t, int i,
			t_env_tools *env)
{
	char *temp;

	t->file_name = NULL;
	temp = ft_strdup(cmd.redirection.file[i]);
	expanded_dynamic_table(&temp, *env, 0);
	t->file_name = ft_strdup(temp);
	free(temp);
	if (temp && ft_strlen(t->file_name) == 0)
	{
		free(t->file_name);
		ft_print_error_directory("bash: ",
			cmd.redirection.file[i], ": ambiguous redirect", 2);
		return (1);
	}
	return (0);
}
