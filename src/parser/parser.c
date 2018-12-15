#include "../../inc/sh.h"
#include "../../inc/expansion.h"
#include "../../inc/builtin.h"

void    command_init(t_command *cmd)
{
	cmd->used_space = 0;
	cmd->av_space = TAB_INITIAL_CAPACITY;
	if (!(cmd->command = malloc(sizeof(t_simp_com) * cmd->av_space)))
		return ;
}

void assign_tok(t_command *cmd, t_lexer lex, int *j, int val_tok)
{
	// (void)lex;// ATTENTION INUTILISE
	simple_cmd_assign(cmd, *cmd->command);
	(*j)++;
	tab_init(&cmd->command[*j]);
	tab_red_init(&cmd->command[*j].redirection);
	cmd->command[*j].tok = val_tok;
}

void    tab_io_assign(t_red *redir, t_lexer lex, int j)
{
	int *temp;
	int i;

	i = -1;	
	temp = redir->fd;
	if (!(redir->fd = (int *)malloc(sizeof(int) * redir->av_space + 1)))
		return ;
	if (redir->fd == NULL)
		exit(EXIT_FAILURE);
		while(++i < redir->used_space)
			redir->fd[i] = temp[i];
		free(temp);
	if (j >= 0 && lex.tokens[j].type == T_IO_NUMB)
		redir->fd[redir->used_space] =  ft_atoi(lex.tokens[j].content);
	else
		redir->fd[redir->used_space] = 1;
}
/*
	add element to the dynamic table of simple commands according to 
	existing operators
	res of this function:
	a dynamic array with operands assigned but without commands yet
	ex: ls | echo hei && diff blabla
	[0]{tok = | ; cmd_simple = NULL}
	[1]{tok = && ; cmd_simple = NULL}
	[2]{tok = -1 ; cmd_simple = NULL}
*/

void    add_token_val(t_command *cmd, t_lexer lex, int *j)
{
	int i;
	
	i = -1;
	*j = -1;
	while (++i < lex.used_size)
	{	
		if (lex.tokens[i].type == T_SEMI)
			assign_tok(cmd, lex, j, T_SEMI);
		if (lex.tokens[i].type == T_PIPE)
			assign_tok(cmd, lex, j, T_PIPE);
		else if(lex.tokens[i].type == T_DBLAND)
			assign_tok(cmd, lex, j, T_DBLAND);
		else if(lex.tokens[i].type == T_DBLOR)
			assign_tok(cmd, lex, j, T_DBLOR);
		else if(lex.tokens[i].type == T_AND)
			assign_tok(cmd, lex, j, T_AND);
		else if (i + 1 == lex.used_size)
			assign_tok(cmd, lex, j, -1);
	}
}
/* (1) and (2) below take dynamic array created in "add_token_val" function 
	and complete array of commands giving values to cmd_simple 
	and redirections if existes*/
/*(1)*/

void	complete_simple_command_and_red(t_command *cmd, t_lexer lex, int i, int *j)
{
	if (i == 0 && lex.tokens[i].type == T_WORD)
		tab_assign(&cmd->command[*j], lex, i);
	else if (lex.tokens[i].type == T_WORD && !is_red(lex, i - 1) &&
			lex.tokens[i - 1].type != T_IO_NUMB)
			tab_assign(&cmd->command[*j], lex, i);
	else if (is_red(lex, i) && lex.tokens[i + 1].type == T_WORD)
	{
		tab_io_assign(&cmd->command[*j].redirection, lex, i - 1);
		tab_red_assign(&cmd->command[*j].redirection, lex, i, i + 1);
	}
	else if(lex.tokens[i + 1].type != T_WORD && is_op(lex,i))
		*cmd->command[*j + 1].cmd_simple = NULL;
	else if (!is_red(lex, i) && lex.tokens[i].type != T_IO_NUMB &&
			lex.tokens[i].type != T_WORD)
			(*j)++;
}

////////////////// HEREDOC

// /*
// *** - Aim of the function :
// *** - Function that counts the number of commands that have an heredoc then mallocs it
// *** - If a other_command != previous command, it means that we met a ; || &&
// *** - so we have to malloc the heredoc for the new command. The first if is for 
// *** - the initialization the first time we meet a <<
// */

