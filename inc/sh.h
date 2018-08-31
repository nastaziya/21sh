#ifndef SH_H
# define SH_H

# include   "lexer.h"
# include   "parser.h"
# include	<unistd.h>
# include	<sys/wait.h>
# include	<sys/types.h>
# include  	"../libft/libft.h"
# include	<stdlib.h>
# include	<stdio.h>
# include	<string.h>
# include	<term.h>

typedef struct		s_env_tools
{
	char			**env_cpy;
	char			**paths;
}					t_env_tools;

int		size_str(char **environ);
char	**copy_env(char **environ, int size);
void	set_path(char *str, char ***paths);
void	path_str(char **envs, char ***paths);
int		exec(char *path, char **str, char **env);
int		error_exec_or_exec(char **paths, char *path, char **str, char **env);
void	free_str(char **str);

#endif
