#include "../inc/sh.h"

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

void	error_command(char **str)
{
	ft_putstr_fd(*str, 2);
	ft_putstr_fd(": command not found", 2);
	ft_putchar('\n');
}

int		exec(char *path, char **str, char **env)
{
	pid_t	pid;
	int 	status;
	int		res;

	res = 0;
	if ((pid = fork()))
	{
		if (pid == -1)
			return (-1);
		waitpid(pid, &status, 0);
		res = WEXITSTATUS(status);	
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
		char **env)
{
	int		res;
	int		i;
	//int 	verif;

	i = 0;
	//verif = 0;
	res = 0;
	if (ft_strchr(str[0], '/'))
	{
		path = ft_strdup(str[0]);
		if (access(path, F_OK) == 0)
			i = 1;
	}
	else
		res = check_path(paths, &path, str);
	if (res == 0 && i == 0)
		error_command(str);
	else if (res > 1)
		ft_putendl_fd("minishell : permission denied.", 2);
	else
	{
	//	g_sig_check = 1;
		res = exec(path, str, env);
	}
	//g_sig_check = 0;
	if (path != NULL)
		free(path);
	return (res);
}

