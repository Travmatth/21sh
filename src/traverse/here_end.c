/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_end.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:40:36 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/21 18:06:43 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

// need to save original vtime, vmin values in the tty struct after calling tcsetattr

int		prep_terminal_here_end(struct termios *tty)
{
	char			*type;
	char			buf[BUFF_SIZE];
	int				vmin;
	int				vtime;

	if (!isatty(STDIN))
		return (ERROR);
	else if (NONE((type = getenv("TERM"))))
		return (ERROR);
	else if (ERR(tgetent(buf, type)))
		return (ERROR);
	else if (ERR(tcgetattr(STDIN, tty)))
		return (ERROR);
	tty->c_lflag &= ~ICANON;
	vmin = tty->c_cc[VMIN];
	vtime = tty->c_cc[VTIME];
	tty->c_cc[VMIN] = 1;
	tty->c_cc[VTIME] = 0;
	if (ERR(tcsetattr(STDIN, TCSANOW, tty)))
		return (ERROR);
	tty->c_cc[VMIN] = vmin;
	tty->c_cc[VTIME] = vtime;
	return (SUCCESS);
}

// use tty struct to restore original vtime, vmin

int		restore_terminal_here_end(struct termios *tty)
{
	tty->c_lflag &= ICANON;
	if (ERR(tcsetattr(STDIN, TCSADRAIN, tty)))
		return (ERROR);
	return (SUCCESS);
}
