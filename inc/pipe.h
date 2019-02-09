/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   pipe.h                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/06 01:00:38 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/06 01:02:19 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef PIPE_H
# define PIPE_H
# include "../libft/libft.h"
# include <sys/types.h>

typedef enum	e_fd_index
{
	READ,
	WRITE,
}				t_fd_index;

typedef struct	s_pipe_struct
{
	int			fds[2];
	t_fd_index	pipe_end;
}				t_pipe_struct;

int				ft_pipe_exec2(t_env_tools *env, t_command cmd, int *i,
					t_pipe_struct *pt, t_exec_redir *t);
int				exec2(char *path, char **str, char **env, int fork_ret);
int				manage_sig_term_ret_1(int ret);

#endif
