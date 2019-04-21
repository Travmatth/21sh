/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 17:59:38 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/20 20:32:26 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		exec_command(int fds[3], t_simple *simple)
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
	return (status);
}

int		exec_simple_command(int fds[3], t_simple *simple)
{
	int		new_fds[3];
	int		status;
	pid_t	pid;

	status = ERROR;
	ft_memcpy(new_fds, fds, sizeof(int) * 3);
	if (!OK((status = open_redirs(fds, simple))))
		return (status);
	else if (simple->builtin == builtin_exec)
		status = exec_command(new_fds, simple);
	else if (NONE((pid = fork())))
	{
		exec_command(new_fds, simple);
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

int		execute_switch(int fds[3], t_exec_node *node)
{
	int		status;

	status = SUCCESS;
	if (node->type == EXEC_PIPE)
		status = exec_pipe(fds, node->pipe);
	else if (node->type == EXEC_AND)
		status = exec_logical(fds, node->operator);
	else if (node->type == EXEC_SIMPLE_COMMAND)
		status = exec_simple_command(fds, node->simple_command);
	else
		status = NIL;
	return (status);
}

int		exec_pipe(int fds[3], t_pipe *pipe)
{
	int		new_fds[3];
	int		pipe_fd[2];
	int		status;

	status = SUCCESS;
	// open in and out
	if (ERR(pipe(fds, new_fds)))
		return (ERROR);
	// save original file descriptors
	ft_memcpy(new_fds, fds, sizeof(int) * 3);
	if (ERR((fds[STDOUT] = dup(fds[STDOUT])))
		|| ERR((fds[STDIN] = dup(fds[STDIN]))))
		return (ERROR);
	// replace stdout left half with pipe_in
	if (ERR((new_fds[STDOUT] = dup2(pipe_fd[PIPE_READ], new_fds[STDOUT]))))
		return (ERROR);
	// exec left side
	if (ERR((pipe->exit_status = execute_switch(new_fds, pipe->left))))
		return (ERROR);
	// replace stdin of right half with pipe_out
	if (ERR((new_fds[STDIN] = dup2(pipe_fd[PIPE_WRITE], new_fds[STDIN]))))
		return (ERROR);
	status = OK(status) ? execute_switch(new_fds, pipe->right) : status;
	// close pipes
	if (ERR(close(pipe_fd[PIPE_READ])))
		return (ERROR);
	if (ERR(close(pipe_fd[PIPE_WRITE])))
		return (ERROR);
	// restore original fds, overwrite new
	if (ERR(dup2(fds[STDIN], new_fds[STDIN]))
		|| ERR(dup2(fds[STDOUT], new_fds[STDOUT])))
		return (ERROR);
	return (status);
}

int		exec_logical(int fds[3], t_operator *operator)
{
	int		status;
	int		not;

	not = operator->type == EXEC_OR ? TRUE : FALSE;
	operator->exit_status = execute_switch(fds, pipe->left);
	operator->exit_status = not && operator->exit_status
		? SUCCESS
		: execute_switch(fds, operator->right);
	return (operator->exit_status);
}

int		execute(int fds[3], t_program *program)
{
	int		i;
	int		status;

	i = -1;
	status = SUCCESS;
	while (OK(status) && program->commands[++i])
		status = execute_switch(fds, program->commands[i]);
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
