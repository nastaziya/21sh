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

/*
** Handles the resizing of the terminal window.
** do not forget to free the copy of the prompt when exit termcaps
** TO MANAGE -> when the process is running, or quit the canonical mode, do nothing (return)
*/

void    win_resize(int sig)
{
    (void)sig;
 
    end_key(&caps);
    tputs(tgoto(tgetstr("cm", NULL), 0, 0), 0, ft_outc);
    tputs(tgetstr("cd", NULL), 1, ft_outc);
    size_windows(&caps);
    ft_putstr_fd(caps.prompt, 1);
    caps.cursor = caps.size_prompt + 1;
    position_char_in_window_print_inside_string(caps.cursor, &caps, caps.sz_str, 0);
    if ((caps.char_pos[0] < caps.window_size[1] && caps.char_pos[1] < caps.window_size[0]) 
        && caps.window_size[1] > caps.size_prompt)
    {
        if (caps.sz_str > caps.size_prompt)
            ft_putstr_fd(caps.str[0],1);
    }
    else
    {
        tputs(tgoto(tgetstr("cm", NULL), 0, 0), 0, ft_outc);
        tputs(tgetstr("cd", NULL), 1, ft_outc);
        ft_putstr_fd("make a larger screen",1);
    }
    caps.y_prompt = 0;
    caps.cursor = caps.sz_str;
}

/*
** Handles the resizing of the terminal window.
** do not forget to free the copy of the prompt when exit termcaps
** TO MANAGE -> when the process is running, or quit the canonical mode, do nothing (return)
*/

void    ctrl_c(int sig)
{
    (void)sig;
    char c;

    c = 10;
    if (keepRunning != 3)
    {
        ft_putchar_fd('\n', 1);
        return ;
    }
	keepRunning = 1;
    end_key(&caps);
    dprintf(2, "|%s|\n", caps.prompt);
    // to manage bash > oui " -> the way I did it ...
    // a string like that would launch the dquote shell
    // That's my way of solving it
    if (caps.str[0])
        free(caps.str[0]);
    caps.str[0] = ft_memalloc(1);
    caps.sz_str = ft_strlen(caps.prompt);
    // caps.str[0] = ft_strdup("o");
    if (!ft_strcmp(caps.prompt, "dquote > "))
    {
        caps.sz_str = 10;
        free(caps.str[0]);
        caps.str[0] = ft_strdup("\"");
        keepRunning = 2;
    }
    else if (!ft_strcmp(caps.prompt, "squote > "))
    {
        caps.sz_str = 10;
        free(caps.str[0]);
        caps.str[0] = ft_strdup("\'");
        keepRunning = 2;
    }
    else if (!ft_strcmp(caps.prompt, "Missing arguments > "))
    {
        caps.sz_str = 10;
        free(caps.str[0]);
        caps.str[0] = ft_strdup("oui");
        keepRunning = 2;
    }
    ioctl(0, TIOCSTI, &c);
}

/*
*** Handles the resizing of the terminal window and the ctrl_c signal
*/

int     initialize_signals(void)
{
    signal(SIGWINCH, win_resize);
    signal(SIGINT, ctrl_c);
    return 1;

}