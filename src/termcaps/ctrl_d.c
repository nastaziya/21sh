/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_strstr.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/04/19 18:02:22 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 22:34:17 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

/*
*** - Aim of the function : to manage the ctrl_d behavior when in subshell
*/

static void		ctrl_d_management_norm(t_tcap *caps)
{
	caps->sz_str = 10;
	free(caps->str[0]);
	g_keeprun = 5;
	if (!ft_strcmp(caps->prompt, "dquote > ") ||
			!ft_strcmp(caps->prompt, "squote > "))
	{
		ft_putstr_fd("bash: unexpected EOF while looking for matching `\"\'\n"
				, 2);
		caps->str[0] = !ft_strcmp(caps->prompt, "dquote > ") ? ft_strdup("\"")
			: ft_strdup("\'");
	}
	else if (!ft_strcmp(caps->prompt, "Missing arguments > "))
		caps->str[0] = ft_strdup("oui");
	else if (!ft_strcmp(caps->prompt, "Heredoc > ")
			|| !ft_strcmp(caps->prompt, "\nHeredoc > "))
	{
		g_keeprun = 4;
		caps->str[0] = ft_memalloc(1);
		caps->sz_str = ft_strlen(caps->prompt);
	}
}

/*
*** - Aim of the function : to manage properly the ctrl_d key
*** - if we are inside the bash prompt, do as if someone wrote "exit"
*** - ctrl_d_management_norm(caps) -> manages when inside subshell
*** - if there is a string, ring the bell and don't exit :-)
*/

int				ctrl_d_management(t_tcap *caps)
{
	if (((caps->sz_str - caps->size_prompt) == 0))
	{
		if (!ft_strcmp(caps->prompt, "bash » "))
		{
			free(caps->str[0]);
			caps->str[0] = ft_strdup("exit");
		}
		else
			ctrl_d_management_norm(caps);
		return (1);
	}
	else
		tputs(tgetstr("bl", NULL), 1, ft_outc);
	return (0);
}
