/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_index.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/06 14:09:06 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/06 18:05:42 by tmatthew         ###   ########.fr       */
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
	int		cur;
	int		i;
	int		col;

	cur = ui->line_index;
	if (!line_exists(line, cur, NEXT))
		return (INVALID);
	i = current_column(line, cur);
	col = i;
	cur -= i;
	cur += next_column(line, cur);
	i = -1;
	while (++i < col && line[cur] && line[cur] != '\n')
		cur += 1;
	return (cur);
}

int		move_line_index_up(char *line, t_interface *ui)
{
	int		i;
	int		col;
	int		cur;

	cur = ui->line_index;
	if (!line_exists(line, ui->line_index, PREV))
		return (INVALID);
	i = current_column(line, cur);
	col = i;
	cur -= (i ? i + 1 : i);
	i = -1;
	cur -= 1;
	cur -= current_column(line, cur);
	while (++i < col && line[cur] && line[cur] != '\n')
		cur += 1;
	return (cur);
}

int		move_index(unsigned long c, char *line, t_interface *ui)
{
	int		i;

	i = ERROR;
	if (line)
	{
		if (c == LEFT && (!ui->line_index || line[ui->line_index - 1] == '\n'))
			i = INVALID;
		else if (c == LEFT)
			i = ui->line_index - 1;
		else if (c == RIGHT &&
			(!line[ui->line_index] || line[ui->line_index] == '\n'))
			i = INVALID;
		else if (c == RIGHT)
			i = ui->line_index + 1;
		else if (c == CTL_LEFT)
			i = move_word_index_left(line, ui);
		else if (c == CTL_RIGHT)
			i = move_word_index_right(line, ui);
		else if (c == CTL_DOWN)
			i = move_line_index_down(line, ui);
		else if (c == CTL_UP)
			i = move_line_index_up(line, ui);
	}
	return (i);
}
