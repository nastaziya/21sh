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

/*
*** - Aim of the function :
*** - Print errors, and return 1
*/

static int	ft_find_path_and_cd2(char c)
{
	if (c == '~')
		ft_putstr_fd("No $HOME variable set.\n", 2);
	else
		ft_putstr_fd("No $OLDPWD variable set.\n", 2);
	return (1);
}

/*
*** - Aim of the function :
*** - Man the len of the char** for the cd command
*** - stops when meets '\0' or a '-' char
*/

int		ft_len_array_char_cd(char **av)
{
    int	i;

    i = 0;
    while (av[i] && (i > 0 ? ft_strcmp(av[i - 1], "-") : 1))
        i++;
    return (i);
}

/*
*** - Function to manage :
*** - cd alone
*** - cd -
*** - ft_find_path_and_cd2 => Manage the errors
*/

static int  ft_find_path_and_cd(char c, char ***c_env, t_env_tools *env,
                t_norm_cd *n) // p == 0 pour cd -P - ; sinon, p == 1
{
	int		count;
	char	*tmp;

	count = 0;
    // env->p = p;
	while ((*c_env)[count] && ft_strncmp((*c_env)[count],
				(c == '~' ? "HOME=" : "OLDPWD="), (c == '~' ? 5 : 7)))
		count++;
	if ((*c_env)[count])
	{
		tmp = ft_strdup(ft_strchr((*c_env)[count], '=') + 1);
        n->dash = 1;
		ft_change_dir_and_pwds(tmp, c_env, env, n);// envoyer n*
		if (c == '-')
            ft_putendl_fd(tmp, 1);
        free(tmp);
	}
	else
		return (ft_find_path_and_cd2(c));
	return (0);
}

/*
*** - Aim of the function : checks if the av is correct
*** - Then : keeps the letter of the last option set (*begin)
*** - Also :
*** - Keeps the position of the av I need to change directory to
*** - Begin == 0 when it ends with a "-"
*** - Aim of this part of the code :
*** - *begin = ((*av)[count - 1][0] == '-' || (i < count - 1)) ? 0 : i - 1;
*** - Begin == 0 quand ça se termine par un "-", sinon, renvoie la position du dernier - 1
*** - enfin, du dernier lu car la boucle ne lit que jusqu'au "-", s'il y en a un
*/

int   ft_normalize_av(char ***av, char *c, int *begin)
{
    int     i;
    int     count;

    i = 0;
    count = ft_len_array_char(*av);
    *c = 0;
    while (++i < count && (i < count && ft_strcmp((*av)[i], "-")))
    {
        if (!ft_strcmp((*av)[i], "-L") || !ft_strcmp((*av)[i], "-P")
            || !ft_strcmp((*av)[i], "-PL") || !ft_strcmp((*av)[i], "-LP")
                || (i == count - 1 && (*av)[i][0] == '-' ?
                    !ft_strcmp((*av)[i], "-") : i == count - 1))
        {
            if (((*av)[i][0] == '-' || !(i == count - 1)) && i != 0)
                if (ft_strcmp((*av)[i], "-"))
                    *c = (*av)[i][ft_strlen((*av)[i]) - 1];
        }
        else if ((*av)[i][0] != '-' && i != (count - 1))
            return (ft_int_error("cd: Too many arguments.\n", 2, 1));
        else
            return (ft_usage_error("cd", ": usage:"," [-L|-P] [dir]", 1));
    }
   *begin = ((*av)[count - 1][0] == '-' || (i < count - 1)) ? 0 : i - 1;
    return (0);
}

// envoyer en dernier paramètre -> n;
int                 ft_manage_cd_p_xxx(char **av, char ***c_env, t_norm_cd *n,
                        t_env_tools *env) // p == 0
{
    char	    buf[1024];
    
    n->p = 0;
    n->dash = 0;
    if (av[n->begin][0] == '/' && (n->dash = 1))
        return (ft_change_dir_and_pwds(av[n->begin], c_env, env, n));
    else if (!ft_strcmp(av[n->begin], "."))
    {
        getcwd(buf, sizeof(buf));
        n->dash = 1;
        return (ft_change_dir_and_pwds(buf, c_env, env, n));
    }
    else
        return (ft_change_dir_and_pwds(av[n->begin], c_env, env, n));
}

// A NORMER
int             ft_manage_cd_normal(char **av, char ***c_env, t_norm_cd *n,
                    t_env_tools *env) // p == 1
{
    char    *tmp;
    int     i;
    int     ret;
    char    buf[1024];

    i = 0;
    n->dash = 0;
    if (av[n->begin][0] == '/' && (n->dash = 1))
        return (ft_change_dir_and_pwds(av[n->begin], c_env, env, n));
    else if (!ft_strcmp(av[n->begin], ".") && (n->dash = 1)) // Gérer l'expansion du . => PWD
    {
        while ((*c_env)[i] && ft_strncmp("PWD=", (*c_env)[i], 4))
            i++;
        if ((*c_env)[i] && (tmp = ft_strdup(ft_strchr((*c_env)[i], '=') + 1)))
        {
            ret = ft_change_dir_and_pwds(tmp, c_env, env, n);
            free (tmp);
            return (ret);
        }
        else if (getcwd(buf, sizeof(buf)))
            return (ft_change_dir_and_pwds(buf, c_env, env, n));
    }
    else // Gérer le normal : cd 21LN (lien symbolique en l'occurence)
        return (ft_change_dir_and_pwds(av[n->begin], c_env, env, n));
    return (0);
}

int			        ft_builtin_cd(char **av, char ***c_env, t_env_tools *env)
{
	// int		argc;
    // char    c;
    // int     begin;
    t_norm_cd   n;

    n.begin = -1;
    n.p = 1;
    if (ft_normalize_av(&av, &n.c, &n.begin))
        return (1);
	n.argc = ft_len_array_char_cd(av);
    dprintf(2, "n.c: %d/%c - argc: %d - begin: %d\n", n.c, n.c, n.argc, n.begin);
    // cd, cd -L, cd -P
	if (n.argc == 1 || (((n.begin == 0 && n.c == 'P') || (n.begin == 0 && n.c == 'L'))
        && ft_strcmp(av[n.argc - 1], "-")))
        return (ft_find_path_and_cd('~', c_env, env, &n));
    // cd -, cd -L -
	else if ((n.c == 0 && n.begin == 0 && !ft_strcmp(av[n.argc - 1], "-"))
        || (n.c == 'L' && n.begin == 0 && !ft_strcmp(av[n.argc - 1], "-")))
        return (ft_find_path_and_cd('-', c_env, env, &n));
    // cd -P -
    else if (n.c == 'P' && n.begin == 0 && !ft_strcmp(av[n.argc - 1], "-"))
    {
        n.p = 0;
        // dprintf(2, "oui");
        return (ft_find_path_and_cd('-', c_env, env, &n));
    }
    // cd -P XXX => N'affiche pas PATH lien symbolique
	else if (n.begin > 0 && n.c == 'P')
        return (ft_manage_cd_p_xxx(av, c_env, &n, env));
    else if ((n.argc == 2 || (n.begin > 0 ))) // the rest
        return (ft_manage_cd_normal(av, c_env, &n, env));
	return (0);
}
