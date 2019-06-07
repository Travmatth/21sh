/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_copy_paste.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/05 16:12:47 by dysotoma          #+#    #+#             */
/*   Updated: 2019/06/07 16:37:49 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void	copy(t_interface *ui, char *buf, char *line)
{
	int		i;

	ui->select = FALSE;
	if (ERR(ui->ccp_start) || ERR(ui->ccp_end))
		return ;
	i = 0;
	while (buf[i] && i < 4096)
		buf[i++] = '\0';
	i = ui->ccp_end - ui->ccp_start;
	ft_memcpy(buf, &(line[ui->ccp_start]), i);
	init_select(ui);
	write_line(ui, line);
}

static void	cut(t_interface *ui, char *buf, char *line)
{
	int		i;

	ui->select = FALSE;
	if (ERR(ui->ccp_start) || ERR(ui->ccp_end))
		return ;
	i = 0;
	while (buf[i] && i < 4096)
		buf[i++] = '\0';
	i = ui->line_len - ui->ccp_end;
	ft_memcpy(buf, &(line[ui->ccp_start]), ui->ccp_end - ui->ccp_start);
	i = ui->line_len - ui->ccp_end;
	i = i ? i + 1 : i;
	ft_memmove(&(line[ui->ccp_start]), &(line[ui->ccp_end]), i);
	i = ui->ccp_end;
	while (buf[i] && i < 4096)
		buf[i++] = '\0';
	init_select(ui);
	write_line(ui, line);
}

static void	paste(t_interface *ui, char *buf, char *line)
{
	int i;

	if (!buf[0])
		return ;
	i = 0;
	while (buf[i] && i < 4096)
		i += 1;
	ft_memmove(&(line[ui->line_index + i]), &(line[ui->line_index]), i);
	ft_memcpy(&(line[ui->line_index]), buf, i);
	i = 0;
	while (buf[i] && i < 4096)
		buf[i] = '\0';
	init_select(ui);
	ui->select = FALSE;
	write_line(ui, line);
}

unsigned long	swap_key(unsigned long c)
{
	if (c == SHIFT_LEFT)
		return (LEFT);
	else if (c == SHIFT_RIGHT)
		return (RIGHT);
	else if (c == SHIFT_UP)
		return (CTL_UP);
	else if (c == SHIFT_DOWN)
		return (CTL_DOWN);
	return (0);
}

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

static void	select_ccp(unsigned long c, t_interface *ui, char *line)
{
	int				target;
	int				original;
	unsigned long	movement;

	ui->select = TRUE;
	movement = swap_key(c);
	original = ui->line_index;
	ui->ccp_orig = ERR(ui->ccp_orig) ? original : ui->ccp_orig;
	target = move_index(movement, line, ui);
	if (target == INVALID)
		return ;
	else if (target == INVALID && !original && movement == LEFT)
		target = 0;
	if (target != INVALID && target != original)
		target = move_cursor(movement, line, ui, target);
	ui->ccp_start = target < ui->ccp_orig ? target : ui->ccp_orig;
	ui->ccp_end = target < ui->ccp_orig ? ui->ccp_orig + 1 : target;
	write_line(ui, line);
}

void		init_select(t_interface *ui)
{
	ui->ccp_start = ERROR;
	ui->ccp_end = ERROR;
	ui->ccp_orig = ERROR;
}

void		cut_copy_paste(unsigned long c, t_interface *ui, char *line)
{
	static char	buff[4096];

	if (c == SHIFT_LEFT || c == SHIFT_RIGHT || c == SHIFT_UP || c == SHIFT_DOWN)
		select_ccp(c, ui, line);
	else if (c == COPY)
		copy(ui, buff, line);
	else if (c == CUT)
		cut(ui, buff, line);
	else if (buff[0] && c == PASTE)
		paste(ui, buff, line);
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
