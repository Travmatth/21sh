/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_copy_paste_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 17:51:17 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/09 22:42:21 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	write_line(t_interface *ui, char *line)
{
	int		i;

	i = -1;
	ft_printf("%s%s%s", tgetstr("sc", NULL), tgetstr("cd", NULL),
	tgetstr("vi", NULL));
	while (++i + ui->line_index - 1 < ui->line_len)
	{
		if (line[i + ui->line_index - 1] == '\n')
			write(STDIN, "\n> ", 3);
		else
		{
			if (ui->select && i + ui->line_index - 1 >= ui->ccp_start
							&& i + ui->line_index - 1 < ui->ccp_end
							&& line[i + ui->line_index - 1])
				tputs(tgetstr("so", NULL), 1, ft_termprint);
			write(STDIN, &line[i + ui->line_index - 1], 1);
			tputs(tgetstr("se", NULL), 1, ft_termprint);
		}
	}
	tputs(tgetstr("rc", NULL), 1, ft_termprint);
	if (i > 0 && line[i + ui->line_index - 1] != '\n')
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
	tputs(tgetstr("ve", NULL), 1, ft_termprint);
	if (ui->line_len == ui->line_index && line[ui->line_index - 1] == '\n')
		move_line_cursor_down(ui, 0);
}

void	init_select(t_interface *ui)
{
	ui->ccp_start = ERROR;
	ui->ccp_end = ERROR;
	ui->ccp_orig = ERROR;
}

void	set_cursor(t_interface *ui, int position)
{
	int			offset;
	t_uiline	*cur;

	if (ui->line_index == position)
		return ;
	cur = current_uiline(ui);
	tputs(tgetstr("cr", NULL), 1, ft_termprint);
	while (position < cur->start || (cur->end != ERROR && position > cur->end))
	{
		if (position < cur->start)
			tputs(tgetstr("up", NULL), 1, ft_termprint);
		else if (cur->end != ERROR && position > cur->end)
			tputs(tgetstr("do", NULL), 1, ft_termprint);
		cur = position < cur->start ? cur->prev : cur->next;
	}
	ui->line_index = cur->start;
	offset = line_exists(ui, PREV) ? 2 : 3;
	while (offset--)
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
	while (ui->line_index != position)
	{
		ui->line_index += ui->line_index > position ? -1 : 1;
		tputs(tgetstr(MOVE(ui, position), NULL), 1, ft_termprint);
	}
}

int		violates_line_len(int insert, char *line, t_interface *ui)
{
	int		offset;
	int		cur;
	int		rest;

	offset = ui->ui_line ? 0 : 3;
	offset = ui->ui_line && line_exists(ui, PREV) ? 2 : 3;
	cur = current_column(line, ui->line_index);
	rest = next_column(line, ui->line_index);
	return (insert + offset + cur + rest > ui->ws.ws_col ? TRUE : FALSE);
}
