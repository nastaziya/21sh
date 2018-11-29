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

/*
*** - Fonction : Gestion d'erreur du cd - et PWD & OLDPWD non set
*/
// fonctionne pour cd et cd -L
// => cd -P == comme avant (cf minishell)
int	    	ft_change_directory_and_modify_pwds(char *av, char ***c_env, t_env_tools *env, int dash) // -> DASH == GESTION "-"
{
	char	    *tmp;
    char        *tmp2;
	char	    buf[1024];
    struct stat buf2;
    int         i;

    i = 0;
    // tmp = NULL;
	if ((access(av, F_OK)) == -1)
		return(ft_print_error(av, ": No such file or directory.\n"));
	else if ((access(av, X_OK)) == -1)
		return (ft_print_error(av, ": Permission denied.\n"));
	else
	{
        dprintf(2, "[%s]\n", av);
		// if (dash == 0)
        // {
        //     getcwd(buf, sizeof(buf));
		//     tmp2 = ft_strjoin("OLDPWD=", buf);
        // }
        // else
        // {

        // }
        while ((*c_env)[i] && ft_strncmp((*c_env)[i], "PWD=", 4))
		    i++;
        dprintf(2, "c_env[i]: %s - %d\n", (*c_env)[i], i);
        tmp2 = ft_strjoin("OLDPWD=", (*c_env)[i] + 4);
        // void		ft_builtin_setenv_2(char *av, char ***c_env, char ***paths, t_env_tools *env)
		dprintf(2, "TMP2: %s\n", tmp2);
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
        dprintf(2, "|%s| - ret: %d - %d\n", tmp2, ret, S_ISLNK(buf2.st_mode));
        if (!S_ISLNK(buf2.st_mode)) // || 
		{
            dprintf(2, "NO LINK\n");
            chdir(av);
            getcwd(buf, sizeof(buf));
		    tmp = ft_strjoin("PWD=", buf);
		    ft_builtin_setenv_2(tmp, c_env, &(env->paths), env);
            dprintf(2, "TMP_!S_NOT_LINK: %s\n", tmp);
		    free(tmp);
            free(tmp2);
        }
        else if (S_ISLNK(buf2.st_mode))
        {
            // Mettre un if, si "-P"
            // IF ("-P")
            // {
                // if (dash == 0)
                // {
                //      chdir(av);
		    //          getcwd(buf, sizeof(buf));
		    //          tmp = ft_strjoin("PWD=", buf);
		    //          ft_builtin_setenv_2(tmp, c_env, 0);
		    //          free(tmp); 
                // }
                // else
                // {
            //          chdir(av);
		    //          getcwd(buf, sizeof(buf));
		    //          tmp = ft_strjoin("PWD=", buf);
		    //          ft_builtin_setenv_2(tmp, c_env, 0);
		    //          free(tmp);
            //      }
            


            // getcwd(buf, sizeof(buf));
            dprintf(2, "LINK\n");//- |%s|
            chdir(av);
            // tmp = ft_strjoin("PWD=", buf);
            if (dash == 0)// quand ce n'est pas la commande cd '-'
            {
                tmp = ft_strjoin("PWD=", tmp2);
                dprintf(2, "TMP_S_IS_LINK_IF: %s\n", tmp2);
            }
            else
            {
                tmp = ft_strjoin("PWD=", av);
                dprintf(2, "TMP_S_IS_LINK_ELSE: %s\n", av);//- |%s|
            }
            // tmp = ft_strjoin("PWD=", tmp2);
            // tmp2 = ft_strjoin(tmp, av);
            free(tmp2);
            ft_builtin_setenv_2(tmp, c_env, &(env->paths), env);
		    free(tmp);
        }
		
	}
    return (0);
}

// int         ft_manage_dot(char *av, char ***c_env, t_env_tools *env)
// {
//     char    *tmp;
//     int     ret;

//     if (!ft_strcmp(av, "."))
//         ret = ft_change_directory_and_modify_pwds(env->home, c_env, env);
//     else
//     {
//         tmp = ft_strjoin(env->home, av + 1);
//         ret = ft_change_directory_and_modify_pwds(tmp, c_env, env);
//         free(tmp);
//     }
//     return (ret);
// }