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
*** - Aim of the function : free the dynamic array for the leaks :-)
*/

void		free_the_content_array_token(t_lexer *lexer)
{
	int	i;

	i = -1;
	while (++i < lexer->used_size)
		free(lexer->tokens[i].content);
	free(lexer->tokens);
}

/*
*** - Aim of the function : initialize the struct
*/

void		lexer_init(t_lexer *lexer)
{
	lexer->used_size = 0;
	lexer->capacity = LEXER_INITIAL_CAPACITY;
	if (!(lexer->tokens = malloc(sizeof(t_lexer_token) * lexer->capacity)))
		return ;
}