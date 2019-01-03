#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include "../../inc/expansion.h"
#include "../../inc/exec.h"

void	save_fd_original_state(t_exec_redir *t)
{
	int i;

	i = 0;
	while (i < 3)
	{
		t->fd_orig[i] = dup(i);
		i++;
	}
}

void	restore_fd_original_state(t_exec_redir *t)
{
	int		i;

	i = 0;
	while (i < 3)
	{
		dup2(t->fd_orig[i], i);
		close(t->fd_orig[i]);
		i++;
	}
}

void	rebuild_fd(t_exec_redir *t)
{
    // rebuild_fd((*cmd)->redirlst);
	t_redir	*redir;

	redir = redirlst;
	while (redir)
	{
		if (redir->fd > 2)
			close(redir->fd);
		redir = redir->next;
	}
}

int		prerun_cmd(t_command cmd, t_exec_redir *t)
{
	int	ret;

	save_fd_original_state(t);
	ret = process_redir(cmd);
	return (ret);
}

int		ft_or_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t)
{
    char 	**cmd_expended;
    
    cmd_expended = expense_cmd(cmd, *env, i + 1);
    if (env->g_return_value > 0)
    {
        if (cmd.command[i + 1].redirection.used_space > 0)
        
        env->g_return_value = ft_exec_command(env, cmd_expended);
    }
    free_str(cmd_expended);
    //
    restore_fd_original_state(t);
	rebuild_fd((*cmd)->redirlst);
    return (0);
}

int		ft_and_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t)
{
    char 	**cmd_expended;
    
    cmd_expended = expense_cmd(cmd, *env, i + 1);
    if (env->g_return_value == 0)
    {
        if (cmd.command[i + 1].redirection.used_space > 0)
        
        env->g_return_value = ft_exec_command(env, cmd_expended);
    }
    free_str(cmd_expended);
    //
    restore_fd_original_state(t);
	rebuild_fd((*cmd)->redirlst);
    return (0);
}

int		ft_semi_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t)
{
    char 	**cmd_expended;
    
    cmd_expended = expense_cmd(cmd, *env, i + 1);
    if (cmd.command[i + 1].redirection.used_space > 0)
        
    env->g_return_value = ft_exec_command(env, cmd_expended);
    free_str(cmd_expended);
    //
    restore_fd_original_state(t);
	rebuild_fd((*cmd)->redirlst);
    return (0);
}

int		ft_first_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t)
{
    char 	**cmd_expended;
    
    if (cmd.used_space > 0)
		cmd_expended = expense_cmd(cmd, *env, 0);    
    //EXECUTION DE LA PREMIERE COMMANDE

	// premiere redirections
	if (cmd.command[0].redirection.used_space > 0)

	// execution de la premiere commande
	if (cmd_expended != NULL && cmd.used_space > 0)
		env->g_return_value = ft_exec_command(env, cmd_expended);
	free_str(cmd_expended);    
    restore_fd_original_state(t);
	rebuild_fd((*cmd)->redirlst);
    return (0);
}