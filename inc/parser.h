#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"
# include  	"../libft/libft.h"

#define TAB_INITIAL_CAPACITY 60

typedef struct      s_exec_redir
{
    int         out;
	// struct stat s;
	int         saved_stdout;
	char        **files;
	int         j;
	int         test;
    
    // function check_op
    int 	    aux;
    int         f_out;
}                   t_exec_redir;

typedef struct      s_red
{
    int          *fd; //io numb
    e_token_type *red;  
    char         **file; // save the name of the file for the >, >>, < operand but also the fd_out in the case of the aggregator
    int          used_space;
    int          av_space;    
}                   t_red;

typedef struct      s_simp_com
{
    char  **cmd_simple;
    int   av_space; //capacity
    int   used_space; 
    int   tok;
    t_red redirection; // dynamic array of redirections
}                   t_simp_com;

typedef struct      s_command
{
    t_simp_com *command;
    int av_space;
    int used_space;
}                   t_command;

/*
*** - struct only built for the norm of
*** - the heredoc
*/

typedef struct  s_hdoc
{
    int		i; // index socké dans words et représente le mot à strcmp
    int		j; // index qui s'arrête quand tombe sur || && ;
    // int		l; // index for the realloc
    int		command; // index pour savoir dans quel * du char *** on est
    int		words[50]; // Tableau qui stocke les différents index des mots clés à strcmp du heredoc
    int		i_words; // index dans le tableau words pour quand on rentre les données
    int		k; // compteur pour savoir l'index d'où on est dans le get_line
    char	*cmd;// pour le get_line
    int     obool; // bool for the \n management
    // char	**tmp; // tmp for the realloc of char*
}               t_hdoc;

void    print_array(int size, char **cmd);
void    print_struct(t_command cmd);
void    tab_init(t_simp_com *simple_cmd);
void    tab_red_init(t_red *redir);
void    tab_assign(t_simp_com *simple_cmd, t_lexer lex, int j);
void    simple_cmd_assign(t_command *cmd, t_simp_com simple_cmd);

/*
*** - utils_parser.c
*/

void    command_init(t_command *cmd);
void    tab_red_assign(t_red *redir, t_lexer lex, int j, int k);

void    add_simple_command(t_command *cmd, t_lexer lex, t_dlist **history, char ***heredoc);
int		is_red(t_lexer lex, int i);
int		is_op(t_lexer lex, int i);
int     parse_errors(t_lexer lex, int i);
int		check_path(char **path_env, char **path, char **str);
int     is_built_in(char **cmd);
int		ft_parse_error_for_heredoc(t_lexer lex);


#endif