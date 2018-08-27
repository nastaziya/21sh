#include "../inc/sh.h"

void check_op(t_command cmd, t_env_tools *env)
{
	int i;
	char	*path;

	path = NULL;
	i = -1;
	while(++i < cmd.used_space)
	{
		error_exec_or_exec(env->paths, path, cmd.command[i].cmd_simple, env->env_cpy);
	}
}
void	all_exec(char **environ)
{

	t_lexer lex;
	t_command cmd;
	t_env_tools	env;
		char	*path;

	path = NULL;

	env.env_cpy = copy_env(environ, size_str(environ));
	path_str(env.env_cpy, &env.paths);
	lex = final_tokens();
	print(&lex);
	command_init(&cmd);
	add_simple_command(&cmd, lex);
	//print_struct(cmd);
	//error_exec_or_exec(env.paths, path, cmd.command[0].cmd_simple, env.env_cpy);
	check_op(cmd, &env);

	free_the_content_array_token(&lex);
	if (env.env_cpy != NULL)
		free_str(env.env_cpy);

}
int main(int argc, char **argv, char **environ)
{
	/*t_lexer lex;
	t_command cmd;
	t_env_tools	env;
	char	*path;

	path = NULL;
	env.env_cpy = copy_env(environ, size_str(environ));
	path_str(env.env_cpy, &env.paths);
	lex = final_tokens();
	//print(&lex);
	command_init(&cmd);
	add_simple_command(&cmd, lex);
	error_exec_or_exec(env.paths, path, cmd.command[0].cmd_simple, env.env_cpy);
	//print_struct(cmd);
	free_the_content_array_token(&lex);
	if (env.env_cpy != NULL)
		free_str(env.env_cpy);*/
	all_exec(environ);
}
