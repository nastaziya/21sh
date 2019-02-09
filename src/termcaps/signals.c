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
 
    end_key(&g_caps);
    tputs(tgoto(tgetstr("cm", NULL), 0, 0), 0, ft_outc);
    tputs(tgetstr("cd", NULL), 1, ft_outc);
    size_windows(&g_caps);
    ft_putstr_fd(g_caps.prompt, 1);
    g_caps.cursor = g_caps.size_prompt + 1;
    position_char_in_window_print_inside_string(g_caps.cursor, &g_caps, g_caps.sz_str, 0);
    if ((g_caps.char_pos[0] < g_caps.window_size[1] && g_caps.char_pos[1] < g_caps.window_size[0]) 
        && g_caps.window_size[1] > g_caps.size_prompt)
    {
        if (g_caps.sz_str > g_caps.size_prompt)
            ft_putstr_fd(g_caps.str[0],1);
    }
    else
    {
        tputs(tgoto(tgetstr("cm", NULL), 0, 0), 0, ft_outc);
        tputs(tgetstr("cd", NULL), 1, ft_outc);
        ft_putstr_fd("make a larger screen",1);
    }
    g_caps.y_prompt = 0;
    g_caps.cursor = g_caps.sz_str;
}

/*
*** - Aim of the function : int function to use putchar_fd inside if
*/

static int     ft_putc_if(char c)
{
    ft_putchar_fd(c, 1);
    return (0);
}

/*
*** - Aim of the function : manage the ctrl_c signal
*** - Handles the resizing of the terminal window.
*** - do not forget to free the copy of the prompt when exit termcaps
*/

void    ctrl_c(int sig)
{
    (void)sig;
    char c;

    c = 10;
    // keeprunning == 3 to differenciate the signal when inside termcap
	// and when i give the control to the system
	// (ls -Rl /, then ctrl_c for example)
    // ft_putc_if -> Add the \n in order to keep the termcaps properly
    if (g_keeprun != 3 && !ft_putc_if('\n'))
        return ;
	g_keeprun = 1;
    end_key(&g_caps);
    // to manage bash > oui " -> the way I did it ...
    // a string like that would launch the dquote shell
    // That's my way of solving it
    if (g_caps.str[0])
        free(g_caps.str[0]);
    g_caps.str[0] = ft_memalloc(1);
    g_caps.sz_str = ft_strlen(g_caps.prompt);
    while (g_caps.history[0]->prev)
		g_caps.history[0] = g_caps.history[0]->prev;
    // caps.str[0] = ft_strdup("o");
    if (ft_strcmp(g_caps.prompt, "bash > ") && (g_caps.sz_str = 10))
    {
        // caps.sz_str = 10;
        free(g_caps.str[0]);
        g_keeprun = 2;
        if (!ft_strcmp(g_caps.prompt, "dquote > "))
            g_caps.str[0] = ft_strdup("\"");
        else if (!ft_strcmp(g_caps.prompt, "squote > "))
            g_caps.str[0] = ft_strdup("\'");
        else if (!ft_strcmp(g_caps.prompt, "Missing arguments > "))
            g_caps.str[0] = ft_strdup("oui");
        else if (!ft_strcmp(g_caps.prompt, "\nHeredoc > "))
            g_caps.str[0] = ft_strdup("pppppp");
        // premiere execution heredoc
        else if (!ft_strcmp(g_caps.prompt, "Heredoc > "))
            g_caps.str[0] = ft_strdup("pppppp");
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