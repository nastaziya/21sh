#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include "../../inc/expansion.h"
#include <fcntl.h>

int		greater_than(t_command cmd, t_env_tools *env, int i, t_exec_redir *t)
{
	struct stat s;
	t->files = expense_red_files(cmd, *env, i);
	if (lstat(t->files[t->j], &s) == -1 && t->files[t->j] && ft_strchr(t->files[t->j], '/'))
	{
		print_errors("No such file or directory", t->files[t->j]);
		return (-2);
	}
	if ((t->aux = dup(cmd.command[i].redirection.fd[t->j])))
	{
		// ecrit dans le dernier fd valide :  ls 1> oui 8> po 7> pi 9> lolo -> ecrit dans oui

		// le fd existe, dup a fonctionne : ls > file
		if (t->aux > -1)
		{
			t->saved_stdout = t->aux;
			// O_TRUNC pour redirection ">" et pour ">>" sans O_TRUNC
			t->out = open(t->files[t->j], O_WRONLY | O_APPEND | O_TRUNC | O_CREAT ,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			t->aux = t->j;
		}
		// ca n'a pas fonctionne : le fd nexiste pas : ls 8> file
		else
		{
			t->test = open(t->files[t->j], O_WRONLY | O_APPEND | O_CREAT ,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			close(t->test);
			if (t->j > 0)
				t->aux = t->j - 1 ;
			else 
				t->aux = 0;
		}
	}
	if (t->j > 0 && t->j < cmd.command[i].redirection.used_space  - 1)
		close(t->out);
	return (0);
}

int		dbl_greater_than(t_command cmd, t_env_tools *env, int i, t_exec_redir *t)
{
	struct stat s;
	t->files = expense_red_files(cmd, *env, i);
	if (lstat(t->files[t->j], &s) == -1 && t->files[t->j] && ft_strchr(t->files[t->j], '/'))
	{
		print_errors("No such file or directory", t->files[t->j]);
		return (-2);
	}
	if ((t->aux = dup(cmd.command[i].redirection.fd[t->j])))
	{
		// ecrit dans le dernier fd valide :  ls 1> oui 8> po 7> pi 9> lolo -> ecrit dans oui

		// le fd existe, dup a fonctionne : ls > file
		if (t->aux > -1)
		{
			t->saved_stdout = t->aux;
			// O_TRUNC pour redirection ">" et pour ">>" sans O_TRUNC
			t->out = open(t->files[t->j], O_WRONLY | O_APPEND | O_CREAT ,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			t->aux = t->j;
		}
		// ca n'a pas fonctionne : le fd nexiste pas : ls 8> file
		else
		{
			t->test = open(t->files[t->j], O_WRONLY | O_APPEND | O_CREAT ,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			close(t->test);
			if (t->j > 0)
				t->aux = t->j - 1 ;
			else 
				t->aux = 0;
		}
	}
	if (t->j > 0 && t->j < cmd.command[i].redirection.used_space  - 1)
		close(t->out);
	return (0);
}

int		out_red(t_command cmd, t_env_tools *env, int i, t_exec_redir *t)
{
	t->out =-1;
	t->j = 0;
	t->aux = 0;
	while (t->j < cmd.command[i].redirection.used_space)
	{
		if (cmd.command[i].redirection.red[t->j] == T_DBL_GREAT)
		{
			dprintf(2, "yesssssss, dblgreaaaaaat\n");
			dbl_greater_than(cmd, env, i, t);
		}
		else if (cmd.command[i].redirection.red[t->j] == T_GREAT)
		{
			dprintf(2, "yesssssss, greaaaaaat\n");
			greater_than(cmd, env, i, t);
		}
		(t->j)++;
	}
	if (dup2(t->out, cmd.command[i].redirection.fd[t->aux]) == -1)
		return (-1);
	if (t->out)
		close(t->out);
	return (t->saved_stdout);
}

void check_op(t_command cmd, t_env_tools *env)
{
	t_exec_redir t;
	int 	i;
	char 	**cmd_expended;
	
	
	t.f_out =-1;
	t.aux = 0;
	// int		ret;

	cmd_expended = NULL;
	i = -1;
	if (cmd.used_space > 0)
		cmd_expended = expense_cmd(cmd, *env, 0);
//	print_array(cmd.command[0].used_space, cmd_expended);
	if (cmd.command[0].redirection.used_space > 0)
	{
		t.f_out = out_red(cmd, env, 0, &t);
		if (t.f_out == -2)
			return;
	}

	if (cmd_expended != NULL && cmd.used_space > 0)
		env->g_return_value = ft_exec_command(env, cmd_expended);
	if (t.f_out > -1)
	{
		dup2(t.f_out, cmd.command[0].redirection.fd[t.aux]);
		close(t.f_out);
	}
	// dprintf(2, "INFOS_boucle: %d - %d\n", i, cmd.used_space);
	while (++i < cmd.used_space && cmd.command[i].tok != -1)
	{
		// dprintf(2, "RENTRE MA POULE\n");
		
		//print_array(cmd.command[i + 1].used_space, cmd.command[i + 1].cmd_simple);
		if (i == cmd.used_space - 1 || cmd.command[i + 1].used_space == 0)
			break;
		cmd_expended = expense_cmd(cmd, *env, i + 1);
		if (cmd.command[i].tok == T_DBLOR)// && !is_built_in(cmd_expended))
		{
			if (env->g_return_value > 0)
			{
				if (cmd.command[i + 1].redirection.used_space > 0)
				{
					t.f_out = out_red(cmd, env, i + 1, &t);
					if (t.f_out == -2)
						return;
				}
				env->g_return_value = ft_exec_command(env, cmd_expended);
			}
			
			
		}
		else if (cmd.command[i].tok == T_PIPE)
		{
			// FAIRE UNE GESTION D'ERREUR COMME POUR LES REDIR -> ls | /oo/sss"
			// bash: /oo/sss: No such file or directory
			
			// A CONSTRUIRE
		}
		else if (cmd.command[i].tok == T_DBLAND)
		{
			if (env->g_return_value == 0)
			{
				if (cmd.command[i + 1].redirection.used_space > 0)
				{
					t.f_out = out_red(cmd, env, i + 1, &t);
					if (t.f_out == -2)
						return;
				}
				env->g_return_value = ft_exec_command(env, cmd_expended);
			}
			
		}
		else if (cmd.command[i].tok == T_SEMI)
		{
			if (cmd.command[i + 1].redirection.used_space > 0)
			{
					t.f_out = out_red(cmd, env, i + 1, &t);
					if (t.f_out == -2)
						return;
			}
			env->g_return_value = ft_exec_command(env, cmd_expended);
		}
		free_str(cmd_expended);
		if (t.f_out > -1)
		{
			dup2(t.f_out,  cmd.command[i + 1].redirection.fd[t.aux]);
			close(t.f_out);
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
