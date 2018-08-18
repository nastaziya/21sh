#ifndef PARSER_H
# define PARSER_H
# include "lexer.h"

#define TAB_INITIAL_CAPACITY 3

typedef struct  s_red
{
    int  **fd; //io numb
    char **red;
    char **file;
    int  used_space;
    int  av_space;    
}        t_red;

typedef struct s_simp_com
{
    char  **cmd_simple;
    int   av_space; //capacite  =  100
    int   used_space; //
    int   tok;
   // e_token_type tok;
    t_red redirection; // dynamic array of redirections
}       t_simp_com;

typedef struct  s_command
{
    t_simp_com *command; 
    int av_space;
    int used_space;
}       t_command;

#endif