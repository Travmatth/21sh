/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_cursor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/06 14:14:11 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/09 23:46:32 by tmatthew         ###   ########.fr       */
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

void	move_line_cursor_up(t_interface *ui, int target)
{
	t_uiline	*cur;
	int			offset;

	if (!(cur = current_uiline(ui)))
		return ;
	offset = line_exists(ui, PREV) ? 3 : 2;
	tputs(tgetstr("cr", NULL), 1, ft_termprint);
	tputs(tgetstr("up", NULL), 1, ft_termprint);
	while (offset--)
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
	ui->line_index = cur->prev->start;
	while (ui->line_index < target)
	{
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
		ui->line_index += 1;
	}
}

void	move_line_cursor_down(t_interface *ui, int target)
{
	t_uiline	*cur;
	int			offset;

	if (!(cur = current_uiline(ui)))
		return ;
	tputs(tgetstr("cr", NULL), 1, ft_termprint);
	tputs(tgetstr("do", NULL), 1, ft_termprint);
	offset = 2;
	while (offset--)
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
	ui->line_index = cur->next->start;
	while (ui->line_index < target)
	{
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
		ui->line_index += 1;
	}
}

int		move_cursor(unsigned long c, t_interface *ui, int target)
{
	if (c == LEFT || c == SHIFT_LEFT)
		tputs(tgetstr("le", NULL), 1, ft_termprint);
	else if (c == RIGHT || c == SHIFT_RIGHT)
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
	else if (c == CTL_LEFT)
		move_word_cursor_left(ui, target);
	else if (c == CTL_RIGHT)
		move_word_cursor_right(ui, target);
	else if (c == CTL_DOWN || c == SHIFT_DOWN)
		move_line_cursor_down(ui, target);
	else if (c == CTL_UP || c == SHIFT_UP)
		move_line_cursor_up(ui, target);
	ui->line_index = target;
	return (target);
}
