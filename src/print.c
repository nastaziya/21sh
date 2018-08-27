#include "../inc/sh.h"


void print_array(int size, char **cmd)
{
	int i;

	i = 0;
	while(i < size)
	{
		if (cmd[i] == NULL)
			printf("je suis null\n");
		printf("%s\n",cmd[i]);
		i++;
	}
}
void print_array_int(int size, int *cmd)
{
	int i;

	i = 0;
	while(i < size)
	{
		printf("%d\n",cmd[i]);
		i++;
	}
}

void print_struct(t_command cmd)
{
	int i = 0;
	int j = 0;
	
	while (j < cmd.used_space)
	{
		printf("COMMAND : \n");
		print_array(cmd.command[j].used_space,cmd.command[j].cmd_simple);
		printf("tok : %d\n", cmd.command[j].tok);
		if (cmd.command[j].redirection.used_space > 0)
		{
			printf("REDIRECTIONS: \n");
			print_array(cmd.command[j].redirection.used_space, cmd.command[j].redirection.red);
			print_array(cmd.command[j].redirection.used_space, cmd.command[j].redirection.file);
			print_array_int(cmd.command[j].redirection.used_space, cmd.command[j].redirection.fd);	
		}
		j++;
	}
}
