/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 19:59:21 by gurival-    ###    #+. /#+    ###.fr     */
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

static int	manage_here_doc(t_simp_com cmd, int i, t_exec_redir *t)
{
	int		ret;
	int		fd[2];

	pipe(fd);
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
*** - Process the fds redirections in order to process the manage file function
*/

static int	manage_file_norm(t_simp_com cmd, int i, t_exec_redir *t,
				struct stat buf)
{
	if (cmd.redirection.red[i] == T_GREAT)
	{
		if (S_ISDIR(buf.st_mode))
			return (ft_perror_norm_dir(cmd.redirection.file[i], 2));
		t->fdoutred[i] = open(t->file_name,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if (cmd.redirection.red[i] == T_DBL_GREAT)
	{
		if (S_ISDIR(buf.st_mode))
			return (ft_perror_norm_dir(cmd.redirection.file[i], 2));
		t->fdoutred[i] = open(t->file_name,
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else if (cmd.redirection.red[i] == T_LESS)
	{
		t->fdoutred[i] = open(t->file_name, O_RDONLY);
		if (t->fdoutred[i] < 0)
		{
			return (ft_print_error_directory("bash: ",
				cmd.redirection.file[i], ": No such file or directory", 2));
		}
	}
	else if (cmd.redirection.red[i] == T_REDIR_LESSGREAT)
		t->fdoutred[i] = open(t->file_name, O_RDWR);
	return (0);
}

int			expand_filename(t_simp_com cmd, t_exec_redir *t, int i,
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

/*
*** - Aim of the function :
*** - Handle [n]>[word], [n]>>[word], [n]<[word], [n]<>[word]
*/

static int	manage_file(t_simp_com cmd, int i, t_exec_redir *t,
				t_env_tools *env)
{
	struct stat	buf;
	char		pbuf[1024];
	char		*tmp;
	char		*tmp2;

	getcwd(pbuf, sizeof(pbuf));
	if (expand_filename(cmd, t, i, env))
		return (1);
	tmp = ft_strjoin(pbuf, "/");
	tmp2 = ft_strjoin(tmp, t->file_name);
	free(tmp);
	lstat(tmp2, &buf);
	if (manage_file_norm(cmd, i, t, buf))
	{
		free(tmp2);
		return (1);
	}
	if (t->fdoutred[i] < 0)
		if (!S_ISDIR(buf.st_mode) && !ft_free(tmp2))
		{
			return (ft_print_error_directory("bash: ",
				cmd.redirection.file[i], ": Permission Denied", 2));
		}
	if (dup2(t->fdoutred[i], cmd.redirection.fd[i]) < 0 && !ft_free(tmp2))
	{
		return (ft_print_error_directory("bash: ",
				cmd.redirection.file[i], ": Is a directory", 2));
	}
	free(tmp2);
	free(t->file_name);
	return (0);
}

/*
*** - Aim of the function :
*** - Handle Aggregations -> [n]>&[word]
*/

static int	manage_aggreg(t_simp_com cmd, int i, t_exec_redir *t)
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

/*
***	- Aim of the function : to make an array of fds of output
*** - of the proper size (used_size of my redirections)
*** - I am doing so to close them easily afterwards
*** - t->outred, that I memset with the int -1
*** - For the while loop :
*** - I modify all the ionumber to the correct one, they were done,
*** - but not in a format easy enough to manage it properly ->
*** - the number that I shall redirect from
*** - if the command is : ls 5> oui => I shall redirect the fd 5
*** - to the fd of the file oui
*** - if the command is : ls > oui => I shall redirect the fd 1
*** - to the fd of the file oui
*** - if the command is : ls < oui => I shall redirect the fd 0
*** - to the fd of the file oui
*/

int			copy_fds(t_exec_redir *t, t_simp_com *cmd)
{
	int i;

	i = -1;
	if (!(t->fdoutred = (int*)malloc(sizeof(int)
		* cmd->redirection.used_space)))
		return (1);
	ft_memset(t->fdoutred, -1, cmd->redirection.used_space);
	while (++i < cmd->redirection.used_space)
	{
		if (cmd->redirection.fd[i] == 1)
		{
			if (cmd->redirection.red[i] == T_LESS ||
				cmd->redirection.red[i] == T_DBL_LESS ||
					cmd->redirection.red[i] == T_DBL_LESS_DASH
						|| cmd->redirection.red[i] == T_REDIR_LESS)
				cmd->redirection.fd[i] = 0;
			else
				cmd->redirection.fd[i] = 1;
		}
	}
	return (0);
}

/*
*** - Aim of the function :
*** - Clear all the used fds
*/

void		clear_fd(t_exec_redir *t, int end)
{
	int i;

	i = -1;
	while (++i < end)
	{
		if (t->fdoutred[i] > 2)
			close(t->fdoutred[i]);
	}
	free(t->fdoutred);
}

/*
***	- Aim of the function :
*** - Calculate the index corresponding to the last heredoc
*** - Example : cat << oui << non << plus
*** - will return : 3 which equals to "plus"
*/

int			ft_calcul_pos_last_heredoc(t_simp_com cmd)
{
	int		i;
	int		count;

	i = -1;
	count = -1;
	while (++i < cmd.redirection.used_space)
	{
		if (cmd.redirection.red[i] == T_DBL_LESS ||
			cmd.redirection.red[i] == T_DBL_LESS_DASH)
			count++;
	}
	return (count);
}

/*
***	- Aim of the function :
*** - Redirect to the proper redirection
*/

int			process_redirections(t_exec_redir *t, t_simp_com cmd,
				t_env_tools *env)
{
	int		i;
	int		ret;
	int		pos_heredoc;

	i = -1;
	ret = 0;
	copy_fds(t, &cmd);
	pos_heredoc = ft_calcul_pos_last_heredoc(cmd);
	while (++i < cmd.redirection.used_space && ret == 0)
	{
		if (cmd.redirection.red[i] == T_REDIR_LESS
			|| cmd.redirection.red[i] == T_REDIR_GREAT)
			ret = manage_aggreg(cmd, i, t);
		else if ((cmd.redirection.red[i] == T_DBL_LESS
			|| cmd.redirection.red[i] == T_DBL_LESS_DASH))
		{
			if (i == pos_heredoc)
				ret = manage_here_doc(cmd, i, t);
		}
		else
			ret = manage_file(cmd, i, t, env);
	}
	clear_fd(t, cmd.redirection.used_space);
	return (ret);
}
