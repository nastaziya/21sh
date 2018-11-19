#include "../../inc/sh.h"


void check_op(t_command cmd, t_env_tools *env)
{
	int i;
	char	*path;
	char **cmd_expended;

	path = NULL;
	cmd_expended = NULL;
	i = -1;
	if (cmd.used_space > 0)
		cmd_expended = expense_cmd(cmd, *env, 0);
//	print_array(cmd.command[0].used_space, cmd_expended);
	if (cmd_expended != NULL && cmd.used_space > 0 && !is_built_in(cmd, 0))
		env->g_return_value = error_exec_or_exec(env->paths, path, cmd_expended, env->env_cpy);
	else if (cmd_expended && is_built_in(cmd, 0))
	{
		//stocker valeur de retour : env->g_return_value
		printf("builtin\n");
	}
	while(++i < cmd.used_space && cmd.command[i].tok != -1)
	{
		//print_array(cmd.command[i].used_space, cmd_expended);
		if (i  == cmd.used_space -1)
			break;
		cmd_expended = expense_cmd(cmd, *env, i + 1);
		if (is_built_in(cmd, i + 1) == 1)
			printf("privet\n");

		else if (cmd.command[i].tok == T_DBLOR && !is_built_in(cmd, i))
		{
			//check_path(env->paths, &path,  cmd.command[i].cmd_simple) == 0 &&
			if (env->g_return_value > 0)
				env->g_return_value = error_exec_or_exec(env->paths, path, cmd_expended, env->env_cpy);
		}
		else if (cmd.command[i].tok == T_DBLAND && !is_built_in(cmd, i))
		{
			//check_path(env->paths, &path,  cmd.command[i].cmd_simple) > 0 &&
			if ( env->g_return_value == 0)
			{
				env->g_return_value = error_exec_or_exec(env->paths, path, cmd_expended, env->env_cpy);
			}
		}
		else if (cmd.command[i].tok == T_SEMI && is_built_in(cmd, i) == 0)
			env->g_return_value = error_exec_or_exec(env->paths, path, cmd_expended, env->env_cpy);
		//free(path);
		free_str(cmd_expended);
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
	while (42)
	{
		lex = final_tokens();
		// final_tokens(&lex);
		//print(&lex);
		command_init(&cmd);
		add_simple_command(&cmd, lex);
		//print_struct(cmd);

		check_op(cmd, &env);
		free_the_op_content_array_token(&lex);
		free_struct(&cmd, lex);		
		
	}
}
int main(int argc, char **argv, char **environ)
{
	all_exec(environ);
}
