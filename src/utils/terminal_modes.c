/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_modes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 11:09:30 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/13 14:16:54 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		prep_terminal(struct termios *ttys, int flags, int vmin, int vtime)
{
	if (!isatty(STDIN)
		|| ERR(tcgetattr(STDIN, &ttys[0]))
		|| ERR(tcgetattr(STDIN, &ttys[1])))
		return (ERROR);
	ttys[0].c_lflag &= flags;
	if (vmin != ERROR)
		ttys[0].c_cc[VMIN] = vmin;
	if (vtime != ERROR)
		ttys[0].c_cc[VTIME] = vtime;
	if (ERR(tcsetattr(STDIN, TCSANOW, &ttys[0])))
		return (ERROR);
	return (SUCCESS);
}

int		restore_terminal(struct termios *tty)
{
	if (ERR(tcsetattr(STDIN, TCSADRAIN, tty)))
		return (ERROR);
	return (SUCCESS);
}

int		ft_termprint(int c)
{
	return (write(1, &c, 1));
}
