/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/09/07 17:58:18 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/exec.h"
#include "../../inc/builtin.h"

// /*
// **	Handle [n]<<[delimiter]
// */

// static int	handle_here_doc(t_redir *redir)
// {
// 	int		ret;
// 	int		fd[2];

// 	if (pipe(fd) < 0)
// 		return (pipe_error());
// 	if (redir->hdoc.here_doc)
// 		write(fd[WRITE_END], redir->hdoc.here_doc,
// 			ft_strlen(redir->hdoc.here_doc));
// 	close(fd[WRITE_END]);
// 	ret = dup2(fd[READ_END], redir->io_nbr);
// 	close(fd[READ_END]);
// 	if (ret < 0)
// 		return (fd_error(redir->filename, 0));
// 	return (0);
// }

int         ft_print_error_directory(char *str, char *str1, char *str2,
                int fd)
{
    ft_putstr_fd(str, fd);
    ft_putstr_fd(str1, fd);
    ft_putendl_fd(str2, fd);
    return (1);
}

int         ft_perror_norm_dir(char *str1, int fd)
{
    ft_putstr_fd("bash: ", fd);
    ft_putstr_fd(str1, fd);
    ft_putendl_fd(": Is a directory", fd);
    return (1);
}

static int  manage_file_norm(t_simp_com cmd, int i, t_exec_redir *t,
                struct stat buf)
{
    if (cmd.redirection.red[i] == T_GREAT)
	{
		if (S_ISDIR(buf.st_mode))
		    return (ft_perror_norm_dir(cmd.redirection.file[i], 2));
		t->fdoutred[i] = open(cmd.redirection.file[i],
            O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	else if (cmd.redirection.red[i] == T_DBL_GREAT)
	{
		if (S_ISDIR(buf.st_mode))
		    return (ft_perror_norm_dir(cmd.redirection.file[i], 2));
		t->fdoutred[i] = open(cmd.redirection.file[i],
            O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else if (cmd.redirection.red[i] == T_LESS)
	{
        t->fdoutred[i] = open(cmd.redirection.file[i], O_RDONLY);
        if (t->fdoutred[i] < 0)
            return (ft_print_error_directory("bash: ",
                cmd.redirection.file[i], ": No such file or directory", 2));
    }
	else if (cmd.redirection.red[i] == T_REDIR_LESSGREAT)
		t->fdoutred[i] = open(cmd.redirection.file[i], O_RDWR);
    return (0);
}

/*
**	Handle [n]>[word], [n]>>[word], [n]<[word], [n]<>[word]
*/

static int	manage_file(t_simp_com cmd, int i, t_exec_redir *t)
{
    struct stat buf;
    char        pbuf[1024];
    char        *tmp;
    char        *tmp2;

    getcwd(pbuf, sizeof(pbuf));
    tmp = ft_strjoin(pbuf, "/");
    tmp2 = ft_strjoin(tmp, cmd.redirection.file[i]);
    free(tmp);
    lstat(tmp2, &buf);
	if (manage_file_norm(cmd, i, t, buf))
    {
        free(tmp2);
        return (1);
    }
	if (t->fdoutred[i] < 0)
        if (!S_ISDIR(buf.st_mode)&& !ft_free(tmp2))
            return (ft_print_error_directory("bash: ",
                cmd.redirection.file[i], ": Permission Denied", 2));
	if (dup2(t->fdoutred[i], cmd.redirection.fd[i]) < 0 && !ft_free(tmp2))
	    return (ft_print_error_directory("bash: ",
                cmd.redirection.file[i], ": Is a directory", 2));
    free(tmp2);
	return (0);
}

// /*
// **	Handle [n]>&[word]
// */

// static int	handle_aggreg(t_redir *redir)
// {
// 	if (redir->filename[0] == '-')
// 	{
// 		close(redir->io_nbr);
// 		return (0);
// 	}
// 	else if (redir->filename[0] && !ft_isdigit(redir->filename[0]))
// 		return (fd_error(redir->filename, 1));
// 	else
// 		redir->fd = ft_atoi(redir->filename);
// 	if ((dup2(redir->fd, redir->io_nbr)) < 0)
// 		return (fd_error(redir->filename, 0));
// 	return (0);
// }

/*
***	- Aim of the function : to make an array of fds of output
*** - of the proper size (used_size of my redirections)
*** - I am doing so to close them easily afterwards
*** - t->outred, that I memset with the int -1
*** - 
*** - For the while loop :
*** - I modify all the ionumber to the correct one, they were done, but not in a format
*** - easy enough to manage it properly -> the number that I shall redirect from
*** - if the command is : ls 5> oui => I shall redirect the fd 5 to the fd of the file oui
*** - if the command is : ls > oui => I shall redirect the fd 1 to the fd of the file oui
*** - if the command is : ls < oui => I shall redirect the fd 0 to the fd of the file oui
*/

int			copy_fds(t_exec_redir *t, t_simp_com *cmd)
{
    int i;

    i = -1;
    // tableau d'int qui va contenir les fds créés
    if (!(t->fdoutred = (int*)malloc(sizeof(int)
        * cmd->redirection.used_space)))
        return (1);
    // Que j'initialise à -1, car si ça plante après, les dup2 pèteront avec les -1
    ft_memset(t->fdoutred, -1, cmd->redirection.used_space);
    while (++i < cmd->redirection.used_space)
    {
        dprintf(3, "cmd.redirection.fd[i]: %d\n", cmd->redirection.fd[i]);
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

void	clear_fd(t_exec_redir *t, int end)
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

int     ft_calcul_pos_last_heredoc(t_simp_com cmd)
{
    int     i;
    int     count;

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
***	- Aim of the function : to redirect to the proper redirection
*/
// cmd.redirection.fd => stocke les anciens fds, avec 1 quand

int			process_redirections(t_exec_redir *t, t_simp_com cmd)
{
    int     i;
	int		ret;
    int     pos_heredoc;
    // int     fd;

	i = -1;
    ret = 0;
    // fd = -1;
    // dprintf(1, "cmd.redirection.used_space: %d\n", cmd.redirection.used_space);
    // 1. Copier tous les fds
	copy_fds(t, &cmd);
    pos_heredoc = ft_calcul_pos_last_heredoc(cmd);
    dprintf(3, "POS_HEREDOC: %d\n", pos_heredoc);
    while (++i < cmd.redirection.used_space && ret == 0)
	{
		if (cmd.redirection.red[i] == T_REDIR_LESS
            || cmd.redirection.red[i] == T_REDIR_GREAT)
			// ret = handle_aggreg(redir);
            ;
		else if (cmd.redirection.red[i] == T_DBL_LESS
            || cmd.redirection.red[i] == T_DBL_LESS_DASH)
			// ret = handle_here_doc(redir);
            ;
		else
			ret = manage_file(cmd, i, t);
	}
    // clear et close tous les fds && free ce que j'ai malloc (fdoutred)
    // dans copy_fds
    clear_fd(t, cmd.redirection.used_space);
    // loop here to do the proper redirections
	return (ret);
}