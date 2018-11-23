#include "../../inc/sh.h"

void	path_str(char **envs, char ***paths)
{
	int		i;
	char	*str;
	int		j;
	int		k;

	i = 0;
	str = NULL;
	while (envs[i])
	{
		if (ft_strncmp(envs[i], "PATH=", 5) == 0)
		{
			str = ft_strnew(ft_strlen(envs[i]) - 5);
			j = 5;
			k = 0;
			while (j < (int)ft_strlen(envs[i]))
			{
				str[k] = envs[i][j];
				k++;
				j++;
			}
		}
		i++;
	}
	set_path(str, paths);
}


void	check_path_loop(char **path_env, char **path, char **str, int *count)
{
	int		j;
	char	*join_slash;
	char	*join_cmd;

	j = 0;
	while (path_env[j])
	{
		join_slash = ft_strjoin(path_env[j], "/");
		join_cmd = ft_strjoin(join_slash, *str);
		free(join_slash);
		if (access(join_cmd, F_OK) == 0)
		{
			if (*count >= 1)
				free(*path);
			*path = ft_strdup(join_cmd);
			(*count)++;
		}
		j++;
		free(join_cmd);
	}
}

int		check_path(char **path_env, char **path, char **str)
{
	int	count;

	count = 0;
	if (path_env != NULL)
		check_path_loop(path_env, path, str, &count);
	return (count);
}

int		exec(char *path, char **str, char **env, t_env_tools *env_struct)
{
	pid_t	pid;
	int 	status;
	int		res;
	int		ret_signal;

	res = 0;
	if ((pid = fork()))
	{
		if (pid == -1)
			return (-1);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			res = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			res = manage_sig_term_ret_1(WTERMSIG(status), env_struct);
		if (res > 0)
			return(res);
	}
	else
	{
		execve(path, str, env);
		exit(EXIT_FAILURE);
	}
	return (0);
}

int		error_exec_or_exec(char **paths, char *path, char **str,
		t_env_tools *env_struct)
{
	int		res;
	int		i;

	i = 0;
	res = 0;
	if (str[0] && ft_strchr(str[0], '/'))
		path = ft_strdup(str[0]);
	else
		res = check_path(paths, &path, str);
	res = error_exit(env_struct, str, path, res);
	if (path != NULL)
		free(path);
	return (res);
}

