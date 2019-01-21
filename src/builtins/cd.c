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
*** - Function to manage :
*** - cd alone
*** - cd -
*** - ft_find_path_and_cd2 => Manage the errors
*** - p == 0 for cd -P - ; otherwise p == 1
*/

static int  ft_find_path_and_cd(char c, char ***c_env, t_env_tools *env,
                t_norm_cd *n)
{
	int		count;
	char	*tmp;

	count = 0;
	while ((*c_env)[count] && ft_strncmp((*c_env)[count],
				(c == '~' ? "HOME=" : "OLDPWD="), (c == '~' ? 5 : 7)))
		count++;
	if ((*c_env)[count])
	{
		tmp = ft_strdup(ft_strchr((*c_env)[count], '=') + 1);
        n->dash = 1;
		ft_change_dir_and_pwds(&tmp, c_env, env, n);
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

static int   ft_normalize_av(char ***av, char *c, int *begin)
{
    int     i;
    int     count;

    i = 0;
    count = ft_len_array_char(*av);
    *c = 0;
    while (++i < count && (i < count && ft_strcmp((*av)[i], "-")))
    {
        // !ft_strcmp((*av)[i], "-L") || !ft_strcmp((*av)[i], "-P")
            // || !ft_strcmp((*av)[i], "-PL") || !ft_strcmp((*av)[i], "-LP"
        if (!ft_usage_is_good("LP", (*av)[i])
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

/*
*** - Aim of the function :
*** - We copy the behavior of the cd builtin
*** - In order of the ifs and elsifs, I manage the following commands :
*** - same behavior as the function below
*** - p == 0
*/

int                 ft_manage_cd_p_xxx(char **av, char ***c_env, t_norm_cd *n,
                        t_env_tools *env)
{
    // char	    buf[1024];
    char	    *buf;
    int         ret;
    
    n->p = 0;
    n->dash = 0;
    buf = ft_strnew(1024);
    if (av[n->begin][0] == '/' && (n->dash = 1))
    {
        ret = ft_change_dir_and_pwds(&(av[n->begin]), c_env, env, n);
        return (ret);
    }
        // return (ft_change_dir_and_pwds(&(av[n->begin]), c_env, env, n));
    else if (!ft_strcmp(av[n->begin], "."))
    {
        getcwd(buf, sizeof(buf));
        n->dash = 1;
        ret = ft_change_dir_and_pwds(&buf, c_env, env, n);
        // return (ft_change_dir_and_pwds(&buf, c_env, env, n));
        return (ret);
    }
    else
    {
        ret = ft_change_dir_and_pwds(&(av[n->begin]), c_env, env, n);
        return (ret);
    }
        // return (ft_change_dir_and_pwds(&(av[n->begin]), c_env, env, n));
}


/*
*** - Aim of the function :
*** - We copy the behavior of the cd builtin
*** - In order of the ifs and elsifs, I manage the following commands :
*** - 1. cd /21sh
*** - 2. cd . blabla or cd ./blabla and verifies that the PWD exists
*** - 3. the normal cd -> cd 21LN for example
*** - p == 1
*/

// A NORMER !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

int             ft_manage_cd_normal(char **av, char ***c_env, t_norm_cd *n,
                    t_env_tools *env)
{
    char    *tmp;
    int     i;
    int     ret;
    char    *buf;

    i = 0;
    n->dash = 0;
    if (av[n->begin][0] == '/' && (n->dash = 1))
        return (ft_change_dir_and_pwds(&(av[n->begin]), c_env, env, n));
    else if (!ft_strcmp(av[n->begin], ".") && (n->dash = 1))
    {
        while ((*c_env)[i] && ft_strncmp("PWD=", (*c_env)[i], 4))
            i++;
        if ((*c_env)[i] && (tmp = ft_strdup(ft_strchr((*c_env)[i], '=') + 1)))
        {
            ret = ft_change_dir_and_pwds(&tmp, c_env, env, n);
            free (tmp);
            return (ret);
        }
        else if ((buf = ft_strnew(1024)) && getcwd(buf, sizeof(buf)))
        {
            ret = ft_change_dir_and_pwds(&buf, c_env, env, n);
            free(buf);
            return (ret);
        }
        free(buf);
    }
    else
        return (ft_change_dir_and_pwds(&(av[n->begin]), c_env, env, n));
    return (0);
}

/*
*** - Aim of the function :
*** - We copy the behavior of the cd builtin
*** - In order of the ifs and elsifs, I manage the following commands :
*** - 1. cd, cd -L, cd -P
*** - 2. cd -, cd -L -
*** - 3. cd -P -
*** - 4. cd -P XXX => which shows the physical link, not the symbolic one
*** - 5. The rest (cd 21LN / cd -L 21LN)
*/

int			        ft_builtin_cd(char **av, char ***c_env, t_env_tools *env)
{
    t_norm_cd   n;

    n.begin = -1;
    n.p = 1;
    if (ft_normalize_av(&av, &n.c, &n.begin))
        return (1);
	n.argc = ft_len_array_char_cd(av);
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
        return (ft_find_path_and_cd('-', c_env, env, &n));
    }
    // cd -P XXX => N'affiche pas PATH lien symbolique
	else if (n.begin > 0 && n.c == 'P')
        return (ft_manage_cd_p_xxx(av, c_env, &n, env));
    else if ((n.argc == 2 || (n.begin > 0 ))) // the rest
        return (ft_manage_cd_normal(av, c_env, &n, env));
	return (0);
}
