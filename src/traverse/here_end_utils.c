/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_end_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:40:36 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/27 17:28:06 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		prep_terminal_here_end(struct termios *tty
	, struct termios *old_tty
	, t_ectx *e_ctx)
{
	char			*type;
	char			buf[BUFF_SIZE];

	if (!isatty(e_ctx->in_fd))
		return (ERROR);
	else if (NONE((type = get_env_var("TERM"))))
		return (ERROR);
	else if (ERR(tgetent(buf, type)))
		return (ERROR);
	else if (ERR(tcgetattr(e_ctx->in_fd, tty)))
		return (ERROR);
	else if (ERR(tcgetattr(e_ctx->in_fd, old_tty)))
		return (ERROR);
	tty->c_lflag &= ~(ICANON | ISIG | ECHO);
	tty->c_cc[VMIN] = 1;
	tty->c_cc[VTIME] = 0;
	if (ERR(tcsetattr(e_ctx->in_fd, TCSANOW, tty)))
		return (ERROR);
	return (SUCCESS);
}

int		restore_terminal_here_end(struct termios *tty, t_ectx *e_ctx)
{
	if (ERR(tcsetattr(e_ctx->in_fd, TCSADRAIN, tty)))
		return (ERROR);
	return (SUCCESS);
}
