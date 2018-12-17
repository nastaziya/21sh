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
    // char *tmp;
    // int curs_pos[2];
 
    // replacer correctement le prompt en pos y
    // pour ce faire :
    // Faire un end_key();
	keepRunning = 0;
    end_key(&caps);
    
    // calculer les positions
    // cursor_position(curs_pos);
    // dprintf(2, "%")
    // size_windows(&caps);
    // dprintf(2, "ctrl_c: %d-%d\n", curs_pos[1], caps.window_size[0]);
    // faire une copie de la str et la print si nécessaire
    // if (caps.sz_str > caps.size_prompt)
    // {
    //     // free(caps.tmp_str);
        // tmp = ft_strdup(caps.str[0]);
    //     dprintf(2, "tmp_signal: |%s|\n", tmp);
    //     // ft_bzero(caps.tmp_str, ft_strlen(caps.tmp_str));
    //     free(caps.tmp_str);
    //     caps.tmp_str = NULL;
    //     // tmp = ft_strdup(caps.str[0]);
    //     // remettre l'historique sur le pointeur de départ;
    // while (caps.history[0]->prev)
    //     down_key(&caps);
    // down_key(&caps);
        // while (caps.sz_str > caps.size_prompt)
        // {
        //     // dprintf(2, "ça passe");
        //     del_key(&caps);
        // }
    //     ft_putstr_fd(tmp, 1);
    //     free(tmp);
    // }

    
    char c = 10;
    ioctl(0, TIOCSTI, &c);
        // down_key(&caps);
 
    
    // 1. si je suis en milieu de fenêtre -> caps.y_prompt
    // 2. si je suis en fin, je garde le même y (if )
    // caps.y_prompt = (curs_pos[1] == caps.window_size[0] ? curs_pos[1] : curs_pos[1] + 1);
    // calculer la pos.y
    
    // calculer taille fenêtre
    
    // dprintf(2, "ctrl_c: %d-%d\n", caps.curs_pos[1], caps.window_size[0]);
    // caps.y_prompt++;
    // dprintf(1, "\nbash > ");
    // end_key(&caps);
    // tputs(tgoto(tgetstr("cm", NULL), 0, 0), 0, ft_outc);
    // tputs(tgetstr("cd", NULL), 1, ft_outc);
    // size_windows(&caps);
    // ft_putstr_fd(caps.prompt, 1);
    // caps.cursor = caps.size_prompt + 1;
    // position_char_in_window_print_inside_string(caps.cursor, &caps, caps.sz_str, 0);
    // if ((caps.char_pos[0] < caps.window_size[1] && caps.char_pos[1] < caps.window_size[0]) 
    //     && caps.window_size[1] > caps.size_prompt)
    // {
    //     if (caps.sz_str > caps.size_prompt)
    //         ft_putstr_fd(caps.str[0],1);
    // }
    // else
    // {
    //     tputs(tgoto(tgetstr("cm", NULL), 0, 0), 0, ft_outc);
    //     tputs(tgetstr("cd", NULL), 1, ft_outc);
    //     ft_putstr_fd("make a larger screen",1);
    // }
    // caps.y_prompt = 0;
    // caps.cursor = caps.sz_str;
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