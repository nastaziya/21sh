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

static void     up_key_next_norm(t_tcap *caps, char *tmp, int i)
{
    end_key(caps);
    while (caps->sz_str > caps->size_prompt)
        del_key(caps);
    // transforme chaque caractère de l'history en buf, qu'on envoie dans la fonction de print
    while (tmp[++i])
    {
        ft_bzero(caps->buf, 2048);
        caps->buf[0] = tmp[i];
        print_normal_char(caps);
    }
    // avancer :-)
    caps->history[0] = caps->history[0]->next;
    caps->ct_arrow = 2;
}

int			     up_key(t_tcap *caps)
{
    int i;
    char *tmp;
    
    i = -1;
    // if (caps->tmp_str)
    //     free(caps->tmp_str);
    // si je suis bien dans bash et non un sous-shell et que 
    if (caps->size_prompt == 7 && caps->history[0]->next)
    {
        if (caps->ct_arrow == 1 && caps->history[0]->next)
            caps->history[0] = caps->history[0]->next;
        tmp = caps->history[0]->content;
        if (caps->history[0]->prev == NULL && !caps->tmp_str)
            // Première initialisation de l'historique -> faire une sauvegarde de la str
            caps->tmp_str = caps->sz_str > caps->size_prompt ? ft_strdup(caps->str[0]) : ft_strnew(1);
        else if (caps->history[0]->prev == NULL && caps->tmp_str)
        {
            free(caps->tmp_str);
            caps->tmp_str = caps->sz_str > caps->size_prompt ? ft_strdup(caps->str[0]) : ft_strnew(1);
        }
        up_key_next_norm(caps, tmp, i);
    }
    return (0);
}

static void     down_key_norm_1_down_key_still_in_history(t_tcap *caps, char *tmp, int i)
{
    if (caps->history[0]->prev)
        caps->history[0] = caps->history[0]->prev;
    tmp = caps->history[0]->content;
    // supprimer tous les chars
    end_key(caps);
    while (caps->sz_str > caps->size_prompt)
        del_key(caps);
    // transforme chaque caractère de l'history en buf, qu'on envoie dans la fonction de print
    while (tmp[++i])
    {
        ft_bzero(caps->buf, 2048);
        caps->buf[0] = tmp[i];
        print_normal_char(caps);
    }
    // Specify that the last key used is down key and that we still are in the history
    caps->ct_arrow = 1;
}

static void      down_key_norm_2_down_key_not_in_history_print_copy_str(t_tcap *caps, int i)
{
    end_key(caps);
    while (caps->sz_str > caps->size_prompt)
        del_key(caps);
    // transforme chaque caractère de l'history en buf, qu'on envoie dans la fonction de print
    while (caps->tmp_str[++i])
    {
        ft_bzero(caps->buf, 2048);
        caps->buf[0] = caps->tmp_str[i];
        print_normal_char(caps);
    }
    //specify that the last key used is down key, but that we are not anymore in the history
    caps->ct_arrow = 0;
}

int			     down_key(t_tcap *caps)
{
    int     i;
    char    *tmp;
    
    i = -1;
    tmp = NULL;
    // If previous key was up key, to avoid double down arrow, just go down again
    if (caps->size_prompt == 7 && caps->ct_arrow == 2 && caps->history[0]->prev)
        caps->history[0] = caps->history[0]->prev;
    if (caps->size_prompt == 7 && caps->history[0]->prev)
        down_key_norm_1_down_key_still_in_history(caps, tmp, i);
    else if (caps->size_prompt == 7 && caps->tmp_str && caps->tmp_str[0])
        down_key_norm_2_down_key_not_in_history_print_copy_str(caps, i);
    else if (caps->size_prompt == 7 && !caps->history[0]->prev && caps->tmp_str)
    {
        end_key(caps);
        while (caps->sz_str > caps->size_prompt)
            del_key(caps);
        //specify that the last key used is down key, but that we are not anymore in the history
        caps->ct_arrow = 0;
    }
    return (0);
}