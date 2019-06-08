/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_copy_paste_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 17:51:17 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/08 13:29:38 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void		write_line(t_interface *ui, char *line)
{
	int		i;

	tputs(tgetstr("sc", NULL), 1, ft_termprint);
	tputs(tgetstr("vi", NULL), 1, ft_termprint);
	i = -1;
	clear_all_lines(line, ui);
	while (++i < ui->line_len)
	{
		if (line[i] == '\\' && line[i + 1] == '\n')
		{
			write(STDIN, "\\\n> ", 4);
			i += 1;
		}
		else
		{
			if (ui->select && i >= ui->ccp_start && i < ui->ccp_end && line[i])
				tputs(tgetstr("so", NULL), 1, ft_termprint);
			write(STDIN, &line[i], 1);
			tputs(tgetstr("se", NULL), 1, ft_termprint);
		}
	}
	tputs(tgetstr("ve", NULL), 1, ft_termprint);
	tputs(tgetstr("rc", NULL), 1, ft_termprint);
}

void		init_select(t_interface *ui)
{
	ui->ccp_start = ERROR;
	ui->ccp_end = ERROR;
	ui->ccp_orig = ERROR;
}

int		same_line(char *line, int current, int target, int step)
{
	while (TRUE)
	{
		if (current == target && line[current] == '\n' && step > 0)
			return (TRUE);
		else if (current == target && line[current] != '\n')
			return (TRUE);
		else if (line[current] == '\n')
			return (FALSE);
		current += step;
	}
}

void		set_cursor(t_interface *ui, char *line, int position)
{
	int		step;
	int		next;
	char	*move;

	if (ui->line_index == position)
		return ;
	step = ui->line_index > position ? -1 : 1;
	while (!same_line(line, ui->line_index, position, step))
	{
		next = move_index(step > 0 ? SHIFT_DOWN : SHIFT_UP, line, ui);
		move_cursor(step > 0 ? SHIFT_DOWN : SHIFT_UP, line, ui, next);
		step = ui->line_index > position ? -1 : 1;
	}
	move = ui->line_index > position ? "le" : "nd";
	while (ui->line_index != position)
	{
		ui->line_index += step;
		tputs(tgetstr(move, NULL), 1, ft_termprint);
	}
}
