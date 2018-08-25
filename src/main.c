#include "../inc/sh.h"

int main()
{
	t_lexer lex;
	t_command cmd;

	lex = final_tokens();
	print(&lex);
	command_init(&cmd);
	add_simple_command(&cmd, lex);
	print_struct(cmd);
	free_the_content_array_token(&lex);
}
