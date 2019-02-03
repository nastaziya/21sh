#include <sys/types.h>

typedef enum fd_pipe{
     READ, WRITE
}   fd_index;

typedef struct  s_pipe_struct
{
    int fds[2];
    fd_index  pipe_end;
}               t_pipe_struct;

int			ft_pipe_exec2(t_env_tools *env, t_command cmd, int *i, t_pipe_struct *pt);
int		exec2(char *path, char **str, char **env, int fork_ret);
int	    manage_sig_term_ret_1(int ret);


