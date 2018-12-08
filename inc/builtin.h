#ifndef BUILTIN_H
# define BUILTIN_H
# include "sh.h"
# include "../libft/libft.h"
# include <unistd.h>

/*
*** - Struct for the norm of the builtin_cd
*** - part of the cd builtin
*/

typedef struct		s_norm_cd
{
    int         begin;
    char        c;
    int         p;
    int         argc;
    int         dash;
}                   t_norm_cd;

/*
*** - Second struct for the norm 
*** - of the management of PWD AND OLDPWD
*** - we take some parameters from the norm_cd
*** - Mostly used to stay under 4 parameters per function
*/

typedef struct      s_nom_pwd
{
    int         dash;
    char	    *tmp;
    char        *tmp2;
	char	    buf[1024];
    struct stat buf2;
    int         i;
    char        *s1;
    char        *s2;
    int         p;
}                   t_norm_pwd;

/*
*** - builtin.c
*/

int		ft_usage_error(char *command, char *btwn, char *after, int i);
int		ft_int_error(char *str, int fd, int ret);
int		ft_int_print_error(char *str, char *str2, char *str3, int fd);
int		ft_len_array_char(char **av);
int		ft_exec_builtin(t_env_tools *env, char **cmd);

/*
*** - unsetenv.c
*/
int		ft_builtin_unsetenv(char **av, char ***c_env, char ***paths, t_env_tools *env);

/*
*** - echo.c
*/

int	    ft_builtin_echo(char **av, char ***c_env);

/*
*** - setenv.c
*/

void		ft_builtin_setenv_2(char *av, char ***c_env, char ***paths, t_env_tools *env);
int		    ft_builtin_setenv(char **av, char ***c_env, char ***paths, t_env_tools *env);

/*
*** - cd2.c
*/

int	    	ft_change_dir_and_pwds(char *av, char ***c_env, t_env_tools *env, t_norm_cd *n);

/*
*** - cd.c
*/

int			ft_builtin_cd(char **av, char ***c_env, t_env_tools *env);

/*
*** - cd_utils.c
*/

int	    ft_find_path_and_cd2(char c);
int		ft_len_array_char_cd(char **av);
char    *ft_skip_slash(char *s);
int		ft_print_error(char *av, char *str);
int		ft_print_dir_error(char *command, char *btwn, char *after, int i);

/*
*** - utils_env_2.c
*/

int		ft_free(char *av);
char	*ft_strdup_without_quotes(char *av);
char	*ft_array_char_to_str(char **c_env, int avoid);

/*
*** - utils_env.c
*/

void	ft_exchange_chars(char **av, char delimitor, char to_replace);
int		ft_str_is_alpha_setenv(char *str);
int     ft_free_av(char **av);
void	ft_swap_string(char **ret, char **tmp, char **c_env);
void	ft_print_env(char ***c_env);

/*
*** - env.c
*/

int			ft_builtin_env(char **av, char ***c_env, char ***paths, t_env_tools *env);
char	    **ft_find_path_and_split(char **c_env);


// void	    path_str(char **envs, char ***paths);


#endif