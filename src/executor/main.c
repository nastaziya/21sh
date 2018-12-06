#include "../../inc/sh.h"
#include "../../inc/builtin.h"

void check_op(t_command cmd, t_env_tools *env)
{
	int 	i;
	// char	*path;
	char 	**cmd_expended;
	// int		ret;

	// path = NULL;
	cmd_expended = NULL;
	i = -1;
	// ret = 0;
	if (cmd.used_space > 0)
		cmd_expended = expense_cmd(cmd, *env, 0);
//	print_array(cmd.command[0].used_space, cmd_expended);
	if (cmd_expended != NULL && cmd.used_space > 0 && !is_built_in(cmd_expended))
		// env->g_return_value = error_exec_or_exec(env->paths, path, cmd_expended, env->env_cpy);
		env->g_return_value = error_exec_or_exec(env->paths, cmd_expended, env->env_cpy);
	else if (cmd_expended && is_built_in(cmd_expended))
	{
		dprintf(2, "[%s]\n", cmd.command[0].cmd_simple[0]);
		env->g_return_value = ft_exec_builtin(env, cmd_expended);
		// env->g_return_value = ft_exec_builtin(env, cmd_expended);
		//stocker valeur de retour : env->g_return_value
		dprintf(2, "builtin\n");
	}
	while(++i < cmd.used_space && cmd.command[i].tok != -1)
	{
		//print_array(cmd.command[i].used_space, cmd_expended);
		if (i  == cmd.used_space -1)
			break;
		cmd_expended = expense_cmd(cmd, *env, i + 1);
		if (is_built_in(cmd_expended) == 1)
			// env->g_return_value = ;
			dprintf(2, "privet\n");
		else if (cmd.command[i].tok == T_DBLOR && !is_built_in(cmd_expended))
		{
			//check_path(env->paths, &path,  cmd.command[i].cmd_simple) == 0 &&
			if (env->g_return_value > 0)
				// env->g_return_value = error_exec_or_exec(env->paths, path, cmd_expended, env->env_cpy);
				env->g_return_value = error_exec_or_exec(env->paths, cmd_expended, env->env_cpy);
		}
		else if (cmd.command[i].tok == T_DBLAND && !is_built_in(cmd_expended))
		{
			//check_path(env->paths, &path,  cmd.command[i].cmd_simple) > 0 &&
			if ( env->g_return_value == 0)
			{
				// env->g_return_value = error_exec_or_exec(env->paths, path, cmd_expended, env->env_cpy);
				env->g_return_value = error_exec_or_exec(env->paths, cmd_expended, env->env_cpy);
			}
		}
		else if (cmd.command[i].tok == T_SEMI && is_built_in(cmd_expended) == 0)
			// env->g_return_value = error_exec_or_exec(env->paths, path, cmd_expended, env->env_cpy);
			env->g_return_value = error_exec_or_exec(env->paths, cmd_expended, env->env_cpy);
		//free(path);
		free_str(cmd_expended);
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
	env.env_cpy = copy_env(environ, size_str(environ), &env);              
	path_str(env.env_cpy, &env.paths);
	// cpy_home(&env);
	history = ft_dlstnew(NULL);
	while (42)
	{
		lex = final_tokens(&history);
		// final_tokens(&lex);
		// print(&lex);
		command_init(&cmd);
		add_simple_command(&cmd, lex, &history, heredoc);
		//print_struct(cmd);
		//debug heredoc
		// int i = -1;
		// int j;
		// while (heredoc[0] && heredoc[0][++i])
		// {
		// // 	// j = -1;
		// // 	// dprintf(2, "i de heredoc: %d\n", i);
		// // 	// while (heredoc[i] && heredoc[i][++j])
		// // 	// {
		// // 		// dprintf(2, "i: %d - j de heredoc: %d\n", i, j);
		// 		dprintf(2, "AFFICHE HEREDOC: [%s]\n", heredoc[0][i]);
		// // 	// }
		// }
		// debug
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
	// à free -> copier-coller - heredoc
}

int main(int argc, char **argv, char **environ)
{
	 t_term		term;
	 char **heredoc;

	(void)argc;
	(void)argv;
	heredoc = NULL;
	// //Initialisation du termios
	// Protection if the program is launched with the env -i option :
	// env -i ./a.out 2&>/dev/ttys001
	if (terminal_data(&term) < 0)
	{
		ft_putstr_fd("envoie les bonnes infos ma gueule !!!\n", 2);
		exit(0);
	}
	all_exec(environ, &heredoc);
}
