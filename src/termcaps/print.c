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

void            print_end_line(t_tcap *caps, char *string, char *tmp)
{
    tmp = caps->str[0];
    caps->str[0] = ft_strjoin(tmp, string);
    free(tmp);
    free(string);
    caps->sz_str++;
    caps->cursor++;
    cursor_position(caps->curs_pos);
    write(1, caps->buf, 3);
    if (caps->curs_pos[0] == caps->window_size[1])
    {
        position_char_in_window_print_inside_string(caps->cursor, caps, caps->sz_str, 0);
        if (caps->char_pos[0] + 1 == (caps->window_size[1]) && caps->char_pos[1] == caps->window_size[0])
        {
            tputs(tgetstr("sf", NULL), 1, ft_outc);
            //diminues the value of the y_prompt, to keep correct track of the y.position of the origin
            caps->y_prompt--;
        }
        tputs(tgoto(tgetstr("cm", NULL), 0, caps->curs_pos[1]), 1, ft_outc);
    }
    else if (caps->curs_pos[1] == caps->window_size[0] && caps->buf[0] == '\n')
        caps->y_prompt--;
}

static void     print_middle_line_manage_char_end_line(t_tcap *caps)
{
    int tst[2];

    // collects position in window of the last char
    position_char_in_window_print_inside_string(caps->cursor, caps, caps->sz_str, 0);

    // dprintf(2, "bien: cpos[0]: %d - cpos_1: %d - col_win: %d - line_win: %d\n", caps->char_pos[0], caps->char_pos[1], caps->window_size[1], caps->window_size[0]);
    // First manages the char at the end of the string when bottom right of the window, the second part manages when \n arrives at the end of line
    if ((caps->char_pos[0] == 0 && caps->char_pos[1] - 1 == caps->window_size[0])
        || (position_char_in_window_print_inside_string(caps->cursor, caps, caps->sz_str, 1)
            && (caps->char_pos[1] == caps->window_size[0] && caps->last_char == '\n')))
    {
        // saves position
        cursor_position(tst);
        // if cursor.y is not at the end of the window.y, go to the line at the bottom
        if (tst[1] != caps->window_size[0])
            tputs(tgoto(tgetstr("cm", NULL), tst[0] - 1, caps->window_size[0]), 1, ft_outc);
        // scroll up one time
        tputs(tgetstr("sf", NULL), 1, ft_outc);
        //diminues the value of the y_prompt, to keep correct track of the y.position of the origin
        caps->y_prompt--;
        // replace the cursor at the previous position
        tputs(tgoto(tgetstr("cm", NULL), tst[0] - 1, tst[1] - 2), 1, ft_outc);
    }
    // Gère le y-- de la pos du prompt (pour la fonction de fleche gauche), quand on est en bas de la fenetre
    else if (caps->buf[0] == '\n' && caps->char_pos[1]-1 == caps->window_size[0])
        caps->y_prompt--;
}

static void     print_middle_line(t_tcap *caps, char *string, char *tmp, char *tmp2)
{
    // tputs(tgetstr("vi", NULL), 1, ft_outc);
    // saves cursor position
    tputs(tgetstr("sc", NULL), 1, ft_outc);
    // manages substrings
    tmp2 = ft_strdup(caps->str[0] + (caps->cursor - caps->size_prompt));
    tmp = ft_strsub(caps->str[0], 0, (caps->cursor - caps->size_prompt));
    free(caps->str[0]);
    caps->str[0] = ft_strjoin(tmp, string);
    free(tmp);
    free(string);
    tmp = caps->str[0];
    caps->str[0] = ft_strjoin(tmp, tmp2);
    free(tmp);
    // efface le reste
    tputs(tgetstr("cd", NULL), 1, ft_outc);
    // usleep(50000);
    //saves cursor position again, but not in the system tmp
    cursor_position(caps->curs_pos);
    // write the new char
    write(1, caps->buf, 3);// write(1, caps->buf, 4); ??
    // prints the rest (the tmp)
    write(1, tmp2, caps->sz_str - caps->cursor);
    // Incrémente le compteur
    caps->sz_str++;
    //replace le curseur
    tputs(tgetstr("rc", NULL), 1, ft_outc);
    
    //manages char end of line, at x == window_size
    print_middle_line_manage_char_end_line(caps);

    // Déplacer le curseur à droite, et incrémente en même temps
    right_key(caps);
    free(tmp2);
    // tputs(tgetstr("ve", NULL), 1, ft_outc);
}

/*
*** - NEED TO NORM THE FUNCTION, BUT WORKS PROPERLY
*/

int			     print_normal_char(t_tcap *caps)
{
	char		*tmp;
	char		*tmp2;
	char		*string;

    tmp = NULL;
    tmp2 = NULL;
	size_windows(caps);
    position_char_in_window_print_inside_string(caps->cursor, caps, caps->sz_str, 0);
    // dprintf(2, "y_prompt: %d - char_pos[0]: %d - caps_wndow_sz[1]: %d - char_pos[1]: %d - caps_wndow_sz[0]: %d\n", caps->y_prompt, caps->char_pos[0], caps->window_size[1], caps->char_pos[1], caps->window_size[0]);
	if (((caps->buf[0] >= 32 && caps->buf[0] <= 127) || caps->buf[0] == 10) && caps->buf[1] == 0
        && !(((caps->y_prompt - 1) == 0 || (caps->y_prompt) == 0) && (caps->char_pos[0] + 1) == caps->window_size[1]
            && caps->char_pos[1] == caps->window_size[0]) && caps->buf[0] != 9)
	{
		string = ft_strndup(caps->buf, 1);
		if (caps->i == 0) // Initialization of the str, the first time
		{
			caps->str[0] = string;
			++(caps->i);
			caps->sz_str++;
			caps->cursor++;
			write(1, caps->buf, 3);
		}
		else if (caps->cursor < caps->sz_str) // Manages when prints char in the middle of string
            print_middle_line(caps, string, tmp, tmp2);
		else
            print_end_line(caps, string, tmp);
	}
    else if (caps->buf[0] != 9) // escape the tab char
        tputs(tgetstr("bl", NULL), 1, ft_outc);
	return (0);
}

/*
*** - For the cmd + c || cmd + v management ! I modified the size of the buf to 4096
*** - And manages checks if ft_strlen == 1 (normal char) or not
// */

int              print_buf(t_tcap *caps, char *buf)
{
    int i;
    // char str[2048];
    char *str;

    i = -1;
    // ft_memcpy(str, buf, 2048);
    str = ft_strdup(buf);
    if (ft_strlen(str) == 1 && str[0] != 9)
    {
        dprintf(2, "OUlA: %d\n", ft_strlen(caps->buf));
        print_normal_char(caps);
    }
    // // else if (str[0] != 9)
    else if ((str[0] > 32 || str[0] == '\n') && !(str[0] == ';' && str[0] == '[')) //&& str[0] == 'C' str[0] == ' ' && 
    // else
    {
    //     dprintf(2, "passe ici: |%s|\n", buf);
        while (str && str[++i])
        {
            ft_bzero(caps->buf, 2048);
            caps->buf[0] = str[i];
            dprintf(2, "%c", str[i]);
            print_normal_char(caps);
        }
    }
    free(str);
    return (0);
}