/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/09/07 17:58:18 by gurival-    ###    #+. /#+    ###.fr     */
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
		ft_putstr_fd("\ndquote > ", 1);
		return ("dquote > ");
	}
	else if (type_quote == '\'')
	{
		ft_putstr_fd("\nsquote > ", 1);
		return ("squote > ");
	}
	else if (type_quote == '\\')
	{
		ft_putstr_fd("\n> ", 1);
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

void		ft_new_prompt(char **cmd, char type_quote, t_dlist	**history)
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
		//printf ("keepRunning : %d\n", keepRunning);
		// if (keepRunning == 0)
		// {
		// 	return;
		// }
		if (!(type_quote = ft_count_quote(*cmd))
				|| (line && ft_strlen(line) == 0 && type_quote == '\\'))
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

void		ft_get_entire_line(char **cmd, char *str, t_dlist **history)
{
	int		ret;
	char	type_quote;

	ft_putstr_fd(str, 1);
	ret = get_line_term(cmd, str, history); //str[0] == '\n' ? str + 1 : 
	if (ret != 2)//  && ret != 3
	{
		if (ret != 0)
		{
			free(*cmd);
			exit(1);
		}
		else if (*cmd && ft_strlen(*cmd) > 0)
			if ((type_quote = ft_count_quote(*cmd)))
				ft_new_prompt(cmd, type_quote, history);
	}
	ft_putchar_fd('\n', 1);
}