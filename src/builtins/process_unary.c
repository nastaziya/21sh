/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 17:44:41 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"
# include "stdio.h"

/*
*** - Aim of the function :
*** - Return the full path of the file
*/

char            *find_file_path(char *arg)
{
    char buf[512];
    char *tmp;
    char *path;

    ft_bzero(buf, 512);
    getcwd(buf, sizeof(buf));
    tmp = ft_strjoin(buf, "/");
    path = ft_strjoin(tmp, arg);
    free(tmp);
    return (path);
}

/*
*** - Aim of the function :
*** - Following of the previous check_option function
*** - For the norm
*** - -s <FILE>	True, if <FILE> exists and has size bigger than 0 (not empty).
*** - -u <FILE>	True, if <FILE> exists and has suid bit set.
*** - -w <FILE>	True, if <FILE> exists and is writable.
*** - -x <FILE>	True, if <FILE> exists and is executable.
*** - -z <STRING>	True, if <STRING> is empty.
*/

void             check_option_norm(t_test_tok tok, struct stat buf, int *ret,
                    char *arg)
{
    if (tok == T_S)
        *ret = (buf.st_size > 0 ? 0 : 1);
    else if (tok == T_U)
        *ret = (buf.st_mode & S_ISUID ? 0 : 1);
    else if (tok == T_W)
        *ret = (buf.st_mode & S_IWUSR ? 0 : 1);
    else if (tok == T_X)
        *ret = (buf.st_mode & S_IXUSR ? 0 : 1);
    else if (tok == T_Z)
        *ret = (ft_strcmp(arg, " ") ? 0 : 1);
}

/*
*** - Aim of the function :
*** - Check all options according to the instructions
*** - -b <FILE>	True, if <FILE> exists and is a block special file.
*** - -c <FILE>	True, if <FILE> exists and is a character special file.
*** - -d <FILE>	True, if <FILE> exists and is a directory.
*** - -e <FILE> True if <FILE> exists.
*** - -f <FILE>	True, if <FILE> exists and is a regular file.
*** - -g <FILE>	True, if <FILE> exists and has sgid bit set.
*** - -L <FILE>	True, if <FILE> exists and is a symbolic link.
*** - -p <FILE>	True, if <FILE> exists and is a named pipe (FIFO).
*** - -r <FILE>	True, if <FILE> exists and is readable.
*** - -S <FILE>	True, if <FILE> exists and is a socket file.
*/

void             check_option(t_test_tok tok, struct stat buf, int *ret,
                    char *arg)
{
    if (tok == T_B)
        *ret = (S_ISBLK(buf.st_mode) ? 0 : 1);
    else if (tok == T_C)
        *ret = (S_ISCHR(buf.st_mode) ? 0 : 1);
    else if (tok == T_D)
    *ret = (S_ISDIR(buf.st_mode) ? 0 : 1);
    else if (tok == T_F)
        *ret = (S_ISREG(buf.st_mode) ? 0 : 1);
    else if (tok == T_G)
        *ret = (buf.st_mode & S_ISGID ? 0 : 1);
    else if (tok == T_LL)
        *ret = (buf.st_mode & S_IFLNK ? 0 : 1);
    else if (tok == T_P)
        *ret = (S_ISFIFO(buf.st_mode) ? 0 : 1);
    else if (tok == T_R)
        *ret = (buf.st_mode & S_IRUSR ? 0 : 1);
    else if (tok == T_SS)
        *ret = (S_ISSOCK(buf.st_mode) ? 0 : 1);
    else
        check_option_norm(tok, buf, ret, arg);
}

/*
*** - Aim of the function :
*** - Process the Unary test
*** - First, we find the full path of the parameter arg
*** - Then we collect data on it (through stat)
*** - if the file doesn't exist for these conditions, return 1
*** - Otherwise, we continue and we check the return of the
*** - corresponding option
*/

int             process_unary(t_test_tok tok, char *arg)
{
    int         ret;
    char        *path;
    struct stat buf;

    ret = 0;
    path = find_file_path(arg);
    if ((stat(path, &buf) == -1) &&
        (tok == T_E || tok == T_R || tok == T_LL || tok == T_G
        || tok == T_S || tok == T_U || tok == T_W || tok == T_X)
        && !ft_free(path))
        return (1);
    
    check_option(tok, buf, &ret, arg);
    free(path);
    return (ret);
}