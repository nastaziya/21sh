#ifndef EXPANTION_H
# define EXPANTION_H
# include  	"../libft/libft.h"
# include "sh.h"


typedef struct s_dynamic_array
{
    char  *array_expanded;
    int   capacity;
    int   used_size;
}              t_dynamic_array;

void    tab_expansion_init(t_dynamic_array *arr);
void	add_char_to_array(t_dynamic_array *arr, char item);
void    expanded_dynamic_table(char **str,  t_env_tools env, int i);
char	*ft_strdup_without_char(char *str, char c);
int     recover_env_var(char *temp,  t_env_tools env, int *i,  char **val_expended);
int		environement_var_call(char *temp,  t_env_tools env, int i, char **val_exp);
void	dquote(char **str, int *j, t_dynamic_array *final_array, t_env_tools env);
void	manage_sq(char **str,  t_dynamic_array *final_array, int *j, int i);
void	home_var_expand(char **str, int i, int *j, t_dynamic_array *final_array);//, char *home_cpy
void    dollar_expand(char **str, int *j, t_dynamic_array *final_array, t_env_tools env);
void    treat_backslash(char **str, int i, int *j, t_dynamic_array *final_array);
void  add_env_var_to_final_array(t_dynamic_array *array_without_backslash,
char *str);
void	add_return_from_env(char *ret_nr, t_dynamic_array *array_without_backslash,
int *j, int add);
char **expense_cmd(t_command cmd, t_env_tools env, int i);
#endif