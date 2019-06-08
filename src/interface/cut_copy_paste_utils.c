/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_copy_paste_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 17:51:17 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/07 19:17:15 by tmatthew         ###   ########.fr       */
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

int			is_cut_copy_paste(unsigned long c)
{
	if ((c == SHIFT_LEFT)
		|| (c == SHIFT_RIGHT)
		|| (c == SHIFT_UP)
		|| (c == SHIFT_DOWN)
		|| (c == COPY)
		|| (c == CUT)
		|| (c == PASTE))
		return (TRUE);
	return (FALSE);
}

void		set_cursor(t_interface *ui, char *line)
{
	int		step;
	int		next;
	char	*move;

	if (ui->line_index == ui->ccp_start)
		return ;
	step = ui->line_index > ui->ccp_start ? -1 : 1;
	while (!same_line(line, ui->line_index, ui->ccp_start, step))
	{
		next = step > 0
			? move_line_index_down(line, ui)
			: move_line_index_up(line, ui);
		step > 0
			? move_line_cursor_down(line, ui, next)
			: move_line_cursor_up(line, ui, next);
		step = ui->line_index > ui->ccp_start ? -1 : 1;
	}
	move = ui->line_index > ui->ccp_start ? "le" : "nd";
	while (ui->line_index != ui->ccp_start)
	{
		ui->line_index += step;
		tputs(tgetstr(move, NULL), 1, ft_termprint);
	}
}
