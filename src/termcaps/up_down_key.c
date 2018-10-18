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

int			     up_key(t_tcap *caps)
{
    int i;
    char *tmp;
    
    i = -1;
    // tmp = caps->history[0]->content;
    // dprintf(2, "upkey: %s - %p\n", tmp, caps->history[0]->next);
    // si je suis bien dans bash et non un sous-shell et que 
    if (caps->size_prompt == 7 && caps->history[0]->next) // 7 => size "bash > "
    {
        if (caps->ct_arrow == 1 && caps->history[0]->next)
            caps->history[0] = caps->history[0]->next;
        // if (caps->tmp_str && caps->tmp_str[0])
		//     free(caps->tmp_str);
        tmp = caps->history[0]->content;
        dprintf(2, "[%s]", tmp);
        if (caps->history[0]->prev == NULL && !caps->tmp_str)// si c'est la premiere fois, stocker la str en cours
        {
            // dprintf(2, "entre dans le copy");
            // Première initialisation de l'historique -> faire une sauvegarde de la str
            // caps->tmp_str = caps->sz_str > 0 ? ft_strdup(caps->str[0]) : ft_strnew();
            caps->tmp_str = caps->sz_str > caps->size_prompt ? ft_strdup(caps->str[0]) : ft_strnew(1);
            // Gérer problème no malloc quand vide, et quand history up
        }
        else if (caps->history[0]->prev == NULL && caps->tmp_str)
        {
            free(caps->tmp_str);
            caps->tmp_str = caps->sz_str > caps->size_prompt ? ft_strdup(caps->str[0]) : ft_strnew(1);
        }
        // dprintf(2, "sz_str: %d-sz_prompt: %d\n", caps->sz_str, caps->size_prompt);
        // supprimer tous les chars
        end_key(caps);
        while (caps->sz_str > caps->size_prompt)
            del_key(caps);
        // transforme chaque caractère de l'history en buf, qu'on envoie dans la fonction de print
        // dprintf(2, "tmp: |%s|", tmp);
        while (tmp[++i])
        {
            // dprintf(2, "c: |%c|", tmp[i]);
            ft_bzero(caps->buf, 5);
            caps->buf[0] = tmp[i];
            // dprintf(2, "||%c||", caps->buf[0]);
            print_normal_char(caps);
        }
        // avancer :-)
        caps->history[0] = caps->history[0]->next;
        // caps->ct_arrow = caps->ct_arrow == 3 ? 3 : 2;// probleme 
        caps->ct_arrow = 2;
    }
    return (0);
}

int			     down_key(t_tcap *caps)
{
    int     i;
    char    *tmp;
    
    i = -1;
    // dprintf(2, "je suis pqsse ici");
    // If previous key was up key, to avoid double down arrow, just go down again
    if (caps->size_prompt == 7 && caps->ct_arrow == 2 && caps->history[0]->prev)
        caps->history[0] = caps->history[0]->prev;
    if (caps->size_prompt == 7 && caps->history[0]->prev) // 7 => size "bash > "
    {
        // dprintf(2, "previous content: %s\n", caps->history[0]->prev->prev->content);

        dprintf(2, "je suis pqsse a linteroeir");
        // to avoid unexpected segfault, when we go from history to empty str
        if (caps->history[0]->prev)
            caps->history[0] = caps->history[0]->prev;
        tmp = caps->history[0]->content;
        // dprintf(2, "sz_str: %d-sz_prompt: %d\n", caps->sz_str, caps->size_prompt);
        // supprimer tous les chars
        end_key(caps);
        while (caps->sz_str > caps->size_prompt)
            del_key(caps);
        // transforme chaque caractère de l'history en buf, qu'on envoie dans la fonction de print
        while (tmp[++i])
        {
            // dprintf(2, "qcaui");
            ft_bzero(caps->buf, 5);
            caps->buf[0] = tmp[i];
            print_normal_char(caps);
        }
        // Specify that the last key used is down key and that we still are in the history
        caps->ct_arrow = 1;
        // avancer :-)
    }
    else if (caps->size_prompt == 7 && caps->tmp_str && caps->tmp_str[0])
    {
        dprintf(2, "jyoyoyo");
        end_key(caps);
        while (caps->sz_str > caps->size_prompt)
            del_key(caps);
        // transforme chaque caractère de l'history en buf, qu'on envoie dans la fonction de print
                dprintf(2, "jyoyoyo");
        while (caps->tmp_str[++i])
        {
            // dprintf(2, "qcaui");
            ft_bzero(caps->buf, 5);
            caps->buf[0] = caps->tmp_str[i];
            print_normal_char(caps);
        }
        //specify that the last key used is down key, but that we are not anymore in the history
        caps->ct_arrow = 0;
    }
    else if (caps->size_prompt == 7 && !caps->history[0]->prev && caps->tmp_str)
    {
        // dprintf(2, "merde, passe par ici");
        end_key(caps);
        while (caps->sz_str > caps->size_prompt)
        {
            dprintf(2, "fait chier");
            del_key(caps);
        }
        //specify that the last key used is down key, but that we are not anymore in the history
        caps->ct_arrow = 0;
    }
    return (0);
}