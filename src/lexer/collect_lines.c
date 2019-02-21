/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2019/02/09 21:36:00 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"

/*
*** - Aim of the function :
*** - Print the corresponding prompt according the corresponding error
*/

char		*ft_manage_prompt(char type_quote)
{
	if (type_quote == '"')
	{
		display_bash("\ndquote > ");
		return ("dquote > ");
	}
	else if (type_quote == '\'')
	{
		display_bash("\nsquote > ");
		return ("squote > ");
	}
	else if (type_quote == '\\')
	{
		display_bash("\n> ");
		return ("> ");
	}
	return (NULL);
}

/*
*** - Aim of the function :
*** - Print new promt and collect from standard entry
*** - Then reallocs everything to get the new finished string
*** - And checks if the closing matching quote is found
*** - if yes, end, otherwise, the loop continues
*/

void		ft_new_prompt(char **cmd, char type_quote, t_dlist **history)
{
	int		ret;
	char	*line;
	char	*tmp;

	while (42)
	{
		ret = get_line_term(&line, ft_manage_prompt(type_quote), history);
		if (line && ft_strlen(line) > 0)
		{
			tmp = *cmd;
			*cmd = ft_strjoin(tmp, "\n");
			free(tmp);
			tmp = *cmd;
			*cmd = ft_strjoin(tmp, line);
			free(tmp);
		}
		free(line);
		line = NULL;
		if (!(type_quote = ft_count_quote(*cmd))
				|| (!line && !type_quote))
			break ;
	}
}

/*
*** - Aim of the function :
*** - First GNL that collects the line on the standard entry
*** - then checks if dquote or squote is needed (ft_manage_dquote)
*** - then checks if the line ends with \
*** - If yes for any of the above, print the next then parse the quotes
*** - return 2 is when user presses \n directly after prompt
*** - returns 3 when ctrl_l key is being pressed, and doesn't print the
*** - \n in this case
*/
void	display_bash(char *str)
{
	int i;

	i = -1;
	ft_putstr_fd("\033[1;32m", 1);
	while (str[++i] != ' ')
		write(1, str + i, 1);
	ft_putstr_fd("\033[1;33m", 1);
	ft_putstr_fd(ft_strchr(str, ' '), 1);
	ft_putstr_fd("\033[0;m", 1);
}

void		ft_get_entire_line(char **cmd, char *str, t_dlist **history)
{
	int		ret;
	char	type_quote;

	display_bash(str);
	ret = get_line_term(cmd, str, history);
	if (ret != 2)
	{
		if (ret != 0)
		{
			free(*cmd);
			exit(1);
		}
		else if (*cmd && ft_strlen(*cmd) > 0)
			if ((type_quote = ft_count_quote(*cmd)))
			{
				ft_dlstadd(history, ft_dlstnew(*cmd));
				ft_new_prompt(cmd, type_quote, history);
				if (!g_keeprun && ((*history) = (*history)->next))
					ft_dlstdelone(&(*history)->prev);
			}
	}
	ft_putchar_fd('\n', 1);
}
