/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:42:31 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/06 17:08:44 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Read single character from STDIN. Break reading on CTRL-C and delete input
** on BACKSPACE, otherwise append character to line and echo to STDOUT
*/

int		interface(char **line)
{
	char			*tmp;
	char			next[2];
	struct termios	tty[2];
	int				status;
	size_t			len;

	len = 0;
	next[1] = '\0';
	if ((!*line && !(*line = ft_strnew(0))))
		return (ERROR);
	status = prep_terminal(tty, ~(ICANON | ISIG | ECHO));
	while (OK(status) && !ERR((status = read(STDIN, &next, 1))))
	{
		// if char is newline or CTRL-C, end current line
		if (next[0] == RETURN || next[0] == INTR)
		{
			status = write(STDOUT, "\n", 1);
			break ;
		}
		// if char is delete, remove last char from line and STDOUT
		else if (next[0] == DEL && len)
		{
			(*line)[--len] = '\0';
			status = ERR(write(STDIN, "\b \b", 3)) ? ERROR : status;
		}
		// else add char to line and STDOUT
		else if ((tmp = ft_strjoin(*line, next)))
		{
			free(*line);
			*line = tmp;
			len += 1;
			status = ERR(write(STDOUT, next, 1)) ? ERROR : status;
		}
		else
			status = ERROR;
	}
	return (ERR(restore_terminal(&tty[1])) ? ERROR : status);
}
