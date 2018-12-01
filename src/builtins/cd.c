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
                int p)
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
		ft_change_dir_and_pwds(tmp, c_env, env, 1, p);
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

int                 ft_manage_cd_p_xxx(char **av, char ***c_env, int begin,
                        t_env_tools *env)
{
    char	    buf[1024];

    if (av[begin][0] == '/')
        return (ft_change_dir_and_pwds(av[begin], c_env, env, 1, 0));
    else if (!ft_strcmp(av[begin], "."))
    {
        getcwd(buf, sizeof(buf));
        return (ft_change_dir_and_pwds(buf, c_env, env, 1, 0));
    }
    else
        return (ft_change_dir_and_pwds(av[begin], c_env, env, 0, 0));
}

// A NORMER
int             ft_manage_cd_normal(char **av, char ***c_env, int begin,
                    t_env_tools *env)
{
    char    *tmp;
    int     i;
    int     ret;
    char    buf[1024];

    i = 0;
    if (av[begin][0] == '/')
        return (ft_change_dir_and_pwds(av[begin], c_env, env, 1, 1));
    else if (!ft_strcmp(av[begin], ".")) // Gérer l'expansion du . => PWD
    {
        while ((*c_env)[i] && ft_strncmp("PWD=", (*c_env)[i], 4))
            i++;
        if ((*c_env)[i] && (tmp = ft_strdup(ft_strchr((*c_env)[i], '=') + 1)))
        {
            ret = ft_change_dir_and_pwds(tmp, c_env, env, 1, 1);
            free (tmp);
            return (ret);
        }
        else if (getcwd(buf, sizeof(buf)))
            return (ft_change_dir_and_pwds(buf, c_env, env, 1, 1));
    }
    else // Gérer le normal : cd 21LN (lien symbolique en l'occurence)
        return (ft_change_dir_and_pwds(av[begin], c_env, env, 0, 1));
    return (0);
}

int			        ft_builtin_cd(char **av, char ***c_env, t_env_tools *env)
{
	int		argc;
    char    c;
    int     begin;

    begin = -1;
    if(ft_normalize_av(&av, &c, &begin))
        return (1);
	argc = ft_len_array_char_cd(av);
    // cd, cd -L, cd -P
	if (argc == 1 || (((begin == 0 && c == 'P') || (begin == 0 && c == 'L'))
        && ft_strcmp(av[argc - 1], "-")))
        return (ft_find_path_and_cd('~', c_env, env, 1));
    // cd -, cd -L -
	else if ((c == 0 && begin == 0 && !ft_strcmp(av[argc - 1], "-"))
        || (c == 'L' && begin == 0 && !ft_strcmp(av[argc - 1], "-")))
        return (ft_find_path_and_cd('-', c_env, env, 1));
    // cd -P -
    else if (c == 'P' && begin == 0 && !ft_strcmp(av[argc - 1], "-"))
        return (ft_find_path_and_cd('-', c_env, env, 0));
    // cd -P XXX => N'affiche pas PATH lien symbolique
	else if (begin > 0 && c == 'P')
        return (ft_manage_cd_p_xxx(av, c_env, begin, env));
    else if ((argc == 2 || (begin > 0 ))) // the rest
        return (ft_manage_cd_normal(av, c_env, begin, env));
	return (0);
}
