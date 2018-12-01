/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/04/19 18:02:22 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"

static int		ft_print_error(char *av, char *str)
{
	ft_putstr_fd(av, 2);
	ft_putstr_fd(str, 2);
	return (1);
}

int		ft_print_dir_error(char *command, char *btwn, char *after, int i)
{
	ft_putstr_fd(command, 2);
	ft_putstr_fd(btwn, 2);
	ft_putendl_fd(after, 2);
	return (i);
}

/*
*** - Fonction : Gestion d'erreur du cd - et PWD & OLDPWD non set
*/
// fonctionne pour cd et cd -L
// => cd -P == comme avant (cf minishell)
int	    	ft_change_dir_and_pwds(char *av, char ***c_env, t_env_tools *env, int dash, int p) // -> DASH == GESTION "-"
{
	char	    *tmp;
    char        *tmp2;
	char	    buf[1024];
    struct stat buf2;
    int         i;
    char        *s1;
    char        *s2;

    i = 0;
    if (dash == 0 && ft_strcmp(av, ".."))
    {
        char    pa[1024];
        getcwd(pa, sizeof(pa));
        s1 = ft_strjoin(buf, "/");
        s2 = ft_strjoin(s1, av);
        free(s1);
    }
    else
        s2 = ft_strdup(av);
    // dprintf(2, "lstat, s2: %s\n", s2);
    stat(s2, &buf2);
	if ((access(av, F_OK)) == -1)
		return(ft_print_error(av, ": No such file or directory.\n"));
	else if (ft_strcmp(av, "..") && !S_ISDIR(buf2.st_mode) && ft_free(s2))
        return (ft_print_dir_error("bash: cd:", av, ": Not a directory", 1));
    else if ((access(av, X_OK)) == -1)
		return (ft_print_error(av, ": Permission denied.\n"));
	else
	{
        free (s2);
        // dprintf(2, "[%s]\n", av);
        while ((*c_env)[i] && ft_strncmp((*c_env)[i], "PWD=", 4))
		    i++;
        // dprintf(2, "c_env[i]: %s - %d\n", (*c_env)[i], i);
        if ((*c_env)[i])
            tmp2 = ft_strjoin("OLDPWD=", (*c_env)[i] + 4);
        else
        {
            char	    curpath[1024];
            getcwd(curpath, sizeof(curpath));
            tmp2 = ft_strjoin("OLDPWD=", curpath);
        }
		// dprintf(2, "TMP2: %s\n", tmp2);
        ft_builtin_setenv_2(tmp2, c_env, &(env->paths), env);
        if (dash == 0)
        {
            free(tmp2);
            getcwd(buf, sizeof(buf));
            tmp = ft_strjoin(buf, "/");
            tmp2 = ft_strjoin(tmp, av);
            free(tmp);
        }
        int ret = 0;
        ret = dash == 0 ? lstat(tmp2, &buf2) : lstat(av, &buf2);
        // dprintf(2, "|%s| - ret: %d - %d\n", tmp2, ret, S_ISLNK(buf2.st_mode));
        if (!S_ISLNK(buf2.st_mode) || p == 0) // || BOOL -> "-P" // || p == 0
		{
            // dprintf(2, "NO LINK\n");
            chdir(av);
            getcwd(buf, sizeof(buf));
		    tmp = ft_strjoin("PWD=", buf);
		    ft_builtin_setenv_2(tmp, c_env, &(env->paths), env);
            // dprintf(2, "TMP_!S_NOT_LINK: %s\n", tmp);
		    free(tmp);
            free(tmp2);
        }
        else if (S_ISLNK(buf2.st_mode))
        {
            // dprintf(2, "LINK\n");//- |%s|
            chdir(av);
            if (dash == 0)// quand ce n'est pas la commande cd '-'
            // {
                tmp = ft_strjoin("PWD=", tmp2);
                // dprintf(2, "TMP_S_IS_LINK_IF: %s\n", tmp2);
            // }
            else
            // {
                tmp = ft_strjoin("PWD=", av);
                // dprintf(2, "TMP_S_IS_LINK_ELSE: %s\n", av);//- |%s|
            // }
            free(tmp2);
            ft_builtin_setenv_2(tmp, c_env, &(env->paths), env);
		    free(tmp);
        }
		
	}
    return (0);
}
