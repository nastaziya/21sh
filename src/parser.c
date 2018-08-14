#include "../inc/sh.h"

void    tab_init(t_simp_com *simple_cmd)
{
    simple_cmd->used_space = 0;
    simple_cmd->av_space = TAB_INITIAL_CAPACITY;
    simple_cmd->cmd_simple = malloc(sizeof(char*) * simple_cmd->av_space);
}

void    tab_assign(t_simp_com *simple_cmd, t_lexer lex, int j)
{
    char **temp;

    int i;

    i = -1;
    if (simple_cmd->used_space == simple_cmd->av_space)
    {
        temp = simple_cmd->cmd_simple;
        simple_cmd->av_space = (simple_cmd->used_space * 3) / 2 + 1;
        simple_cmd->cmd_simple = malloc(sizeof(char*) * simple_cmd->av_space + 1);
        if (simple_cmd->cmd_simple == NULL)
            exit(EXIT_FAILURE);
        while(++i < simple_cmd->used_space)
            simple_cmd->cmd_simple[i] = ft_strdup(temp[i]);
        free(temp);
    }
    simple_cmd->cmd_simple[simple_cmd->used_space] = lex.tokens[j].content;
    ++simple_cmd->used_space;
    /*while(i < lex.used_size)
    {
     //   printf("%d\n", lex.tokens[i + 1].type);
            if (lex.tokens[i + 1].type && lex.tokens[i + 1].type == 3 && lex.tokens[i].type == 27)
                simple_command->tok = T_PIPE;
            if(lex.tokens[i].type == 27)
                simple_command->cmd_simple[i] = ft_strdup(lex.tokens[i].content);
        i++;
    }*/
}

void simple_cmd_assign(t_command *cmd, t_simp_com simple_cmd)
{
    t_simp_com *temp;

    int i;

    i = -1;
    if (cmd->used_space == cmd->av_space)
    {
        temp = cmd->command;
        cmd->av_space = (cmd->used_space * 3) / 2 + 1;
        cmd->command = malloc(sizeof(t_simp_com) * cmd->av_space + 1);
        if (cmd->command == NULL)
            exit(EXIT_FAILURE);
        while(++i < cmd->used_space)
            cmd->command[i] = temp[i];
        free(temp);
    }
    cmd->command[cmd->used_space] = simple_cmd;
    ++cmd->used_space;
    /*while(i < lex.used_size)
    {
     //   printf("%d\n", lex.tokens[i + 1].type);
            if (lex.tokens[i + 1].type && lex.tokens[i + 1].type == 3 && lex.tokens[i].type == 27)
                simple_command->tok = T_PIPE;
            if(lex.tokens[i].type == 27)
                simple_command->cmd_simple[i] = ft_strdup(lex.tokens[i].content);
        i++;
    }*/
}


void    add_token_val(t_command *cmd, t_lexer lex, t_simp_com cmd_simp, int *j)
{
    int i = -1;
    //*j = -1;
     while (++i < lex.used_size)
     {
                if (lex.tokens[i].type == 3)
                {
                    simple_cmd_assign(cmd, cmd_simp);
                    (*j)++;
                    tab_init(&cmd->command[*j]);
                    cmd->command[*j].tok = T_SEMI;
                }
                else if(lex.tokens[i].type == 7)
                {
                    simple_cmd_assign(cmd, cmd_simp);
                    (*j)++;
                    cmd->command[*j].tok = T_AND;
                    tab_init(&cmd->command[*j]);

                }
                else if (i + 1 == lex.used_size)
                {
                    simple_cmd_assign(cmd, cmd_simp);
                    (*j)++;
                    cmd->command[*j].tok = -1;
                    tab_init(&cmd->command[*j]);     
                }
     }
  
}
void    add_simple_command(t_command *cmd, t_lexer lex, t_simp_com cmd_simp)
{
    int size_simple_cmd;
    int i;
    int j;

    i = -1;
    j = 0;
    size_simple_cmd = 0;
    add_token_val(cmd, lex, cmd_simp,&size_simple_cmd);
    while(++i < lex.used_size && j < size_simple_cmd)
    {
        printf("%d\n", lex.tokens[i].type);
        if (lex.tokens[i].type == 27)
        {
            tab_assign(&cmd->command[j], lex, i);
        }
        else{
            j++;
        }

    }
    

}

void print_array(int size, char **cmd)
{
    int i = -1;
    while(++i < size)
        printf("%s\n",cmd[i]);
}
void print_struct(t_command cmd)
{
    int i = 0;
    while (i < cmd.used_space)
    {
        printf("tok : %d\n", cmd.command[i].tok);
        print_array(cmd.command->used_space,cmd.command[i].cmd_simple);
        i++;
    }
}
void    command_init(t_command *cmd)
{
    cmd->used_space = 0;
    cmd->av_space = TAB_INITIAL_CAPACITY;
    cmd->command = malloc(sizeof(t_simp_com) * cmd->av_space);
}
void    add_cmd(t_command *cmd, t_lexer lex)
{
    int i;

    i = -1;
    while(++i)
        tab_assign(cmd->command, lex, i);

}
int main()
{
    t_lexer lex;
    t_command cmd;
    int i =  -1;
    lex = final_tokens();
    print(&lex);
    command_init(&cmd);
    // tab_init(cmd.command);

   // add_cmd(&cmd, lex);
  
    add_simple_command(&cmd, lex, *cmd.command);
   // printf("%d\n",cmd.command[1].tok);
    print_struct(cmd);
}