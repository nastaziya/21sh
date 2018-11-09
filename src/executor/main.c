#include "../../inc/sh.h"

void check_op(t_command cmd, t_env_tools *env)
{
	int i;
	// int res;
	char	*path;
	char **cmd_expended;

	path = NULL;
	i = -1;
	/*if (cmd.used_space > 0 && !is_built_in(cmd, 0))
		res = error_exec_or_exec(env->paths, path, cmd.command[0].cmd_simple, env->env_cpy);
	else if (is_built_in(cmd, 0))
	{
		printf("builtin\n");
	}*/
	//cmd_expended = expense_cmd(cmd, env, 0);
	while(++i < cmd.used_space )//&& cmd.command[i].tok != -1)
	{
		cmd_expended = expense_cmd(cmd, *env, i);
		print_array(cmd.command[i].used_space, cmd_expended);
	/*	if (*cmd.command[i + 1].cmd_simple == NULL)
				break;
		if (is_built_in(cmd, i))
			printf("privet\n");
		if (cmd.command[i].tok == T_DBLOR && !is_built_in(cmd, i))
		{
			if (check_path(env->paths, &path,  cmd.command[i].cmd_simple) == 0 || res > 0)
				res = error_exec_or_exec(env->paths, path, cmd.command[i + 1].cmd_simple, env->env_cpy);
		}
		if (cmd.command[i].tok == T_DBLAND && !is_built_in(cmd, i))
		{
			if (check_path(env->paths, &path,  cmd.command[i].cmd_simple) > 0 && res == 0)
				res = error_exec_or_exec(env->paths, path, cmd.command[i + 1].cmd_simple, env->env_cpy);
		}.*/
		//if (cmd.command[i].tok == T_SEMI && !is_built_in(cmd, i))
		//	printf("hei\n");
			//printf("0 : %s\n",cmd_expended[0]);
			//			printf("1  : %s\n",cmd_expended[1]);
		env->g_return_value = error_exec_or_exec(env->paths, path, cmd_expended, env->env_cpy);
		free(path);
	}
}

void	all_exec(char **environ, char ***heredoc)
{
	t_dlist		*history;
	t_lexer 	lex;
	t_command 	cmd;
	t_env_tools	env;
	char		*path;

	path = NULL;
	env.env_cpy = copy_env(environ, size_str(environ));              
	path_str(env.env_cpy, &env.paths);
	history = ft_dlstnew(NULL);
	while (42)
	{
		lex = final_tokens(&history, heredoc);
		// final_tokens(&lex);
		print(&lex);
		command_init(&cmd);
		add_simple_command(&cmd, lex);
		//print_struct(cmd);
		check_op(cmd, &env);
		free_the_op_content_array_token(&lex);
		free_struct(&cmd, lex);
		//free(lex.tokens);
		// to do -> free heredoc
		
		
	}
	if (env.env_cpy != NULL)
		free_str(env.env_cpy);
	// Je free ici, mais a voir si free plus tard/ autre part (en fonction du exit)
	ft_dlstdel(&history);
}

int main(int argc, char **argv, char **environ)
{
	 t_term		term;
	 char		***heredoc;

	heredoc = NULL;
	// //Initialisation du termios
	terminal_data(&term);
  	modify_terminos(&term);
	all_exec(environ, heredoc);

	reset_termios(&term);
}
