/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_end.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:40:36 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/28 17:44:18 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** extract here_end word from AST
*/

int		process_here_end(char **here_end, t_ast_node *root)
{
	*here_end = NULL;
	if (ft_strcmp("WORD", root->rhs))
		return (ERROR);
	*here_end = (char*)((t_ast_node*)root->val[0])->val[0];
	return (SUCCESS);
}

/*
** If given characters could form here_end, save chars into buf until
** determination can be made
*/

int		manage_read_buf(int fd, int dfa_state[4], char buf, char *read_buf)
{
	if (dfa_state[LAST_STATE] && !dfa_state[STATE])
	{
		ft_strncat(read_buf, &buf, 1);
		if (ERR(write(fd, read_buf, LEN(read_buf, 0))))
			return (ERROR);
		ft_bzero(read_buf, LEN(read_buf, 0));
	}
	// need to write intermediate state to stdout still
	else if (dfa_state[LAST_STATE] && dfa_state[STATE])
		ft_strncat(read_buf, &buf, 1);
	else
	{
		if (ERR(write(fd, read_buf, 1)))
			return (ERROR);
		ft_putchar(buf);
		if (buf == '\n')
			ft_putstr("> ");
	}
	return (SUCCESS);
}

/*
** read from STDIN until here_end is encountered
** write into pipe
*/

int		read_until_here_end(char *here_end, int **dfa, int dfa_state[4], int fd)
{
	char			buf;
	char			*read_buf;
	struct termios	tty[2];
	ssize_t			bytes;

	if (!(read_buf = ft_strnew(LEN(here_end, 0)))
			|| ERR(prep_terminal_here_end(&tty[0], &tty[1])))
		return (ERROR);
	ft_putstr("> ");
	while (TRUE)
	{
		if (NONE((bytes = read(STDIN, &buf, 1))))
			break ;
		dfa_state[STATE] = dfa[dfa_state[STATE]][(int)buf];
		if (dfa_state[STATE] == dfa_state[ACCEPT] && buf == '\n')
			break ;
		else if (dfa_state[STATE] == dfa_state[SIG])
			break ;
		else if (dfa_state[STATE] == dfa_state[ACCEPT])
			continue ;
		else if (ERR(manage_read_buf(fd, dfa_state, buf, read_buf)))
			return (ERROR);
		dfa_state[LAST_STATE] = dfa_state[STATE];
	}
	return (restore_terminal_here_end(&tty[1]));
}

/*
** Once stdin has been read until here_end or signal encountered
** set pipe in as stdin or specified fd
*/

int		swap_here_end_fd(t_ectx *e_ctx
						, int pipe_fds[2]
						, int dfa_state[4]
						, int io_num)
{
	if (dfa_state[STATE] != dfa_state[SIG] && ERR(io_num))
	{
		if (ERR((e_ctx->orig_in = dup(e_ctx->in_fd))))
			return (ERROR);
		close(pipe_fds[1]);
		if (ERR((e_ctx->in_fd = dup2(pipe_fds[0], e_ctx->in_fd))))
			return (ERROR);
	}
	else if (dfa_state[STATE] != dfa_state[SIG])
		dup2(pipe_fds[0], io_num);
	close(pipe_fds[0]);
	return (SUCCESS);
}

/*
** read from stdin until EOF follow by newline encountered
** or a signal is encountered. If successful, set new fd as
** STDIN or given fd
*/

int		process_io_here(int io_num, t_ectx *e_ctx, t_ast_node *root)
{
	char			*here_end;
	int				pipe_fds[2];
	int				**dfa;
	int				dfa_state[4];

	if (ERR(process_here_end(&here_end, root->val[1]))
		|| ERR(create_here_end_dfa(here_end, &dfa, dfa_state)))
		return (ERROR);
	if (!ft_strcmp("DLESSDASH here_end", root->rhs))
	{
		ft_printf("<<- not implemented");
		return (ERROR);
	}
	if (ft_strcmp("DLESS here_end", root->rhs))
		return (ERROR);
	if (ERR(pipe(pipe_fds)))
		return (ERROR);
	if (ERR(read_until_here_end(here_end, dfa, dfa_state, pipe_fds[1])))
		return (ERROR);
	return (swap_here_end_fd(e_ctx, pipe_fds, dfa_state, io_num));
}
