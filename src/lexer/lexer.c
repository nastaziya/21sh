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
	lexer->tokens = malloc(sizeof(t_lexer_token) * lexer->capacity);
}

/*
*** - Aim of the function : tokeniwze the part of the string to the assigned
*** - token, regarding the proper size
*/

void		add_token_to_lexer(t_lexer *lexer, const char *text,
				int text_size, e_token_type type)
{
	t_lexer_token	item;
	t_lexer_token	*temp;
	int				i;

	i = -1;
	item.content = ft_strndup(text, text_size);
	item.content[text_size] = 0;
	item.size = text_size;
	item.type = type;
	if (lexer->used_size == lexer->capacity)
	{
		temp = lexer->tokens;
		lexer->capacity = (lexer->used_size * 3) / 2 + 1;
		lexer->tokens = malloc(sizeof(t_lexer_token) * lexer->capacity + 1);
		if (lexer->tokens == NULL)
			exit(EXIT_FAILURE);
		while (++i < lexer->used_size)
			lexer->tokens[i] = temp[i];
		free(temp);
	}
	lexer->tokens[lexer->used_size] = item;
	++lexer->used_size;
}

/*
*** - Aim of the function : checks if the pointer and the next ones equals one
*** - of the enum, if nothing is found, send an empty struct (not_found);
*/

t_oplist	type_of_token(const char *s)
{
	const t_oplist		*ex_tok;
	t_oplist			not_found;

	ex_tok = existing_token;
	not_found = (t_oplist){0, 0, 0};
	while (ex_tok && ex_tok->op)
	{
		if (ft_strncmp(s, ex_tok->op, ex_tok->size) == 0)
			return (*ex_tok);
		++ex_tok;
	}
	return (not_found);
}

/*
*** - Aim of the function : checks if the number of backslash before a quote
*** - is pair or impair. If pair, returns 0 ; otherwise, returns 1
*/

int 		manage_back_quote(const char *s, const char *begin)
{
	int 		i;
	int 		diff;
	int 		count;
	const char	*dup;

	i = -1;
	count = 0;
	dup = s;
	diff = dup - begin;
	while (++i < diff && --dup)
	{
		if (*dup == '\\')
			count++;
		else if (*dup != '\\')
			break;
	}
	if (count % 2 != 0)
		return (1);
	return (0);
}

//part of the string_to_lexer_quote_function
void		ft_find_closing_quote(const char **s, t_norm *nm)
{
	nm->type_quote = **s;
	while (**s && ++(*s))
		if ((**s == nm->type_quote && nm->type_quote != '\'' &&
			!manage_back_quote(*s, nm->start))
				|| (nm->type_quote == '\'' && **s == '\''))
			break ;
}

void		ft_tokenize_quote_management(const char **s, t_lexer *lexer, t_norm *nm)
{
	if (nm->prev != *s && ++(*s))
		add_token_to_lexer(lexer, nm->prev, *s - nm->prev, T_WORD);
	nm->quote_done = 1;
}

void		ft_string_to_lexer_quote_management(const char **s, t_lexer *lexer,
				t_norm *nm)
{
	while (42)
	{
		ft_find_closing_quote(s, nm);
		if (*s < nm-> end && (*(*s + 1) == '"' || *(*s + 1) == '\'') && ++(*s))
			continue ;
		else if (**s && *(*s + 1) && !(*(*s + 1) >= 8 && *(*s + 1) <= 13)
			&& *(*s + 1) != 32)
		{
			++(*s);
			while (**s && ((**s == '\n' && *(*s - 1) == '\\') ? ++(*s) : *s)
					&& ft_isalnum(*(*s+1)) && *(*s+1) != '"' && *(*s+1) != '\'')
				++(*s);
			if (((*(*s+1) == '"') || (*(*s+1) == '\'')) && ++(*s))
				continue ;
			else if ((*(*s + 1) == 32 || (*(*s+1) >= 8 && *(*s+1) <= 13)))
				++(*s);			
		}
		else if (*s < nm-> end && (*(*s + 1) == 32 || (*(*s+1) >= 8 && *(*s+1) <= 13)))
				++(*s);
		if (((**s >= 8 && **s <= 13) || **s == 32 || !**s) && 
			(*s = ((**s >= 8 && **s <= 13) || **s == 32 || !**s) ? --(*s) : *s))
			break ;
	}
	ft_tokenize_quote_management(s, lexer, nm);
}

