/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_modes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 11:09:30 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/12 18:53:36 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		prep_terminal(struct termios *ttys, int flags, int vmin, int vtime)
{
	ft_dprintf(STDERR, "Turn off isig");
	if (!isatty(STDIN)
		|| ERR(tcgetattr(STDIN, &ttys[0]))
		|| ERR(tcgetattr(STDIN, &ttys[1])))
		return (ERROR);
	ttys[0].c_lflag &= flags;
	ttys[0].c_cc[VMIN] = vmin;
	ttys[0].c_cc[VTIME] = vtime;
	if (ERR(tcsetattr(STDIN, TCSANOW, &ttys[0])))
		return (ERROR);
	return (SUCCESS);
}

int		restore_terminal(struct termios *tty)
{
	ft_dprintf(STDERR, "Turn on isig");
	if (ERR(tcsetattr(STDIN, TCSADRAIN, tty)))
		return (ERROR);
	return (SUCCESS);
}

int		ft_termprint(int c)
{
	return (write(1, &c, 1));
}
