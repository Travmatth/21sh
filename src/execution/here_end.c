/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_end.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/01 15:16:23 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/01 15:16:40 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** if backspace character encountered, either need to remove last
** char in read buffer and set new state or erase last char in file
*/

int		manage_del(int **dfa, int dfa_state[5], char **read_buf)
{
	int		i;

	if (!dfa_state[CHARS])
		return (SUCCESS);
	dfa_state[CHARS] -= 1;
	if (read_buf[HERE_END_BUF][0])
	{
		i = 0;
		while (read_buf[HERE_END_BUF][i])
			i += 1;
		read_buf[HERE_END_BUF][--i] = '\0';
		i = 0;
		dfa_state[STATE] = 0;
		while (read_buf[HERE_END_BUF][i])
			dfa_state[STATE] = dfa[dfa_state[STATE]][(int)read_buf[HERE_END_BUF][i++]];
		write(STDIN, "\b \b", 3);
		return (SUCCESS);
	}
	i = 0;
	while (read_buf[IN_BUF][i])
		i += 1;
	read_buf[IN_BUF][--i] = '\0';
	write(STDIN, "\b \b", 3);
	return (SUCCESS);
}

/*
** If given characters could form here_end, save chars into buf until
** determination can be made
*/

int		manage_read(int dfa_state[5], char buf, char **read_buf)
{
	size_t	b_len;
	char	*tmp;

	ft_putchar(buf);
	dfa_state[CHARS] += buf == '\n' ? -dfa_state[CHARS] : 1;
	b_len = LEN(read_buf[HERE_END_BUF], 0);
	if (dfa_state[STATE] == dfa_state[ACCEPT])
		ft_bzero(read_buf[HERE_END_BUF], b_len);
	if (dfa_state[LAST_STATE] && !dfa_state[STATE])
	{
		ft_strncat(read_buf[HERE_END_BUF], &buf, 1);
		if (!(tmp = ft_strjoin(read_buf[HERE_END_BUF], read_buf[IN_BUF])))
			return (ERROR);
		free(read_buf[HERE_END_BUF]);
		read_buf[HERE_END_BUF] = tmp;
		ft_bzero(read_buf[HERE_END_BUF], b_len + 1);
	}
	else if (dfa_state[STATE])
		ft_strncat(read_buf[HERE_END_BUF], &buf, 1);
	else
	{
		read_buf[HERE_END_BUF][0] = buf;
		if (!(tmp = ft_strjoin(read_buf[IN_BUF], read_buf[HERE_END_BUF])))
			return (ERROR);
		free(read_buf[IN_BUF]);
		read_buf[IN_BUF] = tmp;
		read_buf[HERE_END_BUF][0] = '\0';
	}
	if (buf == '\n')
		ft_putstr("heredoc > ");
	return (SUCCESS);
}

/*
** read from STDIN until here_end is encountered, write into pipe
*/

int		read_until_here_end(char *here_end, int **dfa, int dfa_state[5], int fd)
{
	char			buf;
	char			**read_buf;
	struct termios	tty[2];
	ssize_t			bytes;
	int				err;

	if (!(read_buf = init_bufs(LEN(here_end, 0))) || ERR(prep_here_end(tty)))
		return (ERROR);
	err = FALSE;
	while (!err)
	{
		if (NONE((bytes = read(STDIN, &buf, 1))))
			break ;
		dfa_state[STATE] = dfa[dfa_state[STATE]][(int)buf];
		if (buf == DEL && ERR(manage_del(dfa, dfa_state, read_buf)))
			err = TRUE;
		else if (buf != DEL && (IS_DONE(dfa_state, buf) || IS_SIG(dfa_state)))
		{
			if (ERR(write(fd, read_buf[IN_BUF], LEN(read_buf[IN_BUF], 0))))
				err = TRUE;
			break ;
		}
		else if (buf != DEL && ERR(manage_read(dfa_state, buf, read_buf)))
			err = TRUE;
		dfa_state[LAST_STATE] = dfa_state[STATE];
	}
	if (err && restore_here_end(&tty[1]))
		return (ERROR);
	return (restore_here_end(&tty[1]));
}

/*
** Once stdin has been read until here_end or signal encountered
** set pipe in as stdin or specified fd
*/

int		swap_here_end_fd(int pipe_fds[2], int dfa_state[5], int io_num)
{
	// if (dfa_state[STATE] != dfa_state[SIG] && ERR(io_num))
	// {
	// 	if (ERR((t_ctx->orig_in = dup(t_ctx->in_fd))))
	// 		return (ERROR);
	// 	close(pipe_fds[1]);
	// 	if (ERR((t_ctx->in_fd = dup2(pipe_fds[0], t_ctx->in_fd))))
	// 		return (ERROR);
	// }
	// else if (dfa_state[STATE] != dfa_state[SIG])
	// 	dup2(pipe_fds[0], io_num);
	// close(pipe_fds[0]);
	(void)pipe_fds;
	(void)dfa_state;
	(void)io_num;
	return (SUCCESS);
}

/*
** read from stdin until EOF follow by newline encountered
** or a signal is encountered. If successful, set new fd as
** STDIN or given fd
*/
