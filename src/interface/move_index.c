/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_index.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/06 14:09:06 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/10 19:24:05 by tmatthew         ###   ########.fr       */
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
	if (i != ui->line_len && line[i] == '\\' && line[i + 1] == '\n')
		return (INVALID);
	if (!IS_WHITESPACE(line[i]))
	{
		while (i != ui->line_len && line[i] != '\\' && line[i + 1] != '\n')
		{
			if (IS_WHITESPACE(line[i]))
				break ;
			i++;
		}
	}
	while ((i != ui->line_len) && line[i] != '\\' && line[i + 1] != '\n')
	{
		if (!IS_WHITESPACE(line[i]))
			break ;
		i++;
	}
	return (i);
}

int		move_line_index_down(char *line, t_interface *ui)
{
	t_uiline	*ui_line;
	int			cur_i;
	int			cur_col;
	int			target_col;

	if (!(ui_line = current_uiline(ui, ui->line_index)) || !(ui_line->next))
		return (INVALID);
	cur_i = ui_line->next->start;
	cur_col = current_column(line, cur_i);
	target_col = current_column(line, ui->line_index);
	while (cur_col < target_col && line[cur_i] && (line[cur_i] != '\\' &&
													line[cur_i + 1 ] != '\n'))
	{
		cur_i += 1;
		cur_col = current_column(line, cur_i);
	}
	return (cur_i);
}

int		move_line_index_up(char *line, t_interface *ui)
{
	t_uiline	*ui_line;
	int			cur_i;
	int			cur_col;
	int			target_col;

	if (!(ui_line = current_uiline(ui, ui->line_index)) || !(ui_line->prev))
		return (INVALID);
	cur_i = ui_line->prev->start;
	cur_col = current_column(line, cur_i);
	target_col = current_column(line, ui->line_index);
	while (cur_col < target_col && line[cur_i] && (line[cur_i] != '\\' &&
													line[cur_i + 1 ] != '\n'))
	{
		cur_i += 1;
		cur_col = current_column(line, cur_i);
	}
	return (cur_i);
}

/*
** Determine new index that would result by moving cursor, return INVALID (-2)
** if movement is invalid, or ERROR (-1) if movement parameter c not valid.
** Otherwise returns new index that would be the result of intended movement
*/

int		move_index(unsigned long c, char *line, t_interface *ui)
{
	int		i;
	int		n;

	i = ERROR;
	n = ui->line_index;
	if (line)
	{
		if ((c == LEFT || c == SHIFT_LEFT) && (!n || (line[n - 1] == '\n')))
			i = INVALID;
		else if (c == LEFT || c == SHIFT_LEFT)
			i = n - 1;
		else if (c == RIGHT || c == SHIFT_RIGHT)
			i = (n == ui->line_len || (n && line[n] == '\\'
				&& line[n + 1] == '\n')) ? INVALID : n + 1;
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