void		ft_string_to_lexer_advance(const char **s, t_lexer *lexer,
				t_norm *nm)
{
	*s += nm->current.size;
	if (nm->current.type != T_EAT)
		add_token_to_lexer(lexer, nm->current.op,
				nm->current.size, nm->current.type);
	nm->prev = *s;
	nm->quote_done = 0;
}

void		ft_initialize_nm(const char *s, t_norm *nm)
{
	const char *tmp;

	nm->quote_done = 0;
	nm->prev = s;
	nm->start = s;
	tmp = s;
	while (*tmp)
		++(tmp);
	nm->end = tmp;
}

int			string_to_lexer(const char *s, t_lexer *lexer)
{
	t_norm nm;

	ft_initialize_nm(s, &nm);
	while (s && *s)
	{
		nm.current = type_of_token(s);
		if ((*s == '>' || *s == '<') && (s - nm.start > 0 ?
					ft_isdigit(*(s - 1)) : 0))
			add_token_to_lexer(lexer, nm.prev, s - nm.prev, T_IO_NUMB);
		else if ((*s == '"' || *s == '\'') &&
			!manage_back_quote(s, nm.start))
			ft_string_to_lexer_quote_management(&s, lexer, &nm);
		else if (*s == '\n' && *(s - 1) == '\\' && ++s)
			continue ;
		else if (nm.current.op != 0 && nm.prev != s)
			add_token_to_lexer(lexer, nm.prev, s - nm.prev, T_WORD);
		if (nm.current.op != 0 || nm.quote_done == 1)
			ft_string_to_lexer_advance(&s, lexer, &nm);
		else
			++s;
	}
	if (nm.prev != s)
		add_token_to_lexer(lexer, nm.prev, s - nm.prev, T_WORD);
	return (1);
}

/*
*** - I replaced the for loop into a while one, need to check if it is
*** - ++i inside the while or ++i after as I did, I think it's ok that way
*** - Checked, it's ok now;
*/

void		print(const t_lexer *lexer)
{
	// int	i;

	// i = 0;
	// while (i < lexer->used_size)
	// {
	// 	printf("{ |%s| (%i) } ", lexer->tokens[i].content, lexer->tokens[i].type);
	// 	++i;
	// }
	// // printf("\n");
}

/*
*** - Aim of the function :
*** - The aim is to check if impair number of quotes :
*** -  " ' (impair or pair number of dquotes or squotes)
*** - if everything's allright, returns 0
*** - Otherwise, returns the corresponding char
*** - And checks at the end if the string ends by \ -> if yes, returns \
*/

char		ft_count_quote(char *str)
{
	char	type_quote;
	char	*start;

	type_quote = 0;
	start = str;
	while (str && *str)
	{
		if ((*str == '"' || *str == '\'')
				&& !manage_back_quote(str, start))
		{
			type_quote = *str;
			while (++str && *str)
				if ((*str == type_quote && !manage_back_quote(str, start)
							&& type_quote != '\'')
						|| (type_quote == '\'' && *str == '\''))
					break ;
			if (!*str)
				return (type_quote);
			else
				++str;
		}
		else
			++str;
	}
	return (!*str && manage_back_quote(str, start) ? '\\' : 0);
}

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

			// dprintf(2, "youhou test ici");
	// line = NULL;
	while (42)
	{
		ret = get_line_term(&line, ft_manage_prompt(type_quote), history);
		// if (!line[0])
		// {
		// 	dprintf(2, "passe par la");
		// 	continue ;
		// }
		if (line && ft_strlen(line) > 0)
		{
			tmp = *cmd;
			*cmd = ft_strjoin(tmp, "\n");
			free(tmp);
			tmp = *cmd;
			*cmd = ft_strjoin(tmp, line);
			free(tmp);
		}
		if (!(type_quote = ft_count_quote(*cmd))
				|| (ft_strlen(line) == 0 && type_quote == '\\'))
			break ;
		free(line);
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
	ret = get_line_term(cmd, str[0] == '\n' ? str + 1 : str, history);
	if (ret != 2 && ret != 3)
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
	// if (ret != 3)
	ft_putchar_fd('\n', 1);
}

