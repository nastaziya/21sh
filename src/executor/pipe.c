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

// properly clear the Pipe fds
void	manage_pipe_fds(t_pipe *p)
{
	if ((*p).input != STDIN_FILENO)
		close((*p).input);
	if ((*p).output != STDOUT_FILENO)
		close((*p).output);
	close((*p).fds[WRITE_END]);
	if ((*p).first != STDIN_FILENO)
		close((*p).first);
	(*p).first = (*p).fds[READ_END];
}

static int	pipe_cmd(t_cmds *cmd, t_pipe p, t_shell *shell)
{
	int ret;

	if (p.input != STDIN_FILENO)
		dup2(p.input, STDIN_FILENO);
	else
		dup2(p.first, STDIN_FILENO);
	if (cmd->next && cmd->next->type == S_CMD && p.output == STDOUT_FILENO)
		dup2(p.fds[WRITE_END], STDOUT_FILENO);
	msh_expand(cmd, shell);
	if ((ret = process_redir(cmd)))
		return (ret);
	close(p.fds[READ_END]);
	ret = dispatch_cmd(cmd->cmd, shell, 0);
	if (is_builtin(cmd->cmd))
	{
		free_tokenlst(&shell->tokenlst, 0);
		free_cmds(&shell->cmds);
		ft_exit_shell();
	}
	exit(ret);
}

static int	pipeline(t_cmds *cmds, t_shell *shell)
{
	t_pipe	p;
	pid_t	pid;
	int		status;

	status = 0;
	p.first = STDIN_FILENO;
	while (cmds && cmds->type == S_CMD)
	{
		p.input = STDIN_FILENO;
		p.output = STDOUT_FILENO;
		if (pipe(p.fds) < 0)
			return (pipe_error());
		if ((pid = fork()) < 0)
			return (fork_error());
		if (pid == CHILD)
			pipe_cmd(cmds, p, shell);
		manage_pipe_fds(&p);
		cmds = cmds->next;
	}
	waitpid(pid, &status, 0);
	while (wait(NULL) > 0)
		;
	return (parse_status(status));
}

int			ft_pipe_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t)
{
	char 	**cmd_expended;
	int		ret;

	// titi fait une copie des fds
	cmd_expended = expense_cmd(cmd, *env, 0);
	copy_fds(t, &cmd);
	
	ret = pipeline((*cmds), shell);
	
	// passer au suivant, sûrement avancer le i
	forward_pipes(cmds);
	
	clear_fd(t, cmd.command[i].redirection.used_space);
	free_str(cmd_expended);
	return (ret);
}