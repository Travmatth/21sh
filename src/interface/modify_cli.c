/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_cli.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/05 16:12:47 by dysotoma          #+#    #+#             */
/*   Updated: 2019/06/11 18:26:54 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		is_cut_copy_paste(unsigned long c, t_interface *ui, char *buf)
{
	if (c == SHIFT_LEFT
		|| c == SHIFT_RIGHT
		|| c == SHIFT_UP
		|| c == SHIFT_DOWN
		|| (ui->select && c == COPY)
		|| c == OPT_C
		|| (ui->select && c == CUT)
		|| c == OPT_X
		|| (buf[0] && c == PASTE)
		|| (buf[0] && c == OPT_P))
		return (TRUE);
	return (FALSE);
}

int		cut_copy_paste(unsigned long c
					, t_interface *ui
					, char *line
					, char *buf)
{
	int		status;

	status = SUCCESS;
	if (c == SHIFT_LEFT || c == SHIFT_RIGHT || c == SHIFT_UP || c == SHIFT_DOWN)
		select_ccp(c, ui, line);
	else if (ui->select && c == COPY)
		copy(ui, buf, line);
	else if (c == OPT_C)
		copy_line(ui, buf, line);
	else if (ui->select && c == CUT)
		status = cut(ui, buf, line);
	else if (c == OPT_X)
		status = cut_line(ui, buf, line);
	else if (buf[0] && c == PASTE)
		status = paste(ui, buf, line);
	else if (buf[0] && c == OPT_P)
		status = paste_line(ui, buf, line);
	return (status);
}

int		modify_cli(unsigned long c, t_interface *ui, char *line, int *cont)
{
	int			status;
	static char	buf[INPUT_LEN];

	status = SUCCESS;
	*cont = TRUE;
	if (is_cut_copy_paste(c, ui, buf))
		status = cut_copy_paste(c, ui, line, buf);
	else if (!ui->select && (c == UP || c == DOWN)
		&& ui->line_index == ui->line_len)
		history(c, line, &ui->h_list, ui);
	else if (!ui->select && c == HOME)
		set_cursor(ui, 0);
	else if (!ui->select && c == END)
		set_cursor(ui, ui->line_len);
	else if (c == ESC)
		exit_select(ui, line);
	else if (!ui->select)
		*cont = FALSE;
	return (status);
}
