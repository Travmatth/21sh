/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_index.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/06 14:09:06 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/09 19:30:00 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		move_word_index_left(char *line, t_interface *ui)
{
	int		i;

	i = ui->line_index;
	if (i && line[i - 1] == '\n')
		return (INVALID);
	if (i && i == ui->line_len)
		i--;
	while ((i > 0) && line[i - 1] != '\n' && IS_WHITESPACE(line[i - 1]))
		i--;
	while ((i > 0) && !IS_WHITESPACE(line[i - 1]))
		i--;
	return (i);
}

int		move_word_index_right(char *line, t_interface *ui)
{
	int		i;

	i = ui->line_index;
	if (i != ui->line_len && line[i] == '\n')
		return (INVALID);
	if (!IS_WHITESPACE(line[i]))
	{
		while (i != ui->line_len && line[i] != '\n')
		{
			if (IS_WHITESPACE(line[i]))
				break ;
			i++;
		}
	}
	while ((i != ui->line_len) && line[i] != '\n')
	{
		if (!IS_WHITESPACE(line[i]))
			break ;
		i++;
	}
	return (i);
}

int		move_line_index_down(char *line, t_interface *ui)
{
	t_uiline	*cur_line;
	int			target;
	int			col;

	cur_line = current_uiline(ui);
	if (!(cur_line->next))
		return (INVALID);
	else
		target = cur_line->next->start - 1;
	col = current_column(line, ui->line_index);
	while (++target < col && line[target] && line[target] != '\n')
		target += 1;
	return (target);
}

int		move_line_index_up(char *line, t_interface *ui)
{
	t_uiline	*cur_line;
	int			target;
	int			col;

	cur_line = current_uiline(ui);
	if (!(cur_line->prev))
		return (INVALID);
	else
		target = cur_line->prev->start - 1;
	col = current_column(line, ui->line_index);
	while (++target < col && line[target] && line[target] != '\n')
		target += 1;
	return (target);
}

int		move_index(unsigned long c, char *line, t_interface *ui)
{
	int		i;

	i = ERROR;
	if (line)
	{
		if ((c == LEFT || c == SHIFT_LEFT)
			&& (!ui->line_index || line[ui->line_index - 1] == '\n'))
			i = INVALID;
		else if (c == LEFT || c == SHIFT_LEFT)
			i = ui->line_index - 1;
		else if ((c == RIGHT || c == SHIFT_RIGHT) &&
			(!line[ui->line_index] || line[ui->line_index] == '\n'))
			i = INVALID;
		else if (c == RIGHT || c == SHIFT_RIGHT)
			i = ui->line_index + 1;
		else if (c == CTL_LEFT)
			i = move_word_index_left(line, ui);
		else if (c == CTL_RIGHT)
			i = move_word_index_right(line, ui);
		else if (c == CTL_DOWN || c == SHIFT_DOWN)
			i = move_line_index_down(line, ui);
		else if (c == CTL_UP || c == SHIFT_UP)
			i = move_line_index_up(line, ui);
	}
	return (i);
}
