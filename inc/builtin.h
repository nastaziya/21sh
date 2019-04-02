/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtin.h                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/02/06 00:06:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/03/22 13:40:50 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H
# include "sh.h"
# include "../libft/libft.h"
# include <unistd.h>

/*
*** - Struct for the norm of the builtin_cd
*** - part of the cd builtin
*/

typedef struct	s_norm_cd
{
	int			begin;
	char		c;
	int			p;
	int			argc;
	int			dash;
}				t_norm_cd;

/*
*** - Struct for the norm of the builtin_env
*** - part of the env builtin
*/

typedef struct	s_norm_env
{
	int			argc;
	char		**cp_c_env;
	int			stop;
	int			ret;
	t_env_tools	*env;
}				t_norm_env;

/*
*** - Second struct for the norm
*** - of the management of PWD AND OLDPWD
*** - we take some parameters from the norm_cd
*** - Mostly used to stay under 4 parameters per function
*/

typedef struct	s_nom_pwd
{
	int			dash;
	char		*tmp;
	char		*tmp2;
	char		buf[1024];
	struct stat	buf2;
	int			i;
	char		*s1;
	char		*s2;
	int			p;
}				t_norm_pwd;

/*
*** - Test builtin Part
*/

/*
*** - Enum for the tok of "-d" :
*** - T_B is for option -b
*** - T_LL is for option -L
*** - 1x "B" --> Lowercase in the option
*** - 2x "L" --> Uppercase in the option 
*/

typedef enum	e_test_tok
{
	T_B,
	T_C,
	T_D,
	T_E,
	T_F,
	T_G,
	T_LL,
	T_P,
	T_R,
	T_SS,
	T_S,
	T_U,
	T_W,
	T_X,
	T_Z,
	T_SEQ,
	T_SNE,
	T_EQ,
	T_NE,
	T_GE,
	T_LT,
	T_LE,
	T_EXCLAM,
	T_WORDS,
	T_QUOTE,
	// T_EMPTY
}				t_test_tok;

/*
*** - Enum for the types :
*** - Not -> "!"
*** - Birany -> "="
*** - Unary -> "-d"
*** - Args -> Word
*/

typedef enum	e_type_test
{
	T_ARGS,
	T_NOT,
	T_BINARY,
	T_UNARY,
	T_QU,
}				t_type_test;

/*
*** - Struct used for the initialization
*** - of the array below
*/

typedef struct		s_args_tok
{
	char				*op;
	int					size;
	t_test_tok			tok;
	t_type_test			type;
}					t_args_tok;

/*
*** - Array used to facilitate the tokenization
*** - for the test command
*/

static const t_args_tok	g_args_tok[] = {
	{"-b", 2, T_B, T_UNARY},
	{"-c", 2, T_C, T_UNARY},
	{"-d", 2, T_D, T_UNARY},
	{"-e", 2, T_E, T_UNARY},
	{"-f", 2, T_F, T_UNARY},
	{"-g", 2, T_G, T_UNARY},
	{"-L", 2, T_LL, T_UNARY},
	{"-p", 2, T_P, T_UNARY},
	{"-r", 2, T_R, T_UNARY},
	{"-S", 2, T_SS, T_UNARY},
	{"-s", 2, T_S, T_UNARY},
	{"-u", 2, T_U, T_UNARY},
	{"-w", 2, T_W, T_UNARY},
	{"-x", 2, T_X, T_UNARY},
	{"-z", 2, T_Z, T_UNARY},
	{"=", 1, T_SEQ, T_BINARY},
	{"!=", 2, T_SNE, T_BINARY},
	{"-eq", 3, T_EQ, T_BINARY},
	{"-ne", 3, T_NE, T_BINARY},
	{"-ge", 3, T_GE, T_BINARY},
	{"-lt", 3, T_LT, T_BINARY},
	{"-le", 3, T_LE, T_BINARY},
	{"!", 1, T_EXCLAM, T_NOT},
	{NULL, 1, 0, 0}
};

/*
*** - End builtin Part
*/

