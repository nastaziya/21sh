/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   exec_2.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/15 15:20:51 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/15 15:20:53 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/exec.h"
#include "../../inc/builtin.h"
#include "../../inc/expansion.h"

/*
*** - Aim of the function :
*** - Print is_dir error
*/

int			ft_print_error_directory(char *str, char *str1, char *str2,
				int fd)
{
	ft_putstr_fd(str, fd);
	ft_putstr_fd(str1, fd);
	ft_putendl_fd(str2, fd);
	return (1);
}

/*
*** - Aim of the function :
*** - Print is_dir error
*/

int			ft_perror_norm_dir(char *str1, int fd)
{
	ft_putstr_fd("bash: ", fd);
	ft_putstr_fd(str1, fd);
	ft_putendl_fd(": Is a directory", fd);
	return (1);
}

/*
*** - Aim of the function :
*** - Handle [n]<<[delimiter]
*/

int			manage_here_doc(t_simp_com cmd, int i, t_exec_redir *t, int k)
{
	int		ret;
	int		fd[2];

	pipe(fd);
	t->i_hdoc = k;
	if (t->heredoc && t->heredoc[0] && t->heredoc[0][t->i_hdoc])
		write(fd[1], t->heredoc[0][t->i_hdoc],
			ft_strlen(t->heredoc[0][t->i_hdoc]));
	close(fd[1]);
	ret = dup2(fd[0], cmd.redirection.fd[i]);
	close(fd[0]);
	t->i_hdoc = t->i_hdoc + 1;
	if (ret < 0)
	{
		return (ft_print_error_directory("bash: ",
			cmd.redirection.file[i], ": Bad file descriptor", 2));
	}
	return (0);
}

/*
*** - Aim of the function :
*** - Handle Aggregations -> [n]>&[word]
*/

int			manage_aggreg(t_simp_com cmd, int i, t_exec_redir *t)
{
	if (!ft_strcmp(cmd.redirection.file[i], "-"))
	{
		close(cmd.redirection.fd[i]);
		return (0);
	}
	else
		t->fdoutred[i] = ft_atoi(cmd.redirection.file[i]);
	if ((dup2(t->fdoutred[i], cmd.redirection.fd[i])) < 0)
	{
		return (ft_print_error_directory("bash: ",
			cmd.redirection.file[i], ": Bad file descriptor", 2));
	}
	return (0);
}
