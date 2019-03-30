/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_end_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:40:36 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/29 17:48:56 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		create_here_end_dfa(char *here_end, int ***dfa, int dfa_state[5])
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
	dfa_state[CHARS] = 0;
	dfa_state[STATE] = 0;
	dfa_state[LAST_STATE] = 0;
	dfa_state[ACCEPT] = (int)len;
	dfa_state[SIG] = (int)len + 1;
	return (SUCCESS);
}

int		prep_here_end(struct termios *ttys)
{
	char			*type;
	char			buf[BUFF_SIZE];

	if (!isatty(STDIN)
		|| NONE((type = get_env_var("TERM")))
		|| ERR(tgetent(buf, type))
		|| ERR(tcgetattr(STDIN, &ttys[0]))
		|| ERR(tcgetattr(STDIN, &ttys[1])))
		return (ERROR);
	ttys[0].c_lflag &= ~(ICANON | ISIG | ECHO);
	ttys[0].c_cc[VMIN] = 1;
	ttys[0].c_cc[VTIME] = 0;
	if (ERR(tcsetattr(STDIN, TCSANOW, &ttys[0])))
		return (ERROR);
	return (SUCCESS);
}

int		restore_here_end(struct termios *tty)
{
	if (ERR(tcsetattr(STDIN, TCSADRAIN, tty)))
		return (ERROR);
	return (SUCCESS);
}
