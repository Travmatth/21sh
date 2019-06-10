/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 14:17:03 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/09 21:48:16 by tmatthew         ###   ########.fr       */
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

int			line_exists(t_interface *ui, int direction)
{
	t_uiline	*ui_line;
	int			is;

	ui_line = current_uiline(ui);
	if (!ui_line)
		is = FALSE;
	else if (direction == PREV)
		is = ui_line->prev ? TRUE : FALSE;
	else
		is = ui_line->next ? TRUE : FALSE;
	return (is);
}

t_uiline	*current_uiline(t_interface *ui)
{
	t_uiline	*cur;

	if (!(cur = ui->ui_line))
		return (NULL);
	while (cur->next && cur->end != ERROR && ui->line_index > cur->end)
		cur = cur->next;
	if (cur->end == ERROR && cur->start <= ui->line_index)
		return (cur);
	else if (cur->start <= ui->line_index && cur->end >= ui->line_index)
		return (cur);
	return (NULL);
}
