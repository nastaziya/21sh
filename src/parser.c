#include "../inc/sh.h"

/*void    tab_init(char **command, int capacity, int used_size)
{
    used_size = 0;
    capacity = TAB_INITIAL_CAPACITY;
    command = malloc(sizeof(void) * capacity);
}*/

int main()
{
    t_lexer lex;
    t_command cmd;
    lex = final_tokens();
    print(&lex);
    tab_init(&cmd, cmd.command);    
}