/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 12:05:13 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/13 18:56:55 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Convert error signal int to appropriate type
*/

char	*g_sig_table[32] =
{
	"normal termination",
	"SIGHUP",
	"SIGINT",
	"SIGQUIT",
	"SIGILL",
	"SIGTRAP",
	"SIGABRT",
	"SIGEMT",
	"SIGFPE",
	"SIGKILL",
	"SIGBUS",
	"SIGSEGV",
	"SIGSYS",
	"SIGPIPE",
	"SIGALRM",
	"SIGTERM",
	"SIGURG",
	"SIGSTOP",
	"SIGTSTP",
	"SIGCONT",
	"SIGCHLD",
	"SIGTTIN",
	"SIGTTOU",
	"SIGIO",
	"SIGXCPU",
	"SIGXFSZ",
	"SIGVTALRM",
	"SIGPROF",
	"SIGWINCH",
	"SIGINFO",
	"SIGUSR1",
	"SIGUSR2"
};

/*
** When debugging a program that fork()'s a child process and uses waitpid()
** to block the calling thread, the debugger will immediately unblock the
** parent process by sending an EINTR signal from the child process. To
** block until the process returns, we must detect these EINTR signals and
** call waitpid() again, until child process resolves.
** https://stackoverflow.com/questions/21554195
*/

int		wait_loop(int pid, int *return_val)
{
	int		id;
	int		sig;

	errno = 0;
	while ((id = waitpid(pid, return_val, WNOHANG)) != pid)
	{
		if (ERR(id))
		{
			*return_val = ERROR;
			break ;
		}
		if (errno == EINTR)
			errno = 0;
	}
	if (WIFSIGNALED(*return_val))
	{
		sig = WTERMSIG(*return_val);
		*return_val = sig == SIGINT ? NORMAL_CHILD_EXIT : ERROR;
		if (*return_val == ERROR)
			ft_dprintf(STDERR, WAITPID_ERR, pid, g_sig_table[sig]);
	}
	else if (WIFEXITED(*return_val))
		*return_val = WEXITSTATUS(*return_val);
	return (*return_val);
}

/*
** Free redir structs
*/

void	free_redirs(t_redir *redir)
{
	if (redir)
	{
		free_redirs(redir->next);
		free(redir->word);
		free(redir);
	}
}

/*
** Free nodes by recursing down logical and pipe nodes to find base
** simple command nodes, freeing simple commands and their parents after.
*/

void	free_exec_node(t_exec_node *container)
{
	if (container->type == EXEC_PIPE)
	{
		free_exec_node(container->node.pipe->left);
		free_exec_node(container->node.pipe->right);
		free(container->node.pipe);
	}
	else if (container->type == EXEC_AND || container->type == EXEC_OR)
	{
		free_exec_node(container->node.operator->left);
		free_exec_node(container->node.operator->right);
		free(container->node.operator);
	}
	else if (container->type == EXEC_SIMPLE_COMMAND)
	{
		free_redirs(container->node.simple->redirs);
		ft_freearr(container->node.simple->command, TRUE);
		free(container->node.simple);
	}
	free(container);
}

/*
** Free program tree by freeing individual nodes
*/

void	free_program(t_program *program)
{
	int		i;

	if (!program || !program->commands)
		return ;
	i = -1;
	while (program->commands[++i])
		free_exec_node(program->commands[i]);
	free(program->commands);
}
