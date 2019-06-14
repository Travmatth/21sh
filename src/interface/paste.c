/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paste.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 16:37:28 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/13 19:18:51 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		should_paste(char *line, t_interface *ui, char *buf)
{
	int		i;

	if (!(i = ft_strlen(buf)) || violates_line_len(i, line, ui)
		|| NOT_EOL(line, ui->line_index) || ft_strchr(buf, '\n')
		|| ui->buf_origin != SELECT)
	{
		write(STDOUT, "\a", 1);
		return (SUCCESS);
	}
	ui->buf_origin = BLANK;
	return (SUCCESS);
}

int		paste(t_interface *ui, char *buf, char *line)
{
	int		i;
	int		rest;

	if (ERR(should_paste(line, ui, buf)))
		return (SUCCESS);
	i = ft_strlen(buf);
	ui->select = FALSE;
	write(STDOUT, STEADY_CURSOR, 5);
	rest = ui->line_len - ui->line_index + 1;
	ft_memmove(&(line[ui->line_index + i]), &(line[ui->line_index]), rest);
	ft_memcpy(&(line[ui->line_index]), buf, i);
	ui->line_len += i;
	if (ERR(calculate_uilines(line, ui)))
		return (ERROR);
	clear_all_lines(ui);
	write_line(ui, line);
	i = ui->line_index;
	ui->line_index = 0;
	set_cursor(ui, i);
	init_select(ui);
	return (SUCCESS);
}

int		accept_paste_line(t_interface *ui, char *line, int cursor)
{
	ft_strclr(&line[ui->line_len]);
	set_cursor(ui, 0);
	if (ERR(calculate_uilines(line, ui)))
		return (ERROR);
	clear_all_lines(ui);
	write_line(ui, line);
	ui->line_index = 0;
	set_cursor(ui, cursor);
	return (SUCCESS);
}

int		should_paste_line(t_interface *ui, char *buf, t_uiline **cur)
{
	if (ui->select || !buf[0] || !(*cur = current_uiline(ui, ui->line_index))
		|| ui->buf_origin != FULL_LINE)
	{
		write(STDOUT, "\a", 1);
		return (ERROR);
	}
	ui->buf_origin = BLANK;
	return (SUCCESS);
}

int		paste_line(t_interface *ui, char *buf, char *line)
{
	t_uiline	*cur;
	int			len;
	int			cursor;
	int			start;

	if (ERR(should_paste_line(ui, buf, &cur)))
		return (SUCCESS);
	cursor = cur->start;
	start = cur->start;
	len = strlen(buf);
	if ((!cur->start || cur->end != ui->line_len) && buf[len - 1] != '\n')
	{
		buf[len++] = '\\';
		buf[len++] = '\n';
		if (len + (line_exists(ui, cur->start, PREV) ? 2 : 3) > ui->ws.ws_col)
		{
			write(STDOUT, "\a", 1);
			return (SUCCESS);
		}
	}
	ft_memmove(&(line[cur->start + len]), &(line[cur->start])
		, ui->line_len - cur->start + 1);
	ft_memcpy(&(line[cur->start]), buf, len);
	ui->line_len += len;
	return (accept_paste_line(ui, line, cursor));
}
