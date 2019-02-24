/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/24 16:35:22 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include <fcntl.h>

static void		first_realloc_path(char **s1, char *s2)
{
	char	*tmp;

	tmp = *s1;
	*s1 = ft_strjoin(tmp, s2);
	free(tmp);
}

static void		realloc_path(char **s1, char *s2, char *s3)
{
	char	*tmp;

	tmp = ft_strjoin(*s1, s2);
	*s1 = ft_strjoin(tmp, s3);
	free(tmp);
}

char			*get_envpath_from_file(void)
{
	char	buf[2049];
	char	**paths;
	char	*path;
	int		fd;
	int		i;

	i = -1;
	paths = NULL;
	path = ft_strdup("PATH=");
	if ((fd = open("/etc/paths", O_RDONLY)) > 0)
	{
		ft_bzero(buf, 2049);
		if ((read(fd, buf, 2048)) > 0)
			paths = ft_strsplit(buf, '\n');
		if (paths && paths[++i])
			first_realloc_path(&path, paths[i]);
		while (paths && paths[++i] && paths[i])
			realloc_path(&path, ":", paths[i]);
		close(fd);
	}
	ft_free_av(paths);
	return (path);
}
