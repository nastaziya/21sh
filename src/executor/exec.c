#include "../../inc/sh.h"

void	set_path(char *str, char ***paths)
{
	if (str != NULL)
	{
		*paths = ft_strsplit(str, ':');
		free(str);
	}
	else
		*paths = NULL;
}

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


int	manage_sig_term_ret_1(int ret, t_env_tools *env_struct)
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
 		new_ret = manage_sig_term_ret_2(ret, env_struct);
	return (new_ret);
}

int	manage_sig_term_ret_2(int ret, t_env_tools *env_struct)
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

void print_errors(char *error, char *file_name)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(file_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(error, 2);

}

int		error_exit(t_env_tools *env_struct, char **str, char *path, int res)
{
	struct stat s;

	if (lstat(str[0], &s) != -1 && S_ISDIR(s.st_mode))
	{
		print_errors("is a directory", str[0]);
		return (126);
	}
	else if (lstat(str[0], &s) == -1 && str[0] && ft_strchr(str[0], '/'))
	{
		print_errors("No such file or directory", str[0]);
		return (127);
	}
	else if (lstat(str[0], &s) == -1 && res == 0)
	{
		print_errors("command not found", str[0]);
		return (127);
	}
	else if(access(path, X_OK) == -1)
	{
		print_errors("Permission denied", str[0]);
		return (126);
	}	
	else
		return (exec(path, str, env_struct->env_cpy, env_struct));
	return(0);
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
	/*if (lstat(str[0], &s) != -1 && S_ISDIR(s.st_mode))
	{
		print_errors("is a directory", str[0]);
		res = 126;
	}
	else if (lstat(str[0], &s) == -1 && str[0] && ft_strchr(str[0], '/'))
	{
		res = 127;
		print_errors("No such file or directory", str[0]);
	}
	else if (lstat(str[0], &s) == -1 && res == 0)
	{
		res = 127;
		print_errors("command not found", str[0]);
	}
	else if(access(path, X_OK) == -1)
	{
		res = 126;
		print_errors("Permission denied", str[0]);
	}	
	else
		res = exec(path, str, env_struct->env_cpy, env_struct);*/
	if (path != NULL)
		free(path);
	return (res);
}

