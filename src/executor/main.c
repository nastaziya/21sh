#include "../../inc/sh.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
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

void check_op(t_command cmd, t_env_tools *env, int res)
{
	int i;
	char	*path;
	char **cmd_expended;
	int out =-1;
	int aux = 0;
	
	
	cmd_expended = NULL;
	i = -1;
	env->g_return_value = res;
	if (cmd.used_space > 0)
		cmd_expended = expense_cmd(cmd, *env, 0);
	if (cmd.command[0].redirection.used_space > 0)
	{
		out = out_red(cmd, env, 0, &aux);
		if (out == -2)
			return;
	}
	if (cmd_expended != NULL && cmd.used_space > 0 && !is_built_in(cmd, 0))
		env->g_return_value = error_exec_or_exec(env->paths, path, cmd_expended,  env);
	else if (cmd_expended && is_built_in(cmd, 0))
	{
		printf("builtin\n");
	}
	if (out > -1)
	{
		dup2(out, cmd.command[0].redirection.fd[aux]);
		close(out);
	}
	while(++i < cmd.used_space && cmd.command[i].tok != -1)
	{
		if (i  == cmd.used_space -1)
			break;
		cmd_expended = expense_cmd(cmd, *env, i + 1);
		if (cmd.command[i + 1].redirection.used_space > 0)
		{
			out = out_red(cmd, env, i + 1, &aux);
			if (out == -2)
				return;
		}
		if (is_built_in(cmd, i + 1) == 1)
			printf("privet\n");
		else if (cmd.command[i].tok == T_DBLOR && !is_built_in(cmd, i))
		{
			if (env->g_return_value > 0)
				env->g_return_value = error_exec_or_exec(env->paths, path, cmd_expended,  env);
		}
		else if (cmd.command[i].tok == T_DBLAND && !is_built_in(cmd, i))
		{
			if ( env->g_return_value == 0)
				env->g_return_value = error_exec_or_exec(env->paths, path, cmd_expended,  env);
		}
		else if (cmd.command[i].tok == T_SEMI && is_built_in(cmd, i) == 0)
		{
			env->g_return_value = error_exec_or_exec(env->paths, path, cmd_expended,  env);
		}
		//free(path);
		free_str(cmd_expended);
		if (out > -1)
		{
			dup2(out,  cmd.command[i + 1].redirection.fd[aux]);
			close(out);
		}
	}
	//printf ("ret : %d\n", env->g_return_value);
}
void	all_exec(char **environ)
{

	t_lexer lex;
	t_command cmd;
	t_env_tools	env;
	char	*path;
	int res = 0;

	path = NULL;
	env.env_cpy = copy_env(environ, size_str(environ));              
	path_str(env.env_cpy, &env.paths);
	while (42)
	{
		lex = final_tokens();
		// final_tokens(&lex);
		print(&lex);
		command_init(&cmd);
		res = add_simple_command(&cmd, lex);
		//print_struct(cmd);

		check_op(cmd, &env,  res);
		free_the_op_content_array_token(&lex);
		free_struct(&cmd, lex);		
		
	}
}
int main(int argc, char **argv, char **environ)
{
	all_exec(environ);
}