/*
*** - Aim of the function :
*** - Function that counts the number of commands that have an heredoc then mallocs it
*** - If a other_command != previous command, it means that we met a ; || &&
*** - so we have to malloc the heredoc for the new command. The first if is for 
*** - the initialization the first time we meet a <<
*/

int		ft_initialize_heredoc(t_lexer *lexer, char ****heredoc, int other_command, int previous_command)
{
	int		nb_to_malloc;
	int		i;

	nb_to_malloc = 0;
	i = -1;
	if (lexer->used_size > 0)
	{
		while (++i < lexer->used_size)
		{
			if (lexer->tokens[i].type == T_DBL_LESS && other_command == 0 && (other_command++) && (nb_to_malloc++))
				previous_command++;
			else if ((lexer->tokens[i].type == T_DBLOR
				|| lexer->tokens[i].type == T_SEMI
					|| lexer->tokens[i].type == T_DBLAND))
				other_command++;
			else if (lexer->tokens[i].type == T_DBL_LESS && (other_command != previous_command) && (previous_command = other_command))
				nb_to_malloc++;
		}
		// dprintf(2, "nb_to_malloc: %d\n", nb_to_malloc);
		if (!(*heredoc = (char***)malloc(sizeof(char**) * (nb_to_malloc + 1))))
			return (1);
		i = -1;
		//heredoc[0][nb_to_malloc] = NULL;
		while (++i <= nb_to_malloc)
		{
			heredoc[0][i] = NULL;
		}
		dprintf(2, "ahah: %d - %d", i, nb_to_malloc);
		//ft_memset(heredoc, 1 , nb_to_malloc); // voir si ça ne fait pas planter
		// heredoc[nb_to_malloc] = NULL;
	}
	return (0);
}

/*
*** - Aim of the function :
*** - Function that manages the heredoc if necesary, will malloc of the proper size
*** - will first malloc the *** with the proper number of char** (that will be the number of << word with separators)
*** - => cat << non << oui ; ls ; cat << plus gives 2 char** 
*** - => cat << non ; ls ; cat << yo ; cat << plus gives 3 char***
*** -  and builds the proper char***
*** - En premier, compter le nombre de char** à malloc, et ressortir les cases à remplir
*** - puis après, 
*/