int		ft_initialize_heredoc(t_lexer *lexer, char ***heredoc, int other_command, int previous_command)
{
	int		nb_to_malloc;
	int		i;

	nb_to_malloc = 0;
	i = -1;
	if (lexer->used_size > 0)
	{
		while (++i < lexer->used_size)
		{
			if (lexer->tokens[i].type == T_DBL_LESS
			&& other_command == 0 && (other_command++) && (nb_to_malloc++))
				previous_command++;
			else if ((lexer->tokens[i].type == T_DBLOR
				|| lexer->tokens[i].type == T_SEMI
					|| lexer->tokens[i].type == T_DBLAND))
				other_command++;
			else if (lexer->tokens[i].type == T_DBL_LESS
			&& (other_command != previous_command) && (previous_command = other_command))
				nb_to_malloc++;
		}
		// if (heredoc[0])
		// 	free(heredoc[0]);
		if (!(heredoc[0] = (char**)malloc(sizeof(char*) * (nb_to_malloc + 1))))
			return (1);
		i = -1;
		while (++i <= nb_to_malloc)
 			heredoc[0][i] = NULL;
	}
	return (0);
}

// /*
// *** - Aim of the function :
// *** - Function that finds the end of the command and counts the number of heredocs (the number of <<)
// *** - And stores the index of each keyword to match to quit each heredoc
// */

int		ft_find_end_command_and_nb_kewyords(t_hdoc *h, t_lexer *lexer)
{
	//reset table
	ft_memset(h->words, -1, 50);
	h->i = h->j - 1;
	// find the limit of the current command (find separator -> && || ;)
	while (lexer->tokens[h->j].type != T_DBLOR && lexer->tokens[h->j].type != T_DBLAND
		&& lexer->tokens[h->j].type != T_SEMI && h->j < ft_parse_error_for_heredoc(*lexer)) //&& h->j < lexer->used_size - 1
		++(h->j);
	// first passage to see how many heredoc there are in this current command
	// + keep track of the index for each word after <<
	h->i_words = 0;
	while (++(h->i) < h->j)
		if (lexer->tokens[h->i].type == T_DBL_LESS && h->words[h->i_words] == -1)			
		{
			h->words[h->i_words] = h->i + 1;
			(h->i_words)++;
		}
	// set k value at 0 not after for the norm (gain space for next function)
	h->k = 0;
	return (0);
}

// /*
// *** - Aim of the function :
// *** - Function that reallocs the heredoc for the proper command
// */

int		realloc_heredoc(t_hdoc *h, char ***heredoc)
{
	char	*tmp;

	dprintf(2, "avant: [[%s]]\n", heredoc[0][h->command]);
	tmp = heredoc[0][h->command];
	heredoc[0][h->command] = ft_strjoin(tmp, h->cmd);
	free(tmp);
	free(h->cmd);
	tmp = heredoc[0][h->command];
	// free(heredoc[0][h->command]);
	if ((heredoc[0][h->command] = ft_strjoin(tmp, "\n")))
	{
		// dprintf(2, "heredoc complet: %s\n", heredoc[0][h->command]);
		// heredoc[0][h->command] = ft_strjoin(tmp, "\n");
		free(tmp);
		dprintf(2, "après: [[%s]]\n", heredoc[0][h->command]);
		return (1);
	}
	return (0);
}

// /*
// *** - Aim of the function :
// *** - Function that manages the heredoc when we are waiting for the last keyword of the command
// *** - => cat << oui << non -> we are in this function when we are expecting the "non" keyword
// *** - => The if is for when the user typed the closing word
// *** - the else if for when the user typed another keyword -> realloc
// */

int		ft_manage_last_keyword(t_hdoc *h, t_lexer *lexer, char ***heredoc)
{
	char *tmp;
	char *res;
	// quand c'est le mot clé fermant, on free et c'est tout
	
	tmp = ft_strdup(lexer->tokens[h->words[h->k]].content);
	expanded_dynamic_table_heredoc(&tmp, 0);
	res = ft_strdup(tmp);
	if (ft_strcmp(h->cmd, res) == 0)
	{
		(h->k)++;
		free(h->cmd);
		free(tmp);
		free(res);
	}
	// là on realloc
	else
	{
		dprintf(2, "passe iciavant if\n");
		free(tmp);
		free(res);
		// // ici on initialise
		if (heredoc[0][h->command] == NULL)//[0]
		{
			dprintf(2, "PLUS PRECCISEMENT [%s]\n", h->cmd);
			// if (heredoc[0][h->command])
			// 	free(heredoc[0][h->command]);
			heredoc[0][h->command] = ft_strjoin(h->cmd, "\n");
			free(h->cmd);
		}
		// // ici on realloc
		else //// GERER les leaks, normalement pas bon
		// {
		// 	dprintf(2, "ici ausi\n");
			if (realloc_heredoc(h, heredoc))
				return (1);
		// }
	}
	return (0);
}

