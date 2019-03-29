/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_end_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:40:36 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/28 17:13:49 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		create_here_end_dfa(char *here_end, int ***dfa, int dfa_state[3])
{
	int		i;
	int		j;
	int		len;

	len = (int)LEN(here_end, 0);
	if (!(*dfa = ft_memalloc(sizeof(int*) * (len + 2))))
		return (ERROR);
	i = 0;
	while (i < len + 2)
	{
		if (!((*dfa)[i++] = ft_memalloc(sizeof(int) * 256)))
			return (ERROR);
	}
	i = len;
	while (--i >= 0)
	{
		j = 256;
		while (--j >= 0)
		{
			if (j == EOT || j == INTR)
				(*dfa)[i][j] = len + 1;
			else if (j == here_end[i])
				(*dfa)[i][j] = i + 1;
			else
				(*dfa)[i][j] = 0;
		}
	}
	while (++j < 256)
	{
		if (j == EOT || j == INTR)
			(*dfa)[len][j] = len + 1;
		else
			(*dfa)[len][j] = len;
		(*dfa)[len + 1][j] = len + 1;
	}
	dfa_state[ACCEPT] = (int)len;
	dfa_state[SIG] = (int)len + 1;
	return (SUCCESS);
}

int		prep_terminal_here_end(struct termios *tty
								, struct termios *old_tty)
{
	char			*type;
	char			buf[BUFF_SIZE];

	if (!isatty(STDIN)
		|| NONE((type = get_env_var("TERM")))
		|| ERR(tgetent(buf, type))
		|| ERR(tcgetattr(STDIN, tty))
		|| ERR(tcgetattr(STDIN, old_tty)))
		return (ERROR);
	tty->c_lflag &= ~(ICANON | ISIG | ECHO);
	tty->c_cc[VMIN] = 1;
	tty->c_cc[VTIME] = 0;
	if (ERR(tcsetattr(STDIN, TCSANOW, tty)))
		return (ERROR);
	return (SUCCESS);
}

int		restore_terminal_here_end(struct termios *tty)
{
	if (ERR(tcsetattr(STDIN, TCSADRAIN, tty)))
		return (ERROR);
	return (SUCCESS);
}
