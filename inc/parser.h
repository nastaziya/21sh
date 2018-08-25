#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"

#define TAB_INITIAL_CAPACITY 3

typedef struct  s_red
{
    int  *fd; //io numb
    char **red;  
    char **file;
    int  used_space;
    int  av_space;    
}        t_red;

typedef struct s_simp_com
{
    char  **cmd_simple;
    int   av_space; //capacity
    int   used_space; 
    int   tok;
    t_red redirection; // dynamic array of redirections
}       t_simp_com;

typedef struct  s_command
{
    t_simp_com *command; 
    int av_space;
    int used_space;
}       t_command;

void    print_array(int size, char **cmd);
void    print_struct(t_command cmd);
void    tab_init(t_simp_com *simple_cmd);
void    tab_red_init(t_red *redir);
void    tab_assign(t_simp_com *simple_cmd, t_lexer lex, int j);
void    tab_red_assign(t_red *redir, t_lexer lex, int j, int k);
void    simple_cmd_assign(t_command *cmd, t_simp_com simple_cmd);
void    command_init(t_command *cmd);
void    add_simple_command(t_command *cmd, t_lexer lex);

#endif