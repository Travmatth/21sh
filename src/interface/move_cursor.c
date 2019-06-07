/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_cursor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/06 14:14:11 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/07 00:11:10 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	move_word_cursor_left(t_interface *ui, int target)
{
	int		diff;

	diff = ui->line_index - target;
	while (diff--)
		tputs(tgetstr("le", NULL), 1, ft_termprint);
}

void	move_word_cursor_right(t_interface *ui, int target)
{
	int		diff;

	diff = target - ui->line_index;
	while (diff--)
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
}

void	move_line_cursor_up(char *line, t_interface *ui, int target)
{
	int		i;
	int		col;
	int		offset;

	i = ui->line_index;
	col = current_column(line, i);
	i -= col + 1;
	i = i < 0 ? 0 : i;
	if (i)
		i -= current_column(line, i);
	offset = line_exists(line, i, PREV) ? 2 : 3;
	tputs(tgetstr("cr", NULL), 1, ft_termprint);
	tputs(tgetstr("up", NULL), 1, ft_termprint);
	while (offset--)
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
	while (i < target)
	{
		i += 1;
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
	}
}

void	move_line_cursor_down(char *line, t_interface *ui, int target)
{
	int		offset;
	int		col;
	int		i;

	i = ui->line_index;
	offset = current_column(line, i);
	col = i - offset;
	col += next_column(line, col);
	offset = 2;
	tputs(tgetstr("cr", NULL), 1, ft_termprint);
	tputs(tgetstr("do", NULL), 1, ft_termprint);
	while (offset--)
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
	while (++col <= target)
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
}

int		move_cursor(unsigned long c, char *line, t_interface *ui, int target)
{
	if (c == LEFT)
		tputs(tgetstr("le", NULL), 1, ft_termprint);
	else if (c == RIGHT)
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
	else if (c == CTL_LEFT)
		move_word_cursor_left(ui, target);
	else if (c == CTL_RIGHT)
		move_word_cursor_right(ui, target);
	else if (c == CTL_DOWN)
		move_line_cursor_down(line, ui, target);
	else if (c == CTL_UP)
		move_line_cursor_up(line, ui, target);
	ui->line_index = target;
	return (target);
}
