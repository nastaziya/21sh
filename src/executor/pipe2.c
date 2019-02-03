#include "../../inc/sh.h"
#include "../../inc/exec.h"
#include "../../inc/builtin.h"
#include "../../inc/parser.h"
#include "../../inc/pipe.h"
#include <stdio.h>
#include <stdlib.h>


int		exec2(char *path, char **str, char **env, int fork_ret)
{
	pid_t	pid;
	int 	status;
	int		res;
	// int		ret_signal;

	res = 0;
	if (fork_ret)
	{
		//if (pid == -1)
		//	return (-1);
		waitpid(fork_ret, &status, 0);
        
		// res = WEXITSTATUS(status);
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
static char	*return_path(char **paths, char **str,
		char **env)
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
int ret_nr_pipe(t_command cmd)
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

int			ft_pipe_exec2(t_env_tools *env, t_command cmd, int *i, t_pipe_struct *pt)
{
    char **command;
    char *path;
    int len_pipe = ret_nr_pipe(cmd);
     int   p[2];
   
     int   fd_in = 0;
    	int 	status;
    int ret = 0;
    int aux = 0;

  
 //cat < toto | ew -l seg fault
    pid_t pid;
    while (aux < len_pipe)
    {
       
        command = expense_cmd(cmd, *env, *i);
        path = return_path(env->paths, command, env->env_cpy);
        pipe(p);
        if ((pid = fork()) == -1)
        {
          exit(EXIT_FAILURE);
        }
        if (pid == 0)
        {
            dup2(fd_in, 0);
            if (aux + 1 != len_pipe)
                dup2(p[1], 1);
            if (cmd.command[*i].redirection.used_space )
            {
                if ((ret = process_redirections(cmd.command[*i])))
		            return (ret);
            }
            close(p[0]);
            env->g_return_value = exec2(path,command,env->env_cpy, pid);
        }
       // else
        //{
            close(p[1]);
            if(fd_in != 0)
                close(fd_in);
            fd_in = p[0];
            (*i)++;
            aux++;
            if (aux == len_pipe)
            { 
                (*i)--;   
            }
       // }
          //  free(path);
           // free(command);
    }
    waitpid(pid, &status, 0);
  //  wait(NULL);
    while (wait(NULL) > 0)
		;
    
    
    
   return(0); 
}