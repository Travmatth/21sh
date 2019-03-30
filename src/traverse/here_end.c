/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_end.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:40:36 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/29 17:02:56y tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** If given characters could form here_end, save chars into buf until
** determination can be made
*/

int		manage_read_buf(int fd, int dfa_state[4], char buf, char *read_buf)
{
	size_t	b_len;

	ft_putchar(buf);
	dfa_state[CHARS] += buf == '\n' ? -dfa_state[CHARS] : 1;
	if (dfa_state[STATE] == dfa_state[ACCEPT])
		ft_bzero(read_buf, LEN(read_buf, 0));
	if (dfa_state[LAST_STATE] && !dfa_state[STATE])
	{
		ft_strncat(read_buf, &buf, 1);
		b_len = LEN(read_buf, 0);
		if (ERR(write(fd, read_buf, b_len)))
			return (ERROR);
		ft_bzero(read_buf, b_len);
	}
	else if (dfa_state[STATE])
		ft_strncat(read_buf, &buf, 1);
	else if (ERR(write(fd, &buf, 1)))
		return (ERROR);
	if (buf == '\n')
		ft_putstr("heredoc > ");
	return (SUCCESS);
}

/*
** read from STDIN until here_end is encountered
** write into pipe
*/

int		manage_backspace(int fd, int dfa_state[4], char *read_buf)
{
	int		i;

	if (dfa_state[CHARS])
	{
		dfa_state[CHARS] -= 1;
		if (read_buf[0])
		{
			i = 0;
			while (read_buf[i])
				i += 1;
			read_buf[--i] = '\0';
			dfa_state[STATE] = dfa_state[LAST_STATE];
		}
		else
			write(fd, "\x7f", 1);
	}
	return (SUCCESS);
}

int		read_until_here_end(char *here_end, int **dfa, int dfa_state[4], int fd)
{
	char			buf;
	char			*read_buf;
	struct termios	tty[2];
	ssize_t			bytes;

	if (!(read_buf = ft_strnew(LEN(here_end, 0))) || ERR(prep_here_end(tty)))
		return (ERROR);
	while (TRUE)
	{
		if (NONE((bytes = read(STDIN, &buf, 1))))
			break ;
		dfa_state[STATE] = dfa[dfa_state[STATE]][(int)buf];
		if (buf == DEL)
		{
			if (ERR(manage_backspace(fd, dfa_state, read_buf)))
				return (ERROR);
		}
		else if (IS_DONE(dfa_state, buf) || IS_SIG(dfa_state))
			break ;
		else if (ERR(manage_read_buf(fd, dfa_state, buf, read_buf)))
			return (ERROR);
		dfa_state[LAST_STATE] = dfa_state[STATE];
	}
	return (restore_here_end(&tty[1]));
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

	if (ft_strcmp("WORD", ((t_ast_node*)root->val[1])->rhs))
		return (ERROR);
	here_end = (char*)((t_ast_node*)((t_ast_node*)
		root->val[1])->val[0])->val[0];
	if (ERR(create_here_end_dfa(here_end, &dfa, dfa_state)))
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
	ft_putstr("heredoc > ");
	if (ERR(read_until_here_end(here_end, dfa, dfa_state, pipe_fds[1])))
		return (ERROR);
	ft_putchar('\n');
	return (swap_here_end_fd(e_ctx, pipe_fds, dfa_state, io_num));
}
