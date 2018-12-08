#include "../../inc/sh.h"
#include "../../inc/builtin.h"

/*
*** - Initial copy of the home environment
*** - same management as the PATH (works with copies)
*/

// void	cpy_home(t_env_tools *env)
// {
// 	env->home = ft_strdup(getenv("HOME"));
// }

// void	set_path(char *str, char ***paths)
// {
// 	if (str != NULL)
// 	{
// 		*paths = ft_strsplit(str, ':');
// 		free(str);
// 	}
// 	else
// 		*paths = NULL;
// }

void	path_str(char **envs, char ***paths)
{
	// int		i;
	// char	*str;
	// int		j;
	// int		k;
	// char **res;

	// i = -1;
	*paths = ft_find_path_and_split(envs);
	// *paths = res;
	// str = NULL;
	// while (envs[i])
	// {
	// 	if (ft_strncmp(envs[i], "PATH=", 5) == 0)
	// 	{
	// 		str = ft_strnew(ft_strlen(envs[i]) - 5);
	// 		j = 5;
	// 		k = 0;
	// 		while (j < (int)ft_strlen(envs[i]))
	// 		{
	// 			str[k] = envs[i][j];
	// 			k++;
	// 			j++;
	// 		}
	// 	}
	// 	i++;
	// }
	// dprintf(2, "str_path: |%s|\n", str);
	// set_path(str, paths);
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
	if (!(*path))
		*path = ft_strdup(*str);
}

int		check_path(char **path_env, char **path, char **str)
{
	int	count;

	count = 0;
	if (path_env != NULL)
		check_path_loop(path_env, path, str, &count);
	return (count);
}

void	error_command(char *part, char **str, char *str2)
{
	ft_putstr_fd(part, 2);
	ft_putstr_fd(*str, 2);
	ft_putendl_fd(str2, 2);
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
	/*
		if (execve(path, str, env) == -1)
			exit(EXIT_FAILURE);
		exit(EXIT_FAILURE);*/
	}
	return (0);
}

// str == command
// char *path -> error_exec_or_exec
// int		error_exec_or_exec(char **paths, char *path, char **str,
// 		char **env)
int		error_exec_or_exec(char **paths, char **str,
		char **env, int in_env) // insérer booléen précisant si dans env ou non
{
	int		res;
	int		i;
	char	*path;

	i = 0;
	res = 0;
	path = NULL;
	if (str[0] && ft_strchr(str[0], '/'))
	{
		path = ft_strdup(str[0]);
		if (access(path, F_OK) == 0)
			i = 1;
	}
	else
		res = check_path(paths, &path, str);
	/////////
	// int j = -1;
	// while (paths[++j])
	// 	dprintf(2, "loop: |%s|\n", paths[j]);
	// dprintf(2, "path: |%s - %d|\n", path, res);
	// // //////
	struct stat buf;
	dprintf(2, "|%s|\n", path);
	dprintf(2, "stat: %d - %d - %d - %d - %d - %d\n", res, stat(path, &buf), S_ISDIR(buf.st_mode), access(path, F_OK), access(path, X_OK), in_env);
	/// Quand dossier est chmod 000 -> sort erreur de l'error command
	// Quand executable est chmod 000 -> execute quand même alors que devrait sortir erreur error command
	// dprintf()
	if (in_env != 1 && res == 0 && i == 0 && (access(path, X_OK) == -1) && ft_strncmp(str[0], "./", 2) && str[0][0] != '/')// faire en sorte que cette erreur ne s'affiche pas quand env
	// FAUX QUAND dossier est inaccessible !! devrait être permission denied
	{
		dprintf(2, "1er\n");
		res = 127;
		error_command("bash: ", str, ": command not found");
	} // ((stat(path, &buf) >= 0) && (buf.st_mode > 0) && (S_IEXEC & buf.st_mode)) //(in_env == 1 ? lstat(ft_strsub(path, 0, ft_strrchr(path, '/') - path), &buf) == -1 :
	else if (res == 0 && i == 0 && access(path, X_OK) == -1 && !ft_strncmp(str[0], "./", 2))// faire en sorte que cette erreur ne s'affiche pas quand env
	// FAUX QUAND dossier est inaccessible !! devrait être permission denied
	{
		dprintf(2, "2eme\n");
		res = 127;
		in_env != 1 ? error_command("bash: ", str, ": No such file or directory") : error_command("env: ", str, ": No such file or directory");
	}
	else if ((res > 1 && access(path, X_OK) == 0) || ((res == 1 || res == 0) && access(path, X_OK) == -1))// && S_ISDIR(buf.st_mode) // || (res == 0 && stat(path, &buf) == -1) || (res == 1 && stat(path, &buf) == -1)
	{
		res = 126;
			dprintf(2, "3eme %d - %d - %d - %d\n", access(path, F_OK), access(path, R_OK), access(path, X_OK), lstat(path, &buf));
		dprintf(2, "3eme\n");
		if (in_env != 1)
			error_command("bash: ", str, ": permission denied");
		else if (in_env == 1 && ft_strchr(path, '/') ? !access(ft_strsub(path, 0, ft_strrchr(path, '/') - path), X_OK) && access(path, X_OK) : 1 == 2)
		{
			error_command("env: ", str, ": permission denied");
			dprintf(2, "YES JAI TROUVE\n");
		}
		else if (in_env == 1 && ft_strchr(path, '/') ? access(ft_strsub(path, 0, ft_strrchr(path, '/') - path), X_OK) : 1 == 2)
		{
			dprintf(2, "YES JAI TROUVE- le 2eme\n");
			// dprintf(2, "DEBUG: %d - %d\n", access(path, X_OK), lstat(path, &buf));
			error_command("env: ", str, ": permission denied");
		}
		else
		{
			error_command("env: ", str, ": No such file or directory");
			res = 127; //(in_env != 1 ? 1 : XX)
		}
	}
	else
	{
		dprintf(2, "4eme\n");
		if ((res = exec(path, str, env) == 1))
		{
			in_env == 1 ? error_command("env: ", str, ": permission denied") : error_command("bash: ", str, ": is a directory");
			res = 126;
		}
	}
	if (path != NULL)
		free(path);
	dprintf(2, "return res: %d\n", res);
	return (res);
}
