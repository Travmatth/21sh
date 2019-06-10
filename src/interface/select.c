/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 15:34:39 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/10 15:42:15 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	exit_select(t_interface *ui, char *line)
{
	if (ui->select)
	{
		ui->select = FALSE;
		clear_all_lines(ui);
		init_select(ui);
		write_line(ui, line);
		set_cursor(ui, ui->line_index);
	}
}

void	init_select(t_interface *ui)
{
	ui->ccp_start = ERROR;
	ui->ccp_end = ERROR;
	ui->ccp_orig = ERROR;
}

void	select_ccp(unsigned long c, t_interface *ui, char *line)
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
		write(STDOUT, STEADY_CURSOR, 5);
		return ;
	}
	ui->ccp_start = target < ui->ccp_orig ? target : ui->ccp_orig;
	ui->ccp_end = target < ui->ccp_orig ? ui->ccp_orig + 1 : target;
	clear_all_lines(ui);
	write_line(ui, line);
	ui->line_index = 0;
	if (target != INVALID && target != original)
		set_cursor(ui, target);
}
