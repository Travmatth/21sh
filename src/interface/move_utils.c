/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 14:17:03 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/08 19:42:42 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int			next_column(char *line, int index)
{
	int		col;

	col = 1;
	while (line[index])
	{
		if (line[index] == '\n')
			break ;
		col += 1;
		index += 1;
	}
	return (col);
}

int			current_column(char *line, int index)
{
	int		col;

	col = 0;
	while (index > 0)
	{
		if (index > 1 && line[index - 1] == '\n')
			break ;
		col += 1;
		index -= 1;
	}
	return (col);
}

int			line_exists(char *line, int index, int direction)
{
	int		step;

	step = direction == PREV ? -1 : 1;
	while ((direction == NEXT && line[index]) || (direction == PREV && index))
	{
		if (line[index] == '\n')
		{
			return (TRUE);
		}
		index += step;
	}
	return (FALSE);
}
