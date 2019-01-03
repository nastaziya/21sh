#ifndef EXEC_H
# define EXEC_H
# include  	"../libft/libft.h"
# include <fcntl.h>

// struct for the execution of all commands
typedef struct      s_exec_redir
{
    // int         out;
	// // struct stat s;
	// int         saved_stdout;
	// char        **files;
	// int         j;
	// int         test;
    
    // save original states of the fds
    int				fd_orig[3];

    // function check_op
    // int 	    aux;
    // int         f_out;
}                   t_exec_redir;

/*
*** FILE : exec_all_cases.c
*/

int		ft_first_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t);
int		ft_semi_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t);
int		ft_and_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t);
int		ft_or_exec(t_env_tools *env, t_command cmd, int i, t_exec_redir *t);


#endif