/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/23 12:05:13 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/23 17:38:13 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

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
	int 	id;
	int 	sig;

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
		ft_printf(WAITPID_ERR, pid, g_sig_table[sig]);
		*return_val = ERROR;
	}
	else if (WIFEXITED(*return_val))
		*return_val = WEXITSTATUS(*return_val);
	return (*return_val);
}
