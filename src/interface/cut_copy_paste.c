/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_copy_paste.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/05 16:12:47 by dysotoma          #+#    #+#             */
/*   Updated: 2019/06/09 19:34:57 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void		copy(t_interface *ui, char *buf, char *line)
{
	int		i;

	ui->select = FALSE;
	write(STDOUT, STEADY_CURSOR, 5);
	if (ERR(ui->ccp_start) || ERR(ui->ccp_end))
		return ;
	i = 0;
	while (buf[i] && i < INPUT_LEN)
		buf[i++] = '\0';
	i = ui->ccp_end - ui->ccp_start;
	ft_memcpy(buf, &(line[ui->ccp_start]), i);
	init_select(ui);
	write_line(ui, line);
}

static int		cut(t_interface *ui, char *buf, char *line)
{
	int		cut_len;
	int		i;

	ui->select = FALSE;
	write(STDOUT, STEADY_CURSOR, 5);
	if (ERR(ui->ccp_start) || ERR(ui->ccp_end))
		return (SUCCESS);
	set_cursor(ui, ui->ccp_start);
	i = 0;
	while (buf[i] && i < INPUT_LEN)
		buf[i++] = '\0';
	cut_len = ui->ccp_end - ui->ccp_start;
	ft_memcpy(buf, &(line[ui->ccp_start]), cut_len);
	i = ui->line_len - ui->ccp_end;
	ft_memmove(&(line[ui->ccp_start]), &(line[ui->ccp_end]), i);
	ui->line_len -= cut_len;
	i = ui->line_len;
	while ((line[i] || line[i + 1]) && i < INPUT_LEN - 1)
		line[i++] = '\0';
	init_select(ui);
	if (ERR(calculate_uilines(line, ui)))
		return (ERROR);
	write_line(ui, line);
	return (SUCCESS);
}

static int		paste(t_interface *ui, char *buf, char *line)
{
	int		i;
	int		rest;

	ui->select = FALSE;
	write(STDOUT, STEADY_CURSOR, 5);
	i = 0;
	while (buf[i] && i < INPUT_LEN)
		i += 1;
	if (!i || violates_line_len(i, line, ui)
		|| NOT_EOL(line, ui->line_index) || ft_strchr(buf, '\n'))
	{
		write(STDOUT, "\a", 1);
		return (SUCCESS);
	}
	rest = ui->line_len - ui->line_index;
	ft_memmove(&(line[ui->line_index + i]), &(line[ui->line_index]), rest);
	ft_memcpy(&(line[ui->line_index]), buf, i);
	ui->line_len += i;
	while (i >= 0)
		buf[i--] = '\0';
	init_select(ui);
	if (ERR(calculate_uilines(line, ui)))
		return (ERROR);
	write_line(ui, line);
	return (SUCCESS);
}

static void	select_ccp(unsigned long c, t_interface *ui, char *line)
{
	int				target;
	int				original;

	if (!ui->select)
		write(STDOUT, BLINK_CURSOR, 5);
	ui->select = TRUE;
	original = ui->line_index;
	ui->ccp_orig = ERR(ui->ccp_orig) ? original : ui->ccp_orig;
	target = move_index(c, line, ui);
	if (target == INVALID || (!escaped(line, original)
		&& line[original] == '\\' && line[target] == '\n'))
	{
		write(STDIN, "\a", 1);
		return ;
	}
	else if (target == INVALID && !original && c == LEFT)
		target = 0;
	if (target != INVALID && target != original)
		target = move_cursor(c, ui, target);
	ui->ccp_start = target < ui->ccp_orig ? target : ui->ccp_orig;
	ui->ccp_end = target < ui->ccp_orig ? ui->ccp_orig + 1 : target;
	write_line(ui, line);
}

int			modify_cli(unsigned long c, t_interface *ui, char *line)
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
	else if (!ui->select)
		return (NIL);
	return (status);
}
