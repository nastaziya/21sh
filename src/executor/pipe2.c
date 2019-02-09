#include "../../inc/sh.h"
#include "../../inc/exec.h"
#include "../../inc/builtin.h"
#include "../../inc/parser.h"
#include "../../inc/pipe.h"
#include <stdio.h>
#include <stdlib.h>


int			exec2(char *path, char **str, char **env, int fork_ret)
{
	int 	status;
	int		res;

	res = 0;
	if (fork_ret)
	{
		waitpid(fork_ret, &status, 0);
		if (WIFEXITED(status))
			res = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			res = manage_sig_term_ret_1(WTERMSIG(status));
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

static char		*return_path(char **paths, char **str)
{
	char *path;

	if (str[0] && ft_strchr(str[0], '/'))
	{
		path = ft_strdup(str[0]);
	}
	else
		check_path(paths, &path, str);
	return (path);
}
int				ret_nr_pipe(t_command cmd)
{
	int i;
	int nr_pipe;

	i = 0;
	nr_pipe = 0;
	while(i < cmd.used_space)
	{
		if (cmd.command[i].tok == T_PIPE)
			nr_pipe++;
		i++;
	}
	return(nr_pipe + 1);

}

int			ft_pipe_exec2(t_env_tools *env, t_command cmd, int *i, t_pipe_struct *pt, t_exec_redir *t)
{
	char	**command;
	char	*path;
	int		len_pipe = ret_nr_pipe(cmd);
	int		p[2];
	int		fd_in = 0;
	int		status;
	int		ret = 0;
	int		aux = 0;

    pid_t pid;
    (void)pt;
    while (aux < len_pipe)
    {
       
        command = expense_cmd(cmd, *env, *i);
        path = return_path(env->paths, command);//, env->env_cpy
        pipe(p);
        pid = fork();
        if (pid == -1)
        {
          exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            dup2(fd_in, 0);
            if (aux + 1 != len_pipe)
                dup2(p[1], 1);
            if ((ret = process_redirections(t, cmd.command[*i], env)))
		        return (ret);
            close(p[0]);
            env->g_return_value = ft_exec_command(env, command, pid);
            if (is_built_in(command) || (env->g_return_value != 0))
                exit(EXIT_FAILURE);
            //exec2(path,command,env->env_cpy, pid);
        }
       
            close(p[1]);
            if(fd_in != 0)
                close(fd_in);
            fd_in = p[0];
            (*i)++;
            aux++;
            if (aux == len_pipe)
                (*i)--;   
       // }
          //  free(path);
           // free(command);
    }
    waitpid(pid, &status, 0);
    while (wait(NULL) > 0)
		;
   return(manage_sig_term_ret_1(status)); 
}
