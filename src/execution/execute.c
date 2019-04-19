/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 17:59:38 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/18 19:14:28 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		execute_command(int fds[3], t_simple *simple)
{
	int		argc;
	int		status;

	if (OK((status = perform_redirs(fds, simple))))
	{
		if (simple->builtin)
		{
			argc = 0;
			while (simple->command[argc])
				argc += 1;
			simple->exit_status = simple->builtin(argc, simple->command);
		}
		else
			execve(simple->command[0], simple->command, g_environ);
	}
	else
		status = ERR(simple->exit_status) ? ERROR : SUCCESS;
	return (status);
}

int		exec_pipe(int fds[3], t_pipe *pipe)
{
	int		new_fds[3];
	int		status;

	status = SUCCESS;
	ft_memcpy(new_fds, fds, sizeof(int) * 3);
	(void)pipe;
	return (status);
}

int		exec_and(int fds[3], t_operator *operator)
{
	int		new_fds[3];
	int		status;

	status = SUCCESS;
	ft_memcpy(new_fds, fds, sizeof(int) * 3);
	(void)operator;
	return (status);
}

int		exec_or(int fds[3], t_operator *operator)
{
	int		new_fds[3];
	int		status;

	status = SUCCESS;
	ft_memcpy(new_fds, fds, sizeof(int) * 3);
	(void)operator;
	return (status);
}

int		restore_fds(int old_fds[3], int new_fds[3])
{
	(void)old_fds;
	(void)new_fds;
	return (SUCCESS);
}
int		exec_simple_command(int fds[3], t_simple *simple)
{
	int		new_fds[3];
	int		status;
	pid_t	pid;

	status = ERROR;
	ft_memcpy(new_fds, fds, sizeof(int) * 3);
	if (simple->builtin == builtin_exec)
		status = execute_command(new_fds, simple);
	else if (NONE((pid = fork())))
	{
		execute_command(new_fds, simple);
		ft_printf("fork error: %s", simple->command[0]);
		simple->exit_status = ERROR;
		_exit(1);
	}
	else if (OK(pid))
	{
		waitpid(pid, &status, NIL);
		simple->exit_status = (WIFEXITED(status) ? WEXITSTATUS(status) : -1);
		status = restore_fds(fds, new_fds);
	}
	else if (ERR(pid))
		ft_printf("fork error: %s", simple->command[0]);
	return (status);
}


int		execute(int fds[3], t_program *program)
{
	int		i;
	int		status;

	i = -1;
	status = SUCCESS;
	while (OK(status) && program->commands[++i])
	{
		if (program->commands[i]->type == EXEC_PIPE)
			status = exec_pipe(fds, program->commands[i]->pipe);
		else if (program->commands[i]->type == EXEC_AND)
			status = exec_and(fds, program->commands[i]->operator);
		else if (program->commands[i]->type == EXEC_OR)
			status = exec_or(fds, program->commands[i]->operator);
		else if (program->commands[i]->type == EXEC_SIMPLE_COMMAND)
			status = exec_simple_command(fds, program->commands[i]->simple_command);
		else
			status = NIL;
	}
	return (status);
}

/*
** Parse and execute commands
*/

int		parse_execute_input(int fds[3], char *complete_cmd)
{
	int			status;
	t_list		*tokens;
	t_ast		ast;
	t_program	program;

	tokens = NULL;
	ft_bzero(&ast, sizeof(t_ast));
	ft_bzero(&program, sizeof(t_program));
	if (!OK((status = lexical_analysis(complete_cmd, &tokens, &ast.missing)))
		|| (!OK((status = syntactic_analysis(&tokens, &ast))))
		|| (!OK((status = semantic_analysis(&ast, &program))))
		|| (!OK((status = execute(fds, &program)))))
		return (status);
	return (status);
}