int		ft_manage_heredoc(t_lexer *lexer, char ***heredoc, t_dlist **history)
{
	ft_initialize_heredoc(lexer, &heredoc, 0, 0);

	// trouver le / les mots clés à get_line puis strcmp dessus

	int		i; // index socké dans words et représente le mot à strcmp
	int		j = 0; // index qui s'arrête quand tombe sur || && ;
	// int prev;
	int		l;
	int		command = 0; // index pour savoir dans quel * du char *** on est
	int		words[50]; // Tableau qui stocke les différents index des mots clés à strcmp du heredoc
	int		i_words; // index dans le tableau words pour quand on rentre les données
	int		k; // compteur pour savoir l'index d'où on est dans le get_line
	char	*cmd;// pour le get_line
	char	**tmp; // tmp for the realloc of char*
	// int	has_dblor = 0;

	while (j < lexer->used_size - 1)
	{
		ft_memset(words, -1, 50);
		i = j - 1;
		//DEBUG
		int a = j - 1;
		// prev = j;
		// ft_bzero(words, 2);
		dprintf(2, "dans la boucle ;"); //- prev: %d\n", prev)
		while (lexer->tokens[j].type != T_DBLOR && lexer->tokens[j].type != T_DBLAND
			&& lexer->tokens[j].type != T_SEMI && j < lexer->used_size - 1)
		{
			dprintf(2, "j: %d - used_size: %d\n", j, lexer->used_size);
			++j;
		}
		dprintf(2, "sortie boucle j: %d - word: %s - used_size: %d\n", j, lexer->tokens[j].content,lexer->used_size);
		// premier passage pour savoir nombre de mots clés pour lesquels vérifier, en gardant leur index +
		// si jamais il y a un "<<", sinon passer la suite
		i_words = 0;
		while (++i < j)//j < lexer->used_size && 
		{
			// dprintf(2, "je boucle: %d\n", i);
			if (lexer->tokens[i].type == T_DBL_LESS && words[i_words] == -1)			
			{
				// dprintf(2, "je retntre dans le premier");
				words[i_words] = i + 1;
				i_words++;
			}
		}
		dprintf(2, "TABLEAU words\n");
		//DEBUG
		while (++a < j)
			dprintf(2, "|%d| ", words[a]);
		dprintf(2, "TABLEAU words\n");
		k = 0;
		/// if jamais il y a un heredoc, collecter les infos, et avancer dans le i_words
		// jusqu'à i_words - 1, collecter et free
		// une fois à i_words - 1, 
		dprintf(2, "i_words: %d\n", i_words);
		if (i_words > 0)
		{
			dprintf(2, "rentré dans le if\n");
			dprintf(2, "command: %d\n", command);
			dprintf(2, "avant boucle ++k => k: %d - i_words + 1: %d\n", k, i_words + 1);
			while (k < i_words)// +1
			{
				dprintf(2, "dans boucle ++k => k: %d - i_words + 1: %d\n", k, i_words + 1);
				// dprintf(2, "rentré dans la bouclinf\n");
				l = 0;
				ft_get_entire_line(&cmd, "Heredoc > ", history);
				dprintf(2, "IMPORTANT\n");
				dprintf(2, "words[k]: |%d|", words[k]);
				// if (k < i_words + 1)
				// 	dprintf(2, "[[[[[dans boucle: cmd: |%s| - words[k]: |%s|]]]]]\n", cmd, lexer->tokens[words[k]].content);
				dprintf(2, "IMPORTANT\n");
				//si inférieur au dernier mot clé, free et passer au suivant
				if (k < i_words - 1)
				{
					dprintf(2, "rentré dans le if du while");
					dprintf(2, "[[[[[dans boucle: cmd: |%s| - words[k]: |%s|]]]]]\n", cmd, lexer->tokens[words[k]].content);
					// if (dprintf(2, "||||||||||||||||DEBUG strcmp k < i_words: %d||||||||||\n", !ft_strcoo(cmd, lexer->tokens[words[k]].content)))
					if (ft_strcmp(lexer->tokens[words[k]].content, cmd) == 0)
					{
						// dprintf(2, "||||||||||||||||DEBUG strcmp k < i_words: %d||||||||||\n", ft_strcoo(lexer->tokens[words[k]].content, cmd));
						dprintf(2, "merde j'incrémente et il faut pas\n");
						k++;
					}
					free(cmd);
				}
				// quand on arrive au dernier, on collecte les données dans le char***
				else if (k == i_words - 1)
				{
					dprintf(2, "rentre dans elseif du while => k: %d - i_words: %d\n", k, i_words - 1);
					// dprintf(2, "rentré dans le k == i_words -- %d - %d\n", k, words[k -1]);
					// quand c'est le mot clé fermant, on free et c'est tout
					if (ft_strcmp(cmd, lexer->tokens[words[k]].content) == 0)
					{
						dprintf(2, "rentré dans mot clé fermant k == i_words\n");
						k++;
						free(cmd);
					}
					// là on realloc
					else
					{
						dprintf(2, "rentré dans else k == i_words\n");
						// ici on initialise
						//dprintf(2, "index : %s\n", heredoc[command][0]);
						if (heredoc[command] == NULL)//[0]
						{
							dprintf(2, "k == i_words, else -> initialisation\n");
							if (!(heredoc[command] = (char**)malloc(sizeof(char*) * 2)))
								return (1);
							heredoc[command][0] = cmd;
							heredoc[command][1] = NULL;
						}
						// ici on realloc
						else
						{
							dprintf(2, "k == i_words, else -> realloc\n");
							// compter combien d'éléments il y a 
								while (heredoc[command][l] != NULL)
									++l;
							// puis en rajouter 1
							dprintf(2, "nb_l : %d\n", l);
							// tmp the current heredoc
							tmp = heredoc[command];
							//realloc dans le heredoc avec le bon numéro
							if (!(heredoc[command] = (char**)malloc(sizeof(char*) * (l + 2)))) // OU 1, vérifier avec le printf
								return (1);
							l = 0;
							while (tmp[++l] != NULL)
								heredoc[command][l] = tmp[l];
							heredoc[command][l] = cmd;
							heredoc[command][++l] = NULL;
						}
					}
				}
				dprintf(2, "-Un passage-\n");
			}
		}
		dprintf(2, "words[0]:%d - words[1]:%d - i: %d - j: %d\n", words[0], words[1], i, j);
		// ++i;
		// 2ème passage de collecte de get_line_avec realloc (uniquement quand il y a "<<" )
		if (j < lexer->used_size - 1)
		{
			dprintf(2, "un passage\n");
			j++;
			if (words[0] != -1)
				command++;
		}
	}
	return (0);
}


