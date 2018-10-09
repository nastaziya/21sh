#include "../../inc/expansion.h"

void print_array_2(char **cmd)
{
	int i;

	i = 0;
	while(cmd[i])
	{
		printf("%s\n",cmd[i]);
		i++;
	}
}