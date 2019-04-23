/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 17:59:38 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/22 18:19:16 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		exec_command(t_simple *simple, int should_exit)
{
	int		argc;
	int		status;

	if (OK((status = perform_redirs(simple))))
	{
		if (simple->builtin)
		{
			argc = 0;
			while (simple->command[argc])
				argc += 1;
			simple->exit_status = simple->builtin(argc, simple->command);
			simple->exit_status = OK(simple->exit_status) ? 0 : 1;
			if (should_exit)
				_exit(simple->exit_status);
		}
		else
			execve(simple->command[0], simple->command, g_environ);
	}
	return (status);
}

// when `exec` called, happens in current process.
// if inside pipe would be inside child process, thus no redirections detected

int		exec_simple_command(t_simple *simple, int *exit_status)
{
	int 	waitpid_return;
	int		status;
	pid_t	pid;

	if (!OK((status = open_redirs(simple))))
		return (status);
	else if (simple->builtin == builtin_exec)
		status = exec_command(simple, FALSE);
	else if (NONE((pid = fork())))
	{
		exec_command(simple, TRUE);
		ft_printf("fork error: %s", simple->command[0]);
		simple->exit_status = ERROR;
		_exit(1);
	}
	else if (OK(pid))
	{
		errno = 0;
		while (ERR((waitpid_return = waitpid(pid, &simple->exit_status, NIL))))
		{
			if (errno == EINTR)
				errno = 0;
			else
				break ;
		}
		if (ERR(waitpid_return))
			simple->exit_status = ERROR;
		else if (WIFEXITED(simple->exit_status))
			simple->exit_status = WEXITSTATUS(simple->exit_status);
		else if (WIFSIGNALED(simple->exit_status))
			simple->exit_status = ERROR;
	}
	else if (ERR(pid))
		ft_printf("fork error: %s", simple->command[0]);
	status = !ERR(status) && !ERR(pid) && !ERR(simple->exit_status) ? SUCCESS : ERROR;
	*exit_status = simple->exit_status;
	return (status);
}

// pipes should fork, execute first command in child and 
// fork again in master to exec second child
// for `exec` command, redirections occur in that child process 

int		exec_pipe(int fds[3], t_pipe *pipe_node, int *exit_status)
{
	int		new_fds[3];
	int		pipe_fd[2];
	int		status;

	status = SUCCESS;
	// open in and out
	if (ERR(pipe(pipe_fd)))
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
	if (ERR((status = execute_switch(new_fds, pipe_node->left, &pipe_node->exit_status))))
		return (ERROR);
	// replace stdin of right half with pipe_out
	if (ERR((new_fds[STDIN] = dup2(pipe_fd[PIPE_WRITE], new_fds[STDIN]))))
		return (ERROR);
	status = OK(status) && NORMAL_CHILD_EXIT(pipe_node->exit_status)
		? execute_switch(new_fds, pipe_node->right, &pipe_node->exit_status)
		: status;
	// close pipes
	if (ERR(close(pipe_fd[PIPE_READ])))
		return (ERROR);
	if (ERR(close(pipe_fd[PIPE_WRITE])))
		return (ERROR);
	// restore original fds, overwrite new
	if (ERR(dup2(fds[STDIN], new_fds[STDIN]))
		|| ERR(dup2(fds[STDOUT], new_fds[STDOUT])))
		return (ERROR);
	*exit_status = pipe_node->exit_status;
	return (status);
}

int		exec_logical(int fds[3], t_operator *operator, int *exit_status)
{
	int		child_exit;

	operator->exit_status = execute_switch(fds, operator->left, &child_exit);
	if (ERROR_CHILD_EXIT(child_exit) && operator->type == EXEC_OR)
		operator->exit_status = execute_switch(fds, operator->right, &child_exit);
	else if (OK(operator->exit_status) && NORMAL_CHILD_EXIT(child_exit) && operator->type == EXEC_AND)
		operator->exit_status = execute_switch(fds, operator->right, &child_exit);
	*exit_status = operator->exit_status;
	return (operator->exit_status);
}

int		execute_switch(int fds[3], t_exec_node *node, int *exit_status)
{
	int		status;

	status = SUCCESS;
	if (node->type == EXEC_PIPE)
		status = exec_pipe(fds, node->pipe, exit_status);
	else if (node->type == EXEC_AND)
		status = exec_logical(fds, node->operator, exit_status);
	else if (node->type == EXEC_SIMPLE_COMMAND)
		status = exec_simple_command(node->simple_command, exit_status);
	else
		status = NIL;
	return (status);
}

int		execute(int fds[3], t_program *program)
{
	int		i;
	int		status;
	int		process_exit_status;

	i = -1;
	status = SUCCESS;
	while (OK(status) && program->commands[++i])
		status = execute_switch(fds, program->commands[i], &process_exit_status);
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
