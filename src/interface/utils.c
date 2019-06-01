/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 12:50:52 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/31 17:44:57 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

t_uisegment	*new_segment(void)
{
	t_uisegment	*segment;

	if (!(segment = ft_memalloc(sizeof(t_uisegment))))
		return (NULL);
	segment->x = ERROR;
	segment->y = ERROR;
	segment->start = ERROR;
	segment->end = ERROR;
	segment->word_end = ERROR;
	return (segment);
}

void	add_segment(t_uisegment **segments, t_uisegment *seg)
{
	t_uisegment	*current;
	if (!*segments)
		*segments = seg;
	else
	{
		current = *segments;
		while (current->next)
			current = current->next;
		seg->prev = current;
		current->next = seg;
	}
}

/*
** To obtain current x,y position of cursor we can query the tty using Control
** Sequence Introducer with Parameter #6, will write position to STDOUT
** Ps = 6  -> Report Cursor Position (CPR) [row;column].
** \e[y;xR\0
** https://invisible-island.net/xterm/ctlseqs/ctlseqs.html#h2-Control-Bytes_-\
** Characters_-and-Sequences
*/

int		get_cursor_position(int *x, int *y)
{
	int		status;
	char	buf[64];
	char	*col;
	char	*row;

	ft_bzero(buf, 64);
	if (OK((status = ERR(write(STDOUT, CURSOR_POS_REQ, 4)) ? ERROR : SUCCESS)))
		status = ERR(read(STDIN, buf, 64)) ? ERROR : status;
	if (!(row = ft_strchr(buf, '[')))
		return (ERROR);
	else if (!(col = ft_strchr(buf, ';')))
		return (ERROR);
	status = OK(status) && OK(ft_safeatoi(++row, x)) ? status : ERROR;
	status = OK(status) && OK(ft_safeatoi(++col, y)) ? status : ERROR;
	return (status);
}
