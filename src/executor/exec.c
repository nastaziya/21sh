/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/24 16:36:16 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include "../../inc/expansion.h"
#include "../../inc/exec.h"

void	norm_exec(pid_t *pid, int fork_val)
{
	if (fork_val != 0)
		*pid = fork();
	else
		*pid = 0;
}

int		right_return(int status)
{
	int res;

	res = 0;
	if (WIFEXITED(status))
		res = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		res = manage_sig_term_ret_1(WTERMSIG(status));
	return (res);
}

int		exec(char *path, char **str, char **env, int fork_val)
{
	pid_t	pid;
	int		status;
	int		res;

	res = 0;
	norm_exec(&pid, fork_val);
	if (pid)
	{
		if (pid == -1)
			return (-1);
		waitpid(pid, &status, 0);
		res = right_return(status);
		if (res > 0)
			return (res);
	}
	else
	{
		execve(path, str, env);
		exit(EXIT_FAILURE);
	}
	return (0);
}

/*
*** - Place it somewhere else for the norm
*/

int		check_errors_exec(char *path, char **str, int in_env)
{
	int			res;
	struct stat	buf;

	res = 0;
	if (path == NULL)
		res = error_command(in_env == 2 ? "env: " : "bash: ", str,
			": command not found", 127);
	else if (access(path, F_OK) == -1)
			res = error_command(in_env == 2 ? "env: " : "bash: ", str,
			": No such file or directory", 127);
	else if (!(stat(path, &buf) == -1) && (buf.st_mode & S_IFMT) == S_IFDIR)
		res = error_command(in_env == 2 ? "env: " : "bash: ",
			str, ": is a directory", 126);
	else if ((stat(path, &buf) == 0 && (buf.st_mode & S_IXUSR) == 0))
		res = error_command(in_env == 2 ? "env: "
			: "bash: ", str, ": permission denied", 126);
	return (res);
}

int		error_exec_or_exec(char **paths, char **str,
			char **env, int in_env)
{
	int			res;
	char		*path;

	path = NULL;
	res = 0;
	if (str[0] && ft_strchr(str[0], '/'))
		path = ft_strdup(str[0]);
	else
		check_path(paths, &path, str);
	if ((res = check_errors_exec(path, str, in_env)))
		;
	else
		res = exec(path, str, env, in_env);
	if (path != NULL)
		free(path);
	return (res);
}
