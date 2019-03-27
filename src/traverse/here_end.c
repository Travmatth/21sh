/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_end.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:40:36 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/24 16:35:32 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		process_here_end(char **here_end, t_ast_node *root)
{
	*here_end = NULL;
	if (ft_strcmp("WORD", root->rhs))
		return (ERROR);
	*here_end = (char*)((t_ast_node*)root->val[0])->val[0];
	return (SUCCESS);
}

int		process_io_here(int io_num, t_ectx *e_ctx, t_ast_node *root)
{
	char			read_buf[10];
	char			*buf;
	char			*tmp_buf;
	struct termios	tty;
	struct termios	old_tty;
	char			*here_end;
	ssize_t			bytes;
	ssize_t			total;
	int				pipe_fds[2];

	if (ERR(process_here_end(&here_end, root->val[1])) || !(buf = ft_strnew(0)))
		return (ERROR);
	total = 0;
	tmp_buf = buf;
	if (!ft_strcmp("DLESS here_end", root->rhs))
	{
		if (ERR(pipe(pipe_fds)))
			return (ERROR);
		prep_terminal_here_end(&tty, &old_tty, e_ctx);
		while (TRUE)
		{
			ft_bzero(read_buf, 10);
			if (NONE((bytes = read(STDIN, read_buf, 9))) || read_buf[0] == EOT)
				break ;
			if (!(tmp_buf = ft_strjoin(buf, read_buf)))
				break ;
			total += bytes;
			free(buf);
			buf = tmp_buf;
			if (ft_strstr(buf, here_end))
				break ;
		}
		restore_terminal_here_end(&old_tty, e_ctx);
		if (!tmp_buf)
			return (ERROR);
		write(pipe_fds[1], buf, total);
		if (ERR(io_num))
		{
			if (ERR((e_ctx->orig_in = dup(e_ctx->in_fd))))
				return (ERROR);
			close(pipe_fds[1]);
			if (ERR((e_ctx->in_fd = dup2(pipe_fds[0], e_ctx->in_fd))))
				return (ERROR);
		}
		else
			dup2(pipe_fds[0], io_num);
		close(pipe_fds[0]);
	}
	else if (ft_strcmp("DLESSDASH here_end", root->rhs))
	{
		ft_printf("<<- not implemented");
		return (ERROR);
	}
	return (SUCCESS);
}
