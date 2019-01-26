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
#include "../../inc/parser.h"

int		pipe_error(void)
{
	ft_putstr_fd("lsh: pipe error: ", 2);
	if (errno == ENFILE)
		ft_putendl_fd("Too many opened files", 2);
	else if (errno == EMFILE)
		ft_putendl_fd("Too many used fds", 2);
	while (wait(NULL) > 0)
		;
	return (1);
}

int		fork_error(void)
{
	write(2, "lsh: fork: ", 11);
	if (errno == EAGAIN)
		ft_putstr_fd("Resource temporarily unavailable", 2);
	ft_putchar('\n');
	while (wait(NULL) > 0)
		;
	return (1);
}

static void	sigerror(char *err, int status)
{
	ft_putstr_fd(err, 2);
	ft_putnbr_fd(status, 2);
	ft_putchar_fd('\n', 2);
}

int			parse_status(int status)
{
	int		ret;

	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == 6)
			sigerror("Abort trap: ", 6);
		else if (WTERMSIG(status) == 8)
			sigerror("Floating point exception: ", 8);
		else if (WTERMSIG(status) == 9)
			sigerror("Killed: ", 9);
		else if (WTERMSIG(status) == 10)
			sigerror("Bus error: ", 10);
		else if (WTERMSIG(status) == 11)
			sigerror("Segmentation fault: ", 11);
		else if (WTERMSIG(status) == 13)
			sigerror("Broken pipe: ", 13);
		ret = status + 128;
	}
	else
		ret = WEXITSTATUS(status);
	return (ret);
}

// properly clear the Pipe fds
void	manage_pipe_fds(t_pipe *p)
{
	if ((*p).input != STDIN_FILENO)
		close((*p).input);
	if ((*p).output != STDOUT_FILENO)
		close((*p).output);
	close((*p).fds[1]); // WRITE_END
	if ((*p).first != STDIN_FILENO)
		close((*p).first);
	(*p).first = (*p).fds[0]; // READ_END
}

static int	pipe_cmd(t_simp_com cmd, t_exec_redir *t, t_env_tools *env)
{
	int ret;

	if (t->p.input != 0) // STDIN_FILENO
		dup2(t->p.input, 0); // STDIN_FILENO
	else
		dup2(t->p.first, 0); // STDIN_FILENO
	if (cmd->next && cmd->next->type == S_CMD && t->p.output == 1) // STDOUT_FILENO
		dup2(t->p.fds[1], 1); // WRITE_END - STDOUT_FILENO
	// expansion cmd
	msh_expand(cmd, shell);
	if ((ret = process_redir(cmd)))
		return (ret);
	close(t->p.fds[0]); // READ_END
	// fonction d'exec
	ret = dispatch_cmd(cmd->cmd, shell, 0);
	if (is_builtin(cmd->cmd))
	{
		free_tokenlst(&shell->tokenlst, 0);
		free_cmds(&shell->cmds);
		ft_exit_shell();
	}
	exit(ret);
}

// static int	fork_and_pipe(t_command cmd, t_env_tools *env, t_exec_redir *t, int i)
// {
// 	t->p.input = 0; // STDIN_FILENO;
// 	t->p.output = 1; // STDOUT_FILENO;
// 	if (pipe(t->p.fds) < 0)
// 		return (pipe_error());
// 	if ((t->pid = fork()) < 0)
// 		return (fork_error());
// 	if (t->pid == 0)// CHILD
// 		pipe_cmd(cmd.command[i], t, env);
// 	manage_pipe_fds(&(t->p));
// 	return (0);
// }

static int	pipeline(t_command cmd, t_env_tools *env, t_exec_redir *t, int *i)
{
	int		status;

	status = 0;
	t->p.first = 0; // STDIN_FILENO
	// tant que ce sont des pipes, j'avance ;
	// mais je dois exécuter la commande avant la dernière
	// => ls | cat -e | oui ; cat -e Makefile ==> doit itérer sur ls, cat -e, oui
	while (*i < cmd.used_space && cmd.command[*i].tok == T_PIPE)
	{
		t->p.input = 0; // STDIN_FILENO;
		t->p.output = 1; // STDOUT_FILENO;
		if (pipe(t->p.fds) < 0)
			return (1); // pipe_error()
		if ((t->pid = fork()) < 0)
			return (fork_error()); // fork_error()
		if (t->pid == 0) // CHILD
			pipe_cmd(cmd.command[*i], t, env);
		manage_pipe_fds(&(t->p));
		(*i)++;
	}
	(*i)++;
	// // après
	t->p.input = 0; // STDIN_FILENO;
	t->p.output = 1; // STDOUT_FILENO;
	if (pipe(t->p.fds) < 0)
		return (pipe_error());
	if ((t->pid = fork()) < 0)
		return (fork_error());
	if (t->pid == 0) // CHILD
		pipe_cmd(cmd.command[*i], t, env);
	manage_pipe_fds(&(t->p));
	// // reprise

	waitpid(t->pid, &status, 0);
	while (wait(NULL) > 0)
		;
	return (parse_status(status));
}

int			ft_pipe_exec(t_env_tools *env, t_command cmd, int *i, t_exec_redir *t)
{
	char 	**cmd_expended;
	int		ret;

	(*i)++;
	// titi fait une copie des fds
	cmd_expended = expense_cmd(cmd, *env, 0);
	copy_fds(t, &cmd);
	
	ret = pipeline(cmd, env, t, i);
	
	// passer au suivant, sûrement avancer le i
	// forward_pipes(cmds);
	
	clear_fd(t, cmd.command[*i].redirection.used_space);
	free_str(cmd_expended);
	return (ret);
}