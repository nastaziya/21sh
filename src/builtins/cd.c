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

static int	ft_find_path_and_cd2(char c)
{
	if (c == '~')
		ft_putstr_fd("No $home variable set.\n", 2);
	else
		ft_putstr_fd("No $OLDPWD variable set.\n", 2);
	return (1);
}

/*
*** - Function to manage :
*** - cd alone
*** - cd -
*** - ft_find_path_and_cd2 => Manage the errors
*/

static int  ft_find_path_and_cd(char c, char ***c_env, t_env_tools *env)
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
		ft_change_directory_and_modify_pwds(tmp, c_env, env, 1);
		if (c == '-')
            ft_putendl_fd(tmp, 1);
        free(tmp);
	}
	else
		return (ft_find_path_and_cd2(c));
	return (0);
}

// static int  ft_manage_dash_p_option(char c, char ***c_env, t_env_tools *env)
// {
// 	int		count;
// 	char	*tmp;

// 	count = 0;
// 	while ((*c_env)[count] && ft_strncmp((*c_env)[count],
// 				(c == '~' ? "HOME=" : "OLDPWD="), (c == '~' ? 5 : 7)))
// 		count++;
// 	if ((*c_env)[count])
// 	{
// 		tmp = ft_strdup(ft_strchr((*c_env)[count], '=') + 1);
// 		ft_change_directory_and_modify_pwds(tmp, c_env, env, 1);
// 		if (c == '-')
//         ft_putendl_fd(tmp, 1);
//         free(tmp);
// 	}
// 	else
// 		return (ft_find_path_and_cd2(c));
// 	return (0);
// }


int   ft_normalize_av(char ***av, char *c, int *begin)
{
    int     i;
    int     count;

    i = 0;
    count = ft_len_array_char(*av);
    *c = 0;
    while (++i < count)
    {
        if (!ft_strcmp((*av)[i], "-L") || !ft_strcmp((*av)[i], "-P")
            || !ft_strcmp((*av)[i], "-PL") || !ft_strcmp((*av)[i], "-LP")
                || (i == count - 1))
            if (!(i == count - 1 && !ft_strcmp(av[i], "-")))
                *c = (*av)[i][ft_strlen((*av)[i]) - 1];
        else if ((*av)[i][0] != '-' && i != (count - 1))
        {
            ft_putstr_fd("cd: Too many arguments.\n", 2);
		    return (1);
        }
        else
            return (ft_usage_error("cd", ": usage:"," [-L|-P] [dir]", 1));
    }
    // Begin == 0 quand ça se termine par un "-", sinon, renvoie la position du dernier - 1
    *begin = (*av)[count - 1][0] == '-' ? 0 : count - 1;
    // begin = (count - 1);
    return (0);
}


void		ft_manage_dash_p_option(char *av, char ***c_env, t_env_tools *env)
{
	char	*tmp;
	char	buf[1024];

	if ((access(av, F_OK)) == -1)
		ft_print_error(av, ": No such file or directory.\n");
	else if ((access(av, X_OK)) == -1)
		ft_print_error(av, ": Permission denied.\n");
	else
	{
		getcwd(buf, sizeof(buf));
		tmp = ft_strjoin("OLDPWD=", buf);
		ft_builtin_setenv_2(tmp, c_env, 0);
		free(tmp);
		chdir(av);
		getcwd(buf, sizeof(buf));
		tmp = ft_strjoin("PWD=", buf);
		ft_builtin_setenv_2(tmp, c_env, 0);
		free(tmp);
	}
}

// À gérer : cd . && cd .. ??? => DONE

// Gérer cd -P && -L

    // else if (argc == 2 && (!ft_strcmp(av[1], ".") || (!ft_strncmp(av[1], "./", 2)))) // cd . or cd ./
    // return(ft_manage_dot(av[1], c_env, env));
int			        ft_builtin_cd(char **av, char ***c_env, t_env_tools *env)
{
	int		count;
	int		argc;
    char    c;
    int     begin;

	count = 0;
    begin = -1;
    if(ft_normalize_av(&av, &c, &begin))
        return (1);
	argc = ft_len_array_char(av);
    // (argc == 1) || (argc == 2 && c == '-P') || (argc == 2 && c == '-L') => cd, cd -L, cd -P
	if (argc == 1 || (begin == 0 && c == 'P') || (begin == 0 && c == 'L')) // cd alone, I send with the ~ char to differentiate with the -
		return (ft_find_path_and_cd('~', c_env, env));
    // (argc == 2 && c == '-') || (argc == 3 && c == 'L' && av[len -1] == '-') => cd -, cd -L -
	else if ((c == 0 && begin == 0 && !ft_strcmp(av[count - 1], "-")) || (c == 'L' && begin == 0 && !ft_strcmp(av[count - 1], "-"))) // cd -
		return (ft_find_path_and_cd('-', c_env, env)); // || c == 'P') <= option ajoutée
    // (argc == 3 && c == '-P' && av[len -1] == '-') => Manage "cd -P -" ==> refaire une fonction ft_find_path_and_cd qui prend en compte sa particularité
    else if (c == 'P' && begin == 0 && !ft_strcmp(av[count - 1], "-"))
        return (ft_manage_dash_p_option('-', c_env, env)); 
        // -> Raffiche le lien symbolique la premiere fois, sinon après passe par le lien physique
    // (argc == 3 && c == '-P' => cd -P XXX => N'affiche pas PATH lien symbolique
	else if (begin > 0 && c == 'P') // Gérer les -P
        return (ft_change_directory_PWD(av[begin], c_env, env));
        //oui
        // return (0);
    else if (argc == 2 || (begin > 0 && c == 'P')) // the rest
		ft_change_directory_and_modify_pwds(av[begin], c_env, env, 0);
	// else // error
	// {
	// 	ft_putstr_fd("cd: Too many arguments.\n", 2);
	// 	return (1);
	// }
	return (0);
}