/*
*** - Aim of the function :
*** - Function that manages if the lexing ends up with && || |
*** - if that's the case, we need to show a new prompt, collect a new string
*** -  and lex it
*** - Also manages the "cat << &&" error <= Should not enter in the Missing argument prompt
*** - with this line : 	&& (lexer->used_size > 1 ? lexer->tokens[lexer->used_size - 2].type != T_DBL_LESS : 1))
*** - Also manages the heredoc
*** - Function also adds history
*/

int			ft_manage_string_to_lexer(const char *s, t_lexer *lexer, t_dlist **history, char ***heredoc)
{
	char	*cmd;
	char	*tmp;

	if (!string_to_lexer(s, lexer))
		return (0);
	while (history[0]->prev)
		history[0] = history[0]->prev;
	// ajouter ici historique
	ft_dlstadd(history, ft_dlstnew(s));
	while (42)
	{
		if (lexer->used_size > 0 && lexer->tokens[lexer->used_size - 1].content
		&& (!ft_strcmp(lexer->tokens[lexer->used_size - 1].content, "|")
			|| !ft_strcmp(lexer->tokens[lexer->used_size - 1].content, "&&")
			|| !ft_strcmp(lexer->tokens[lexer->used_size - 1].content, "||"))
			&& (lexer->used_size > 1 ? lexer->tokens[lexer->used_size - 2].type != T_DBL_LESS : 1))
		{
			ft_get_entire_line(&cmd, "Missing arguments > ", history);
			if (cmd && ft_strlen(cmd) > 0)
				if (!string_to_lexer(cmd, lexer))
					ft_putendl_fd("error !", 1);
			// History add, if arguments are missing (realloc)
			tmp = (*history)->content;
			(*history)->content = ft_strjoin(tmp, " ");
			free(tmp);
			tmp = (*history)->content;
			(*history)->content = ft_strjoin(tmp, cmd);
			free(tmp);
			free(cmd);
		}
		else
			break ;
	}
	ft_manage_heredoc(lexer, heredoc, history);
	return (1);
}

/*
*** - Aim of the function :
*** - Function that manages first the collect of the line : ft_get_entire_line
*** - then initialize the lexer : lexer_init
*** - and fills it : string_to_lexer
*/

t_lexer		final_tokens(t_dlist **history, char ***heredoc)
{
	char	*cmd;
	t_lexer	lexer;

	ft_get_entire_line(&cmd, "bash > ", history);
	lexer_init(&lexer);
	if (cmd && ft_strlen(cmd) > 0)
		if (!ft_manage_string_to_lexer(cmd, &lexer, history, heredoc))
			ft_putendl_fd("error !", 1);
	// dprintf(2, "history: %s\n", (*history)->content);
	// if (cmd[0])
	// if (ft_strlen(cmd) > 0)
	free(cmd);
	return (lexer);
}