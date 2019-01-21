#include "../../inc/sh.h"

void      free_the_op_content_array_token(t_lexer *lexer)
{
  int i;

  i = -1;
  while (++i < lexer->used_size)
  {
	  if(lexer->used_size == 1)
		break;
	  else if (lexer->tokens[i].type != T_WORD && !is_red(*lexer, i))
	  	free(lexer->tokens[i].content);
  }
  free(lexer->tokens);
}

void	free_struct(t_command *cmd)
{
	int i;

	i = 0;
	while (i < cmd->used_space)
	{
		if (cmd->command[i].used_space > 0)
		{
			// dprintf(3, "cmd->command[i].cmd_simple[0]: |%s| - %d\n", cmd->command[i].cmd_simple[1], cmd->command[i].redirection.red[0]);
			free_str(cmd->command[i].cmd_simple);
		}

		// free_str_2(cmd->command[i].redirection.red,
		// cmd->command[i].redirection.used_space);

		// free(cmd->command[i].redirection.red);
		free_str_2(cmd->command[i].redirection.file, 
		cmd->command[i].redirection.used_space);
		free(cmd->command[i].redirection.fd);
		// }
		i++;
	}
	free(cmd->command);
}