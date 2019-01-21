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

/*
*** - Aim of the function :
*** - Initialize the array of pointers function
*/
t_tab		*tab_termcaps(void)
{
	static t_tab ttab[17] = {
		{&left_key, 27, 91, 68, 0, 0, "le"},
		{&right_key, 27, 91, 67, 0, 0, "nd"},
		{&del_key, 127, 0, 0, 0, 0, "del"},
		{&home_key, 27, 91, 72, 0, 0, "home"}, // verify if keyboard at school has the same numbers for home
		{&end_key, 27, 91, 70, 0, 0, "end"},// verify if keyboard at school has the same numbers for end
		{&alt_up_key, 27, 27, 91, 65, 0, "alt_up"},
		{&alt_down_key, 27, 27, 91, 66, 0, "alt_down"},
		{&up_key, 27, 91, 65, 0, 0, "up"},
		{&down_key, 27, 91, 66, 0, 0, "down"},
		{&alt_right_key, 27, 27, 91, 67, 0, "alt_right"},
		{&alt_left_key, 27, 27, 91, 68, 0, "alt_left_key"},
		{&ctrl_l, 12, 0, 0, 0, 0, "ctrl_l"},
		{&alt_x, -30, -119, -120, 0, 0, "alt_x"},
		{&alt_s, -61, -110, 0, 0, 0, "alt_s"},
		{&alt_w, -30, -128, -71, 0, 0, "alt_w"},
		{&alt_p, -49, -128, 0, 0, 0, "alt_p"},
		{NULL, 0, 0, 0, 0, 0, NULL}
	};

	return ((t_tab*)ttab);
}

int		ft_clean(void *s, size_t n)
{
	ft_bzero(s, n);
	return (0);
}

void		ft_initialize_get_line(t_tab **ttab, char *str, t_dlist **history)
{
// Initialisation du tableau de pointeurs sur fonction
	*ttab = tab_termcaps();
// Initialisation de la struct caps
	initialize_caps(&caps, str);
	initialize_signals();
//inclure un printf de prompt pour voir
	caps.history = history;
}

int 		get_line_term_termcaps(char **res, char *str, t_dlist **history)
{
	t_tab		*ttab;
	t_tab		*tmp_tab;

	ft_initialize_get_line(&ttab, str, history);
// Itérer sur le read
	while ((tmp_tab = (ttab - 1)) && !ft_clean(caps.buf, 2048)
		&& (read(0, caps.buf, 2047) >= 0))
	{
		// dprintf(2, "LA: %d %d %d %d %d\n", caps.buf[0], caps.buf[1], caps.buf[2], caps.buf[3], caps.buf[4]);
		if (ENTER_KEY && !end_key(&caps)
			&& ((caps.sz_str - caps.size_prompt) == 0)
				&& (*res = ft_memalloc(2))
					&& (caps.str[0] ? !ft_free(caps.str[0]) : 1)
						&& !ft_free_char_char(caps.str))
				return (2);
		else if (ENTER_KEY && !end_key(&caps))// if (ENTER_KEY)
				break ;
		if (CTRL_D_KEY && ctrl_d_management(&caps))
			break ;
		while ((++tmp_tab)->cmd)
			if (BUF_EQUALS_ARRAY && !(tmp_tab->ptr(&caps)))
				break ;
		if (!tmp_tab->cmd)
			print_buf(&caps, caps.buf);
	}
	return (0);
}

int 		get_line_term(char **res, char *str, t_dlist **history)
{
	t_term		term;
	int			ret;

	terminal_data(&term);
	modify_terminos(&term);
	keepRunning = 3; // int to know if inside termcaps or not
	if ((ret = get_line_term_termcaps(res, str, history)))
		return (ret);
	
	// for the norm
	*res = caps.str[0];
	// FIRST FREES
	if (caps.tmp_str)// && caps.tmp_str[0]
		free(caps.tmp_str);
	caps.tmp_str = NULL;
	if (caps.prompt)
		free(caps.prompt);
	caps.prompt = NULL;
	if (caps.copy_str)
		free(caps.copy_str);
	caps.copy_str = NULL;
	free(caps.str);
	caps.str = NULL;
	// for the norm

	// keeprunning == 3 to differenciate the signal when inside termcap
	// and when i give the control to the system
	// (ls -Rl /, then ctrl_c for example)
	if (keepRunning == 3)
		keepRunning = 0;
	reset_termios(&term);
	return (0);
}