/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_end_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:40:36 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/25 17:04:32 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		prep_here_end(struct termios *ttys)
{
	if (!isatty(STDIN)
		|| ERR(tcgetattr(STDIN, &ttys[0]))
		|| ERR(tcgetattr(STDIN, &ttys[1])))
		return (ERROR);
	ttys[0].c_lflag &= ~(ICANON | ISIG | ECHO);
	ttys[0].c_cc[VMIN] = 1;
	ttys[0].c_cc[VTIME] = 0;
	if (ERR(tcsetattr(STDIN, TCSANOW, &ttys[0])))
		return (ERROR);
	return (NORMAL_CHILD_EXIT);
}

int		restore_here_end(int pipe_in, struct termios *tty)
{
	if (ERR(close(pipe_in))
		|| ERR(tcsetattr(STDIN, TCSADRAIN, tty)))
		return (ERROR);
	return (NORMAL_CHILD_EXIT);
}
