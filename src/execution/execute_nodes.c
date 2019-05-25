/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_nodes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/19 14:57:40 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/19 14:57:56 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Given a simple command inside of a child process, execute builtin commands
** by invoking related function pointer, or call programs with execve. Exit
** child process if needed, int returned is exit status of forked process
*/

int		exec_command(t_simple *simple, int should_exit)
{
	if (IS_NORMAL_CHILD_EXIT((simple->exit_status = perform_redirs(simple))))
	{
		if (simple->builtin)
		{
			simple->exit_status = simple->builtin(simple->argc
												, simple->command);
			simple->exit_status = OK(simple->exit_status) ? 0 : 1;
			if (should_exit)
				_exit(simple->exit_status);
		}
		else
		{
			execve(simple->command[0], simple->command, g_environ);
			simple->exit_status = ERROR_CHILD_EXIT;
			if (should_exit)
				_exit(1);
		}
	}
	return (simple->exit_status);
}

/*
** Manage execution of simple commands by forking when appropriate,
** calls exec_command inside child process to perform execution of command.
** Int returned is exit status of forked process.
*/

int		exec_simple_command(t_simple *simple)
{
	pid_t	pid;

	if (IS_NORMAL_CHILD_EXIT((simple->exit_status = open_redirs(simple))))
	{
		if (simple->is_exec)
			simple->exit_status = exec_command(simple, FALSE);
		else if (ERR((pid = fork())))
			simple->exit_status = ERROR_CHILD_EXIT;
		else if (pid == 0)
			exec_command(simple, TRUE);
		else if (pid > 0)
			wait_loop(pid, &simple->exit_status);
	}
	return (simple->exit_status);
}

/*
** Execute pipe operations by executing left command, then right if successful.
** Before the first command is executed, pipe forks itself and redirects STDOUT
** to pipe in. Before second command, input is redirected to pipe out. Children
** execution managed by execute_switch
*/

void	exec_pipe_parent_process(t_pipe *pipe_node, int pid, int pipe_fd[2])
{
	int		child_pid;

	wait_loop(pid, &pipe_node->exit_status);
	if (IS_NORMAL_CHILD_EXIT(pipe_node->exit_status))
	{
		if (ERR((child_pid = fork())))
			pipe_node->exit_status = ERROR;
		else if (!ERR(pipe_node->exit_status) && child_pid == 0)
		{
			if (!ERR((pipe_node->exit_status = close(pipe_fd[PIPE_WRITE]))))
				pipe_node->exit_status = dup2(pipe_fd[PIPE_READ], STDIN);
			if (!ERR(pipe_node->exit_status))
				pipe_node->exit_status = execute_switch(pipe_node->right);
			_exit(pipe_node->exit_status);
		}
		else if (!ERR(pipe_node->exit_status) && child_pid > 0)
		{
			close(pipe_fd[PIPE_WRITE]);
			close(pipe_fd[PIPE_WRITE]);
			wait_loop(child_pid, &pipe_node->exit_status);
		}
	}
}

/*
** Execute pipe operations by executing left command, then right if successful.
** Before the first command is executed, pipe forks itself and redirects STDOUT
** to pipe in. Before second command, input is redirected to pipe out. Children
** execution managed by execute_switch
*/

int		exec_pipe(t_pipe *pipe_node)
{
	int		pipe_fd[2];
	int		pid;

	if (ERR(pipe(pipe_fd)) || ERR((pid = fork())))
		return (ERROR);
	else if (pid == 0)
	{
		if (!ERR((pipe_node->exit_status = close(pipe_fd[PIPE_READ]))))
			pipe_node->exit_status = dup2(pipe_fd[PIPE_WRITE], STDOUT);
		if (!ERR(pipe_node->exit_status))
			pipe_node->exit_status = execute_switch(pipe_node->left);
		_exit(pipe_node->exit_status);
	}
	else if (!ERR(pipe_node->exit_status) && pid > 0)
		exec_pipe_parent_process(pipe_node, pid, pipe_fd);
	return (pipe_node->exit_status);
}

/*
** Execute logical operations by executing left hand command, then right hand
** command if operation is OR and first command failed, or if operation is AND
** and first command was succesful
*/

int		exec_logical(t_operator *operator)
{
	operator->exit_status = execute_switch(operator->left);
	if (IS_ERROR_CHILD_EXIT(operator->exit_status)
		&& operator->type == EXEC_OR)
		operator->exit_status = execute_switch(operator->right);
	else if (IS_NORMAL_CHILD_EXIT(operator->exit_status)
		&& operator->type == EXEC_AND)
		operator->exit_status = execute_switch(operator->right);
	return (operator->exit_status);
}
