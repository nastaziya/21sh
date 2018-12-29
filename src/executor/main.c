#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include "../../inc/expansion.h"
#include <fcntl.h>

int		out_red(t_command cmd, t_env_tools *env, int i, int *aux)
{
	int out =-1;
	struct stat s;
	int saved_stdout;
	char **files;
	int j;
	int test;
	
	j = 0;
	*aux = 0;
	while (j < cmd.command[i].redirection.used_space)
	{
		files = expense_red_files(cmd, *env, i);
		if (lstat(files[j], &s) == -1 && files[j] && ft_strchr(files[j], '/'))
		{
			print_errors("No such file or directory", files[j]);
			return (-2);
		}
		if ((*aux = dup(cmd.command[i].redirection.fd[j])))
		{
			if (*aux > -1)
			{
				saved_stdout = *aux;
				out = open(files[j], O_WRONLY | O_CREAT | O_TRUNC,
   	    		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
				*aux = j;
			}
			else
			{
				test = open(files[j], O_WRONLY | O_CREAT | O_TRUNC,
   	   			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
				close(test);
				if (j > 0)
					*aux = j - 1 ;
				else 
					*aux = 0;
			}
		}
		if (j > 0 && j < cmd.command[i].redirection.used_space  - 1)
			close(out);
		j++;
	}

	if (dup2(out, cmd.command[i].redirection.fd[*aux]) == -1)
		return -1;
	if (out)	
		close(out);
	return (saved_stdout);
}

void check_op(t_command cmd, t_env_tools *env)
{
	int 	i;
	// char	*path;
	char 	**cmd_expended;
	int 	out =-1;
	int 	aux = 0;
	// int		ret;

	// path = NULL;
	cmd_expended = NULL;
	i = -1;
	// ret = 0;
	if (cmd.used_space > 0)
		cmd_expended = expense_cmd(cmd, *env, 0);
	// dprintf(2, "redirection_used_space: %d\n", cmd.command[0].redirection.used_space);
//	print_array(cmd.command[0].used_space, cmd_expended);
	if (cmd.command[0].redirection.used_space > 0)
	{
		// dprintf(2, "ENTRE DANS LE OUTRED\n");
		out = out_red(cmd, env, 0, &aux);
		if (out == -2)
			return;
	}
	if (cmd_expended != NULL && cmd.used_space > 0 && !is_built_in(cmd_expended))
		// env->g_return_value = error_exec_or_exec(env->paths, path, cmd_expended, env->env_cpy);
		env->g_return_value = error_exec_or_exec(env->paths, cmd_expended, env->env_cpy, 0);
	else if (cmd_expended && is_built_in(cmd_expended))
	{
		dprintf(2, "[%s]\n", cmd.command[0].cmd_simple[0]);
		env->g_return_value = ft_exec_builtin(env, cmd_expended);
		// env->g_return_value = ft_exec_builtin(env, cmd_expended);
		//stocker valeur de retour : env->g_return_value
		dprintf(2, "builtin\n");
	}
	if (out > -1)
	{
		dup2(out, cmd.command[0].redirection.fd[aux]);
		close(out);
	}
	dprintf(2, "INFOS_boucle: %d - %d\n", i, cmd.used_space);
	while (++i < cmd.used_space && cmd.command[i].tok != -1)
	{
		dprintf(2, "RENTRE MA POULE\n");
		//print_array(cmd.command[i + 1].used_space, cmd.command[i + 1].cmd_simple);
		if (i == cmd.used_space - 1 || cmd.command[i + 1].used_space == 0)
			break;
		cmd_expended = expense_cmd(cmd, *env, i + 1);
		if (cmd.command[i].tok == T_DBLOR)// && !is_built_in(cmd_expended))
		{
			if (env->g_return_value > 0 && is_built_in(cmd_expended))
				env->g_return_value = ft_exec_builtin(env, cmd_expended);
			else if (env->g_return_value > 0)
				env->g_return_value = error_exec_or_exec(env->paths, cmd_expended, env->env_cpy, 0);
		}
		else if (cmd.command[i].tok == T_DBLAND)// && !is_built_in(cmd_expended))
		{
			if (env->g_return_value == 0 && is_built_in(cmd_expended))
				env->g_return_value = ft_exec_builtin(env, cmd_expended);
			else if (env->g_return_value == 0)
			{
				env->g_return_value = error_exec_or_exec(env->paths, cmd_expended, env->env_cpy, 0);
			}
		}
		else if (cmd.command[i].tok == T_SEMI)// && is_built_in(cmd_expended) == 0)
		{
			if (is_built_in(cmd_expended))
				env->g_return_value = ft_exec_builtin(env, cmd_expended);
			else
				env->g_return_value = error_exec_or_exec(env->paths, cmd_expended, env->env_cpy, 0);
		}
		//free(path);
		free_str(cmd_expended);
		if (out > -1)
		{
			dup2(out,  cmd.command[i + 1].redirection.fd[aux]);
			close(out);
		}
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
	keepRunning = 0;
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
		if (keepRunning == 1 || keepRunning == 2)
			env.g_return_value = 1;
		else if (keepRunning == 5)
			env.g_return_value = 258;
			// keepRunning = 5 -> 

	 	// si je n'ai ni fait ctrl_c, de ctrl_d, ou j'ai fait ctrl_d pendant le heredoc 
		//  -> Exécute les commandes
		if (!keepRunning || keepRunning == 4)
			check_op(cmd, &env);
		if (*heredoc)
		{
			ft_free_av(*heredoc);
			*heredoc = NULL;
		}
		free_the_op_content_array_token(&lex);
		free_struct(&cmd);//, lex
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
