/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 17:59:38 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/24 16:31:48 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

// int returned is exit status of forked process

int		exec_command(t_simple *simple, int should_exit)
{
	int		argc;

	if (NORMAL_CHILD_EXIT((simple->exit_status = perform_redirs(simple))))
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
	return (simple->exit_status);
}

// when `exec` called, happens in current process.
// if inside pipe would be inside child process, thus no redirections detected
// int returned is exit status of forked process

int		exec_simple_command(t_simple *simple)
{
	pid_t	pid;

	if (NORMAL_CHILD_EXIT((simple->exit_status = open_redirs(simple))))
	{
		if (simple->builtin == builtin_exec)
			simple->exit_status = exec_command(simple, FALSE);
		if (ERR((pid = fork())))
			ft_printf("fork error: %s\n", simple->command[0]);
		else if (pid == 0)
		{
			exec_command(simple, TRUE);
			ft_printf("fork error: %s\n", simple->command[0]);
			simple->exit_status = ERROR;
			_exit(1);
		}
		else if (pid > 0 && ERR(wait_loop(pid, &simple->exit_status)))
			simple->exit_status = simple->exit_status;
	}
	return (simple->exit_status);
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
	int		child_pid;

	if (ERR((pid = fork())) || ERR(pipe(pipe_fd)))
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
	{
		wait_loop(pid, &pipe_node->exit_status);
		if (NORMAL_CHILD_EXIT(pipe_node->exit_status))
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
	return (pipe_node->exit_status);
}

int		exec_logical(t_operator *operator)
{
	operator->exit_status = execute_switch(operator->left);
	if (ERROR_CHILD_EXIT(operator->exit_status) && operator->type == EXEC_OR)
		operator->exit_status = execute_switch(operator->right);
	else if (NORMAL_CHILD_EXIT(operator->exit_status) && operator->type == EXEC_AND)
		operator->exit_status = execute_switch(operator->right);
	return (operator->exit_status);
}

int		execute_switch(t_exec_node *node)
{
	int		status;

	if (node->type == EXEC_PIPE)
		status = exec_pipe(node->pipe);
	else if (node->type == EXEC_AND || node->type == EXEC_OR)
		status = exec_logical(node->operator);
	else if (node->type == EXEC_SIMPLE_COMMAND)
		status = exec_simple_command(node->simple_command);
	else
		status = 1;
	return (status);
}

int		execute(t_program *program)
{
	int		i;
	int		status;

	i = -1;
	status = SUCCESS;
	while (!ERR(status) && program->commands[++i])
		status = execute_switch(program->commands[i]);
	return (ERR(status) ? ERROR : SUCCESS);
}

/*
** Parse and execute commands
*/

int		parse_execute_input(char *complete_cmd)
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
		|| (!OK((status = execute(&program)))))
		return (status);
	return (status);
}
