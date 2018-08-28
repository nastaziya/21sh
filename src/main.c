#include "../inc/sh.h"

void check_op(t_command cmd, t_env_tools *env)
{
	int i;
	int res;
	int verif;
char	*path;

	path = NULL;
	
	verif = 0;
	i = 0;
	while(i < cmd.used_space)
	{
		res = error_exec_or_exec(env->paths, path, cmd.command[i].cmd_simple, env->env_cpy);
		if (cmd.command[i].tok == T_DBLOR)
		{
			//printf("%d\n", res);
			/*if (error_exec_or_exec(env->paths, path, cmd.command[i].cmd_simple, env->env_cpy))
				printf("error\n");*/
			if ((verif = check_path(env->paths, &path,  cmd.command[i].cmd_simple)) > 0 && res > 0)
			{
				error_exec_or_exec(env->paths, path, cmd.command[i + 1].cmd_simple, env->env_cpy);	
			}

		}
		else if (cmd.command[i].tok == -1 && cmd.used_space == 1)
			error_exec_or_exec(env->paths, path, cmd.command[i].cmd_simple, env->env_cpy);
		i++;
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
	check_op(cmd, &env);
	free_the_content_array_token(&lex);
	if (env.env_cpy != NULL)
		free_str(env.env_cpy);

}
int main(int argc, char **argv, char **environ)
{
	all_exec(environ);
}
