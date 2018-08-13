#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"

typedef struct  s_op
{
    int t_pipe;
    int t_semi;
    int t_and;
    int t_or;
    int t_ampersand;
}       t_op;

typedef struct  s_red
{
    int  *fd; //io numb
    char *redirection;
    char *file;
    int  used_space;
    int  av_space;    
}        t_red;

typedef struct s_simple_com
{
    char  **command;
    int   av_space; //capacite  =  100
    int   used_space; //
    t_op  oper; // check op
    //int op; a penser
    t_red *redirection; // dynamic array of redirections
}       t_simp_com;

typedef struct  s_command
{
    t_simp_com *command; 
    int av_space;
    int used_space;
}       t_command;

#endif

