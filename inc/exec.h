/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   exec.h                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/06 00:12:59 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/06 00:13:01 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H
# include "../libft/libft.h"
# include "parser.h"
# include <fcntl.h>

typedef struct	s_pipe
{
	int			fds[2];
	int			first;
	int			input;
	int			output;
}				t_pipe;

/*
*** - struct for the execution of all commands
*/

typedef struct	s_exec_redir
{
	int			fd_orig[3];
	int			*fdoutred;
	char		***heredoc;
	int			i_hdoc;
	t_pipe		p;
	pid_t		pid;
	char		*file_name;
}				t_exec_redir;

/*
*** - FILE : exec_all_cases.c
*/

int				ft_first_exec(t_env_tools *env, t_command cmd, int i,
					t_exec_redir *t);
int				ft_semi_exec(t_env_tools *env, t_command cmd, int i,
					t_exec_redir *t);
int				ft_and_exec(t_env_tools *env, t_command cmd, int i,
					t_exec_redir *t);
int				ft_or_exec(t_env_tools *env, t_command cmd, int i,
					t_exec_redir *t);
void			save_original_fd(t_exec_redir *t);
void			restore_original_fd(t_exec_redir *t);

/*
*** - FILE : redirections.c
*/

int				process_redirections(t_exec_redir *t, t_simp_com cmd,
					t_env_tools *env);
int				ft_pipe_exec(t_env_tools *env, t_command cmd, int *i,
					t_exec_redir *t);
void			clear_fd(t_exec_redir *t, int end);
int				copy_fds(t_exec_redir *t, t_simp_com *cmd);

#endif
