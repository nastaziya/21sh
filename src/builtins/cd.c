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
		ft_putstr_fd("No $HOME variable set.\n", 2);
    // else if (c == '|')
    //     ft_putstr_fd("No $PWD variable set.\n", 2);
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

static int  ft_find_path_and_cd(char c, char ***c_env, t_env_tools *env, int p)
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
		ft_change_directory_and_modify_pwds(tmp, c_env, env, 1, p);
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
*** - Then : checks returns the letter of the last option set
*** - Also :
*** - And the position of the av I need to change directory to
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

int		ft_len_array_char_cd(char **av)
{
    int	i;

    i = 0;
    while (av[i] && (i > 0 ? ft_strcmp(av[i - 1], "-") : 1))
        i++;
    dprintf(2, "[[%d]]\n", i);
    return (i);
}


// À gérer : cd . && cd .. ??? => DONE

// Gérer cd -P && -L

    // else if (argc == 2 && (!ft_strcmp(av[1], ".") || (!ft_strncmp(av[1], "./", 2)))) // cd . or cd ./
    // return(ft_manage_dot(av[1], c_env, env));
int			        ft_builtin_cd(char **av, char ***c_env, t_env_tools *env)
{
	// int		count;
	int		argc;
    char    c;
    int     begin;

    // count = 0;
    begin = -1;
    if(ft_normalize_av(&av, &c, &begin))
        return (1);
    dprintf(2, "|%c - %d|\n", c, begin);
	argc = ft_len_array_char_cd(av);
    // (argc == 1) || (argc == 2 && c == '-P') || (argc == 2 && c == '-L') => cd, cd -L, cd -P
	if (argc == 1 || (((begin == 0 && c == 'P') || (begin == 0 && c == 'L')) && ft_strcmp(av[argc - 1], "-"))) // cd alone, I send with the ~ char to differentiate with the -
	{
        dprintf(2, "1er");
        return (ft_find_path_and_cd('~', c_env, env, 1)); // 1 == PAS DE P
    }
    // (argc == 2 && c == '-') || (argc == 3 && c == 'L' && av[len -1] == '-') => cd -, cd -L -
	else if ((c == 0 && begin == 0 && !ft_strcmp(av[argc - 1], "-")) || (c == 'L' && begin == 0 && !ft_strcmp(av[argc - 1], "-"))) // cd - // || c == 'P'
	{
        /////////Intégrer la gestion de l'arrêt en plein milieu -> quand "-"
        /// !ft_strcmp(av[argc - 1], "-"), modifier fonction ft_len_array_char
        dprintf(2, "2ème");
        return (ft_find_path_and_cd('-', c_env, env, 1)); // || c == 'P') <= option ajoutée
    }
    // (argc == 3 && c == '-P' && av[len -1] == '-') => Manage "cd -P -" ==> refaire une fonction ft_find_path_and_cd qui prend en compte sa particularité
    else if (c == 'P' && begin == 0 && !ft_strcmp(av[argc - 1], "-"))
    {
        dprintf(2, "3ème");
        return (ft_find_path_and_cd('-', c_env, env, 0));
    }
        // -> Raffiche le lien symbolique la premiere fois, sinon après passe par le lien physique
    // (argc == 3 && c == '-P' => cd -P XXX => N'affiche pas PATH lien symbolique
	else if (begin > 0 && c == 'P') // Gérer les -P
    {// && ft_strcmp(av[begin], ".")
        dprintf(2, "4ème"); // terner is used to manage if av[begin] == /bla/bla/folder
        if (av[begin][0] == '/')
            return (ft_change_directory_and_modify_pwds(av[begin], c_env, env, 1, 0));
        else if (!ft_strcmp(av[begin], "."))
        {
            char	    buf[1024];
            getcwd(buf, sizeof(buf));
            // int i = 0;
            // while ((*c_env)[i] && ft_strncmp("PWD=", (*c_env)[i], 4))
            //     i++;
            // if ((*c_env)[i])
                return (ft_change_directory_and_modify_pwds(buf, c_env, env, 1, 0));// + 1
            // else
            //      return (ft_find_path_and_cd2('|'));
        }
        else
            return (ft_change_directory_and_modify_pwds(av[begin], c_env, env, 0, 0));
    }
        //oui
        // return (0);
    else if ((argc == 2 || (begin > 0 ))) // the rest //&& c == 'P'
	{// && ft_strcmp(av[begin], ".")
        dprintf(2, "5ème"); // terner is used to manage if av[begin] == /bla/bla/folder
        if (av[begin][0] == '/') // Gérer envoie PATH et par la même occasion expansion ~/bla
            return (ft_change_directory_and_modify_pwds(av[begin], c_env, env, 1, 1));
        else if (!ft_strcmp(av[begin], ".")) // Gérer l'expansion du . => PWD
        {
            char *tmp;
            int i = 0;
            while ((*c_env)[i] && ft_strncmp("PWD=", (*c_env)[i], 4))
                i++;
            // dprintf(2, "zut - |%s|\n", ft_strchr((*c_env)[i], '=') + 1);
            if ((*c_env)[i])
            {
                tmp = ft_strdup(ft_strchr((*c_env)[i], '=') + 1);
                int ret = ft_change_directory_and_modify_pwds(tmp, c_env, env, 1, 1);
                free (tmp);
                return (ret);
            }
                // return (ft_change_directory_and_modify_pwds(tmp, c_env, env, 1, 1));// + 1
            else
            {
                char	    buf[1024];
                getcwd(buf, sizeof(buf));
            ///// !!!!!! /////
                ///// SUREMENT PASSER PAR LE CURRENT WORKING DIRECTORY ET NON LA GESTION D'ERREUR
                // return (ft_find_path_and_cd2('|'));
                return (ft_change_directory_and_modify_pwds(buf, c_env, env, 1, 1));
            }
        }
       else // Gérer le normal : cd 21LN (lien symbolique en l'occurence)
            return (ft_change_directory_and_modify_pwds(av[begin], c_env, env, 0, 1));
    }
	// else // error
	// {
	// 	ft_putstr_fd("cd: Too many arguments.\n", 2);
	// 	return (1);
	// }
	return (0);
}

/////// Gerer expansion ~ => conflit avec LINK OU NON