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
void		add_char_to_array(t_dynamic_array *arr, char item);
void modif_backslash_str(char **str,  t_env_tools env, int i);
char	*ft_strdup_without_char(char *str, char c);
void	join_text_between_quotes(char **temp,  t_env_tools env, int *i,  char *tmp_var);
void	treat_value_env(char **temp,  t_env_tools env, int i);
#endif