// /*
// *** - Aim of the function :
// *** - Function that collects the line, manages when we are not at the last keyword of the heredoc
// *** - (cat << oui << non --> when we are expecting the Oui keyword) <= managed in the if
// *** - else if -> manage when last keyword
// */

int		ft_collect_line_and_realloc_heredoc(t_hdoc *h, t_lexer *lexer, char ***heredoc, t_dlist **history)
{
	char *tmp;
	char *res;

	if (h->i_words > 0)
		while (h->k < h->i_words)
		{
			h->obool > 0 ? ft_putstr_fd("\nHeredoc > ", 1) : ft_putstr_fd("Heredoc > ", 1); 
			// if (h->cmd)
			// 	free(h->cmd);
			h->obool > 0 ? get_line_term(&h->cmd, "\nHeredoc > ", history)
				: get_line_term(&h->cmd, "Heredoc > ", history);
			// if (h->cmd)
			// free(h->cmd);
			//si mot clé c'est pas le dernier, free ce que l'on get_line et passer au suivant
			if (h->k < h->i_words - 1)
			{
				//control expansion
				tmp = ft_strdup(lexer->tokens[h->words[h->k]].content);
				expanded_dynamic_table_heredoc(&tmp, 0);
				res = ft_strdup(tmp);
				// expansionner le lexer->tokens[h->words[h->k]].content sur les " et les /
				if (ft_strcmp(res, h->cmd) == 0)
					(h->k)++;
				free(h->cmd);
				free(tmp);
				free(res);
			}
			// quand on arrive au dernier, on collecte les données dans le char***
			else if (h->k == h->i_words - 1)
				ft_manage_last_keyword(h, lexer, heredoc);
			h->obool++;
		}
	return (0);
}

// /*
// *** - Aim of the function :
// *** - Function that manages the heredoc if necesary, will malloc of the proper size
// *** - will first malloc the *** with the proper number of char** (that will be the number of << word with separators)
// *** - => cat << non << oui ; ls ; cat << plus gives 2 char** 
// *** - => cat << non ; ls ; cat << yo ; cat << plus gives 3 char***
// *** -  and builds the proper char***
// *** - En premier, compter le nombre de char** à malloc, et ressortir les cases à remplir
// *** - puis après, 
// */

int		ft_manage_heredoc(t_lexer *lexer, char ***heredoc, t_dlist **history)
{
	t_hdoc	h;
	h.j = 0;
	h.command = 0;
	h.obool = 0;

	ft_initialize_heredoc(lexer, heredoc, 0, 0);
	
	// loop on the different commands (with separators : && || ;)
	while (h.j < lexer->used_size - 1)
	{
		ft_find_end_command_and_nb_kewyords(&h, lexer);
		ft_collect_line_and_realloc_heredoc(&h, lexer, heredoc, history);
		// Go to the next command, and if a << was found in the current command
		// go to the next * of the char*** heredoc
		if (h.j < lexer->used_size - 1)
		{
			(h.j)++;
			if (h.words[0] != -1)
				(h.command)++;
		}
	}
	if (h.obool > 0)
		ft_putstr_fd("\n", 1);
	return (0);
}
////////////////// HEREDOC

/*(2)*/
void    add_simple_command(t_command *cmd, t_lexer lex, t_dlist **history, char ***heredoc)
{
	int size_simple_cmd;
	int i;
	int j;

	i = -1;
	j = 0;
	size_simple_cmd = 0;
	ft_manage_heredoc(&lex, heredoc, history);
	if (parse_errors(lex))
	{
		add_token_val(cmd, lex, &size_simple_cmd);
		while (++i < lex.used_size && j <= size_simple_cmd)
			complete_simple_command_and_red(cmd, lex, i, &j);
		// ft_manage_heredoc(&lex, heredoc, history);
	}
	// ft_free_av(*heredoc);
}