/*
*** - builtin.c
*/

int				ft_usage_error(char *command, char *btwn, char *after, int i);
int				ft_usage_error_env(char *command, char *btwn, char *after,
					int i);
int				ft_int_error(char *str, int fd, int ret);
int				ft_int_print_error(char *str, char *str2, char *str3, int fd);
int				ft_len_array_char(char **av);
int				ft_exec_command(t_env_tools *env, char **cmd, int fork, char **raw_cmd);

/*
*** - Nouvelle feuille
*/
int				ft_usage_is_good(char *limitor, char *str);

/*
*** - unsetenv.c
*/

int				ft_builtin_unsetenv(char **av, char ***c_env, char ***paths,
					t_env_tools *env);

/*
*** - echo.c
*/

int				ft_builtin_echo(char **av);

/*
*** - setenv.c
*/

void			ft_builtin_setenv_2(char *av, char ***c_env, char ***paths,
					t_env_tools *env);
int				ft_builtin_setenv(char **av, char ***c_env, t_env_tools *env);
void			ft_builtin_setenv_env_builtin(char *av, char ***c_env);

/*
*** - cd2.c
*/

int				ft_change_dir_and_pwds(char **av, char ***c_env,
					t_env_tools *env, t_norm_cd *n);

/*
*** - cd.c
*/

int				ft_builtin_cd(char **av, char ***c_env, t_env_tools *env);

/*
*** - cd_utils.c
*/

int				ft_find_path_and_cd2(char c);
int				ft_len_array_char_cd(char **av);
char			*ft_skip_slash(char *s);
int				ft_print_error(char *av, char *str);
int				ft_print_dir_error(char *command, char *btwn, char *after,
					int i);

/*
*** - utils_env_2.c
*/

int				ft_free(char *av);
char			*ft_strdup_without_quotes(char *av);
char			*ft_array_char_to_str(char **c_env, int avoid);
int				ft_free_char_char(char **av);

/*
*** - utils_env.c
*/

void			ft_exchange_chars(char **av, char delimitor, char to_replace);
int				ft_str_is_alpha_setenv(char *str);
int				ft_free_av(char **av);
void			ft_swap_string(char **ret, char **tmp, char **c_env);
void			ft_print_env(char ***c_env);

/*
*** - env.c
*/

int				ft_builtin_env(char **av, char ***c_env, t_env_tools *env);
char			**ft_find_path_and_split(char **c_env);

/*
*** - exit.c
*/

int				ft_builtin_exit(char **cmd, t_env_tools *env);

/*
*** - ctrl_d.c
*/

int				ctrl_d_management(t_tcap *caps);

/*
*** - utils2.c
*/

int				ft_find_path_and_cd(char c, char ***c_env, t_env_tools *env,
					t_norm_cd *n);
void			ft_norm_env(char **av, int i, t_norm_env *t, char ***c_env);
int				ft_builtin_env2(char **av, char **cp_c_env, int i, t_env_tools
					*env);
int				ft_manage_option_i_env(char ***cp_c_env, char **env);
char			*ft_array_char_to_str_replace_env(char **c_env, int avoid,
					char *av);

/*
*** - test.c
*/

int				ft_builtin_test(char **cmd, char **raw_cmd);
int             process_exclamation(int nb, int ret_in);
int             process_unary(t_test_tok tok, char *arg);
int             process_binary(char *str1, t_test_tok tok, char *str2);

/*
*** - utils_test.c
*/

t_args_tok      **initialize_arr_tok(int len);
void            tokenize_test(char **cmd, t_args_tok  ***t);
int				free_tokens(t_args_tok  ***t, int len);
int				ft_len_array_char_test(char **av);

/*
*** - process_2_3args_test.c
*/

int             is_all(t_args_tok *t);
int             is_any(t_args_tok *t);
int             is_s_bin(t_args_tok *t);
int             ft_test_2_args(t_args_tok  **t, int boul);
int             ft_test_3_args(t_args_tok  **t, int boul);

#endif
