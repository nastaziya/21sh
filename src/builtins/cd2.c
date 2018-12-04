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

void         ft_copy_and_stat(t_norm_cd *n_cd, t_norm_pwd *n, char *av)
{
    // ft_copy_and_stat(n_cd, n, av);
    // s2 = ft_manage_double_dots;
    // gérer ".." -> dans chemin ou non ?
    if (n_cd->dash == 0 && ft_strcmp(av, ".."))
    {
        getcwd(n->buf, sizeof(n->buf));
        n->s1 = ft_strjoin(n->buf, "/");
        n->s2 = ft_strjoin(n->s1, av);
        free(n->s1);
    }
    else
        n->s2 = ft_strdup(av);
    // stat pour regarder si là ou ça pointe est un dossier -> erreur not a directory
    stat(n->s2, &n->buf2);
    // return (0);
}

/*
*** - Fonction : Gestion d'erreur du cd - et PWD & OLDPWD non set
*/
// fonctionne pour cd et cd -L
// => cd -P == comme avant (cf minishell)
int	    	ft_change_dir_and_pwds(char *av, char ***c_env, t_env_tools *env, t_norm_cd *n_cd) // -> DASH == GESTION "-"
{
    // int             dash;
    t_norm_pwd      n;
    // dash = n_cd->dash;
    // n.p = env->p;
    n.i = 0;
    ft_copy_and_stat(n_cd, &n, av);
    // // s2 = ft_manage_double_dots;
    // // gérer ".." -> dans chemin ou non ?
    // if (n_cd->dash == 0 && ft_strcmp(av, ".."))
    // {
    //     getcwd(n->buf, sizeof(n->buf));
    //     n->s1 = ft_strjoin(n->buf, "/");
    //     n->s2 = ft_strjoin(n->s1, av);
    //     free(n->s1);
    // }
    // else
    //     n->s2 = ft_strdup(av);
    // // stat pour regarder si là ou ça pointe est un dossier -> erreur not a directory
    // stat(n->s2, &n->buf2);
    // erreurs
	if ((access(av, F_OK)) == -1)
		return(ft_print_error(av, ": No such file or directory.\n"));
	else if (ft_strcmp(av, "..") && !S_ISDIR(n->buf2.st_mode) && ft_free(n->s2))
        return (ft_print_dir_error("bash: cd:", av, ": Not a directory", 1));
    else if ((access(av, X_OK)) == -1 && ft_free(n->s2))
		return (ft_print_error(av, ": Permission denied.\n"));
	else //process
	{
        // pour les leaks
        free (n->s2);
        // on remplace l'OLDPWD par là où on est -> LE PWD ACTUEL OU LE OLDPWD SI
        // PWD A ETE UNSETENV
        while ((*c_env)[n->i] && ft_strncmp((*c_env)[n->i], "PWD=", 4))
		    n->i++;
        if ((*c_env)[n->i])
            n->tmp2 = ft_strjoin("OLDPWD=", (*c_env)[n->i] + 4);
        else
        {
            getcwd(n->buf, sizeof(n->buf));
            n->tmp2 = ft_strjoin("OLDPWD=", n->buf);
        }
        ft_builtin_setenv_2(n->tmp2, c_env, &(env->paths), env);
       
       // si c'est un chemin sans / au début, alors on rajoute par rapport au
       // chemin actuel -> 
        if (n_cd->dash == 0)
        {
            free(n->tmp2);
            getcwd(n->buf, sizeof(n->buf));
            n->tmp = ft_strjoin(n->buf, "/");
      // A MODIFIER -> passer les ./././ et les ../../../ je pense
      /////////////////
      //////////
            n->tmp2 = av[0] == '.' && av[1] == '/' ? ft_strjoin(n->tmp, av + 2) : ft_strjoin(n->tmp, av);
            free(n->tmp);
        }
        // lstat sur le path correct
        n_cd->dash == 0 ? lstat(n->tmp2, &n->buf2) : lstat(av, &n->buf2);
        if (!S_ISLNK(n->buf2.st_mode) || norm_cd->p == 0) // p == Gérer le -P && lien non symboliques
		{
            chdir(av);
            getcwd(n->buf, sizeof(n->buf));
		    n->tmp = ft_strjoin("PWD=", n->buf);
		    ft_builtin_setenv_2(n->tmp, c_env, &(env->paths), env);
		    free(n->tmp);
            free(n->tmp2);
        }
        else if (S_ISLNK(n->buf2.st_mode)) // Gérer les liens symboliques
        {
            chdir(av);
            if (n_cd->dash == 0)// quand il y a un / dans le path ou commande cd -
                n->tmp = ft_strjoin("PWD=", n->tmp2);
            else
                n->tmp = ft_strjoin("PWD=", av);
            free(n->tmp2);
            ft_builtin_setenv_2(n->tmp, c_env, &(env->paths), env);
		    free(n->tmp);
        }
	}
    return (0);
}
