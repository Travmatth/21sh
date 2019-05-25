/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_end_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:40:36 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/25 16:01:37 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		init_heredoc(t_redir *redir
					, t_heredoc *heredoc
					, struct termios tty[2]
					, int *found)
{
	int		status;

	heredoc->buf_len = 0;
	heredoc->line_len = 0;
	heredoc->here_end_len = LEN(redir->word, 0);
	if ((heredoc->buf = ft_strnew(heredoc->here_end_len)))
	{
		*found = FALSE;
		heredoc->tmp = NULL;
		heredoc->line = NULL;
		status = ERR(prep_terminal(tty, ~(ICANON | ISIG | ECHO)))
			|| ERR(pipe(heredoc->fd)) ? ERROR : SUCCESS;
		redir->replacement = ERR(status) ? ERROR : heredoc->fd[0];
		ft_putstr("heredoc > ");
	}
	else
		status = ERROR;
	return (status);
}

int		handle_full_buffer(t_heredoc *heredoc)
{
	if (!(heredoc->tmp = ft_strjoin(heredoc->line, heredoc->buf)))
		return (ERROR);
	free(heredoc->line);
	heredoc->line = heredoc->tmp;
	heredoc->tmp = NULL;
	heredoc->line_len += heredoc->buf_len;
	heredoc->buf_len = 0;
	ft_bzero(heredoc->buf, heredoc->here_end_len);
	return (SUCCESS);
}

int		add_newline_char(t_redir *redir, t_heredoc *heredoc)
{
	int		status;

	status = SUCCESS;
	ft_putstr("\nheredoc > ");
	if (redir->heredoc_quoted)
		heredoc->tmp = heredoc->line;
	else
		status = heredoc_line_expansion(&heredoc->tmp, heredoc->line);
	if (OK(status)
		&& (ERR(write(heredoc->fd[1], heredoc->tmp, LEN(heredoc->tmp, 0)))
			|| ERR(write(heredoc->fd[1], "\n", 1))))
		status = ERROR;
	free(heredoc->tmp);
	heredoc->tmp = NULL;
	heredoc->line = NULL;
	heredoc->line_len = 0;
	ft_bzero(heredoc->buf, heredoc->buf_len);
	heredoc->buf_len = 0;
	return (status);
}

int		remove_last_char(t_heredoc *heredoc)
{
	int		status;

	status = SUCCESS;
	if (heredoc->buf_len)
	{
		heredoc->buf[--heredoc->buf_len] = '\0';
		status = ERR(write(STDIN, "\b \b", 3)) ? ERROR : status;
	}
	else if (heredoc->line_len)
	{
		heredoc->line[--heredoc->line_len] = '\0';
		status = ERR(write(STDIN, "\b \b", 3)) ? ERROR : status;
	}
	return (status);
}

void	add_next_char(t_heredoc *heredoc, char *next)
{
	heredoc->buf_len += 1;
	ft_putchar(*next);
	ft_strncat(heredoc->buf, next, 1);
}
