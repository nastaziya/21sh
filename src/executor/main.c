/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   lexer.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: gurival- <marvin@le-101.fr>                +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2018/09/06 16:48:04 by gurival-     #+#   ##    ##    #+#       */
/*   Updated: 2018/09/07 17:58:18 by gurival-    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../../inc/sh.h"
#include "../../inc/builtin.h"
#include "../../inc/expansion.h"
#include "../../inc/exec.h"

// typedef struct		s_shell
// {
// 	t_input			input;
// 	t_headlst		env;
// 	t_headlst		local_var;
// 	t_erm			terminal;
// 	t_hash_table	*bin_table;
// 	t_history		*history;
// 	t_tokenlst		*tokenlst;
// 	t_cmds			*cmds;
// 	short			sigint;
// 	int				fd_orig[3];
// 	int				nb_binaries;
// 	char			*process_name;
// 	pid_t			last_process_id;
// 	int				exit;
// 	int				exit_status;
// }					t_shell;


// FAIRE UNE GESTION D'ERREUR COMME POUR LES REDIR -> ls | /oo/sss"
// => dans ma fonction d'execution je pense
// bash: /oo/sss: No such file or directory
void check_op(t_command cmd, t_env_tools *env, char ***heredoc)
{
	t_exec_redir t;
	int 	i;

	i = 0;
	t.heredoc = heredoc;
	save_original_fd(&t);
	// dprintf(1, "array: %d - %d\n", cmd.command[0].tok, cmd.command[1].tok);
	ft_first_exec(env, cmd, i, &t);
	restore_original_fd(&t);
// j'itere sur le reste des commandes
	while (++i < cmd.used_space && cmd.command[i].tok != -1)
	{
		// dprintf(1, "array: %d\n", cmd.command[i].tok);
		// break de ma boucle quand c'est fini
		if (i == cmd.used_space - 1 || cmd.command[i + 1].used_space == 0)
			break;
		save_original_fd(&t);
		// gestion ||
		if (cmd.command[i].tok == T_DBLOR)
			ft_or_exec(env, cmd, i, &t);
		// gestion |
		else if (cmd.command[i].tok == T_PIPE)
			;
		// {			
		// 	// bash: /oo/sss: No such file or directory
		// }
		// gestion &&
		else if (cmd.command[i].tok == T_DBLAND)
			ft_and_exec(env, cmd, i, &t);
		// gestion ;
		else if (cmd.command[i].tok == T_SEMI)
			ft_semi_exec(env, cmd, i, &t);
		restore_original_fd(&t);
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
		// print(&lex);
		command_init(&cmd);
		add_simple_command(&cmd, lex, &history, heredoc);
		if (keepRunning == 1 || keepRunning == 2)
			env.g_return_value = 1;
		else if (keepRunning == 5)
			env.g_return_value = 258;
			// keepRunning = 5 -> 

	 	// si je n'ai ni fait ctrl_c, de ctrl_d, ou j'ai fait ctrl_d pendant le heredoc 
		//  -> Exécute les commandes
		if (!keepRunning || keepRunning == 4)
			check_op(cmd, &env, heredoc);
		if (*heredoc)
		{
			ft_free_av(*heredoc);
			*heredoc = NULL;
		}
		free_the_op_content_array_token(&lex);
		free_struct(&cmd);
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
