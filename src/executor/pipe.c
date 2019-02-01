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

// int		pipe_error(void)
// {
// 	ft_putstr_fd("lsh: pipe error: ", 2);
// 	if (errno == ENFILE)
// 		ft_putendl_fd("Too many opened files", 2);
// 	else if (errno == EMFILE)
// 		ft_putendl_fd("Too many used fds", 2);
// 	while (wait(NULL) > 0)
// 		;
// 	return (1);
// }

int		fork_error(void)
{
	write(2, "bash: fork: ", 11);
	// if (errno == EAGAIN)
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

int			exit_status(int status)
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

static int	pipe_cmd(t_command cmd, t_exec_redir *t, t_env_tools *env, int i)
{
	int 	ret;
	char 	**cmd_expended;

	if (t->p.input != 0) // STDIN_FILENO
		dup2(t->p.input, 0); // STDIN_FILENO
	else
		dup2(t->p.first, 0); // STDIN_FILENO
	if (t->p.output == 1) // STDOUT_FILENO
		dup2(t->p.fds[1], 1); // WRITE_END - STDOUT_FILENO
	// expansion cmd
	cmd_expended = expense_cmd(cmd, *env, i);
	// msh_expand(cmd, shell);
	if ((ret = process_redirections(t,cmd.command[i])))
		return (ret);
	close(t->p.fds[0]); // READ_END
	// fonction d'exec
	ret = ft_exec_command(env, cmd_expended);
    free_str(cmd_expended);
	exit(ret);
}

static int	pipeline(t_command cmd, t_env_tools *env, t_exec_redir *t, int *i)
{
	int		status;

	status = 0;
	t->p.first = 0; // STDIN_FILENO
	// tant que ce sont des pipes, j'avance ;
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
			pipe_cmd(cmd, t, env, *i);
		manage_pipe_fds(&(t->p));
		(*i)++;
	}
	// Exécution de la dernière commande après le dernier pipe
	// 
	t->p.input = 0; // STDIN_FILENO;
	t->p.output = 1; // STDOUT_FILENO;
	if (pipe(t->p.fds) < 0)
		return (1); // return (pipe_error());
	if ((t->pid = fork()) < 0)
		return (fork_error());
	if (t->pid == 0) // CHILD
		pipe_cmd(cmd, t, env, *i);
	manage_pipe_fds(&(t->p));
	// 
	// Attente des PIDs
	// 
	// on récupère le status du dernier pid exécuté, pour avoir le code retour
	waitpid(t->pid, &status, 0);
	// on attend tous les processus ouverts
	while (wait(NULL) > 0)
		;
	return (exit_status(status));
}

int			ft_pipe_exec(t_env_tools *env, t_command cmd, int *i, t_exec_redir *t)
{
	// incrémentation non possible dans l'appel de la fonction car on envoie son adresse,
	// ainsi, je le fais ici
	(*i)++;
	// copie des fds
	copy_fds(t, &cmd.command[*i]);
	// Exécution de la fonction Pipe
	env->g_return_value = pipeline(cmd, env, t, i);
	// Voir si le i est bon ou mauvais apès l'itération sur les pipes
	(*i)--; // car dans la boucle principale, on joue avec des i + 1
	// Clear les fds correspondants -> Tout remettre en place
	clear_fd(t, cmd.command[*i].redirection.used_space);
	// Clear la struct t.p -> Tout remettre à 0

	return (0);
}