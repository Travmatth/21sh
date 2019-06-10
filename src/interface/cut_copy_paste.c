/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_copy_paste.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/05 16:12:47 by dysotoma          #+#    #+#             */
/*   Updated: 2019/06/10 15:35:12 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void		copy(t_interface *ui, char *buf, char *line)
{
	int		i;

	if (!ui->select)
		return ;
	write(STDOUT, STEADY_CURSOR, 5);
	if (ERR(ui->ccp_start) || ERR(ui->ccp_end))
		return ;
	ft_strclr(buf);
	i = ui->ccp_end - ui->ccp_start;
	ft_memcpy(buf, &(line[ui->ccp_start]), i);
	i = ui->ccp_start;
	clear_all_lines(ui);
	init_select(ui);
	write_line(ui, line);
	ui->line_index = 0;
	set_cursor(ui, i);
}

static int		cut(t_interface *ui, char *buf, char *line)
{
	int		cut_len;
	int		i;

	if (!ui->select || ERR(ui->ccp_start) || ERR(ui->ccp_end))
		return (SUCCESS);
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
	write_line(ui, line);
	ui->line_index = 0;
	set_cursor(ui, ui->ccp_start);
	init_select(ui);
	return (SUCCESS);
}

static int		paste(t_interface *ui, char *buf, char *line)
{
	int		i;
	int		rest;

	if (!ui->select)
		return ;
	write(STDOUT, STEADY_CURSOR, 5);
	if (!(i = ft_strlen(buf)) || violates_line_len(i, line, ui) || NOT_EOL(line, ui->line_index) || ft_strchr(buf, '\n'))
	{
		write(STDOUT, "\a", 1);
		return (SUCCESS);
	}
	rest = ui->line_len - ui->line_index;
	ft_memmove(&(line[ui->line_index + i]), &(line[ui->line_index]), rest);
	ft_memcpy(&(line[ui->line_index]), buf, i);
	ui->line_len += i;
	ft_strclr(buf);
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

int			modify_cli(unsigned long c, t_interface *ui, char *line, int *cont)
{
	int			status;
	static char	buff[INPUT_LEN];

	status = SUCCESS;
	if (c == SHIFT_LEFT || c == SHIFT_RIGHT || c == SHIFT_UP || c == SHIFT_DOWN)
		select_ccp(c, ui, line);
	else if (c == COPY)
		copy(ui, buff, line);
	else if (ui->select && c == CUT)
		status = cut(ui, buff, line);
	else if (buff[0] && c == PASTE)
		status = paste(ui, buff, line);
	else if (!ui->select
		&& (c == UP || c == DOWN)
		&& ui->line_index == ui->line_len)
		history(c, line, &ui->h_list, ui);
	else if (!ui->select && c == HOME)
		set_cursor(ui, 0);
	else if (!ui->select && c == END)
		set_cursor(ui, ui->line_len);
	else if (c == ESC)
		exit_select(ui, line);
	else
		*cont = !ui->select ? FALSE : TRUE;
	return (status);
}
