/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 16:36:06 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/10 18:03:07 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		cut(t_interface *ui, char *buf, char *line)
{
	int		cut_len;
	int		i;

	if (!ui->select || ERR(ui->ccp_start) || ERR(ui->ccp_end))
		return (SUCCESS);
	ui->select = FALSE;
	write(STDOUT, STEADY_CURSOR, 5);
	set_cursor(ui, 0);
	ft_strclr(buf);
	cut_len = ui->ccp_end - ui->ccp_start;
	ft_memcpy(buf, &(line[ui->ccp_start]), cut_len);
	i = ui->line_len - ui->ccp_end;
	ft_memmove(&(line[ui->ccp_start]), &(line[ui->ccp_end]), i);
	ui->line_len -= cut_len;
	ft_strclr(&(buf[cut_len]));
	ft_strclr(&(line[ui->line_len]));
	if (ERR(calculate_uilines(line, ui)))
		return (ERROR);
	clear_all_lines(ui);
	i = ui->ccp_start;
	init_select(ui);
	write_line(ui, line);
	ui->line_index = 0;
	set_cursor(ui, i);
	return (SUCCESS);
}

int		cut_line(t_interface *ui, char *buf, char *line)
{
	t_uiline	*cur;
	int			start;
	int			len;
	int			cursor;

	if (ui->select || !(cur = current_uiline(ui, ui->line_index)))
	{
		write(STDOUT, "\a", 1);
		return (SUCCESS);
	}
	cursor = cur->start;
	ft_strclr(buf);
	set_cursor(ui, 0);
	len = cur->end - cur->start;
	len += line[cur->end] == '\n' ? 1 : 0;
	ft_memcpy(buf, &(line[cur->start]), len);
	start = cur->end;
	start += line[start] == '\n' ? 1 : 0;
	ft_memmove(&line[cur->start], &line[start], len);
	ui->line_len -= len;
	ft_strclr(&line[ui->line_len]);
	if (ERR(calculate_uilines(line, ui)))
		return (ERROR);
	clear_all_lines(ui);
	write_line(ui, line);
	ui->line_index = 0;
	set_cursor(ui, cursor);
	return (SUCCESS);
}
