/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_copy_paste_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 17:51:17 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/10 17:25:30 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"
#define LINE_INDEX_OFFSET i + ui->line_index - 1

void	write_line(t_interface *ui, char *line)
{
	int		i;

	i = -1;
	ft_printf("%s%s%s", tgetstr("sc", NULL), tgetstr("cd", NULL),
	tgetstr("vi", NULL));
	while (ui->line_index && ++i + ui->line_index - 1 < ui->line_len)
	{
		if (line[LINE_INDEX_OFFSET] == '\n')
			write(STDIN, "\n> ", 3);
		else
		{
			if (ui->select && LINE_INDEX_OFFSET >= ui->ccp_start
							&& LINE_INDEX_OFFSET < ui->ccp_end
							&& line[LINE_INDEX_OFFSET])
				tputs(tgetstr("so", NULL), 1, ft_termprint);
			ft_printf("%c%s", line[LINE_INDEX_OFFSET], tgetstr("se", NULL));
		}
	}
	tputs(tgetstr("rc", NULL), 1, ft_termprint);
	if (i > 0 && line[LINE_INDEX_OFFSET] != '\n')
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
	tputs(tgetstr("ve", NULL), 1, ft_termprint);
	if (ui->line_len == ui->line_index && ui->line_index &&
		line[ui->line_index - 1] == '\n' && ui->line_index--)
		move_line_cursor_down(ui, 0);
}

void	set_cursor(t_interface *ui, int position)
{
	int			offset;
	t_uiline	*cur;

	if (ui->line_index == position
		|| !(cur = current_uiline(ui, ui->line_index)))
		return ;
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
	offset = line_exists(ui, ui->line_index, PREV) ? 2 : 3;
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
	offset = ui->ui_line && line_exists(ui, ui->line_index, PREV) ? 2 : 3;
	cur = current_column(line, ui->line_index);
	rest = next_column(line, ui->line_index);
	return (insert + offset + cur + rest > ui->ws.ws_col ? TRUE : FALSE);
}
