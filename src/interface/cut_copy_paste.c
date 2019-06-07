/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_copy_paste.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/05 16:12:47 by dysotoma          #+#    #+#             */
/*   Updated: 2019/06/06 21:40:35 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void	copy(t_interface *ui, char *buf, char **line)
{
	(void)ui;
	(void)buf;
	(void)line;
	// turn cursor invisible then save cursor at starting position of the
	// copying area with sc. From the staring index copy the line into the buffer
	// up to the end index after which you should use cd to clear to the end
	// starting from the start index and then rewrite after turning off standout
	// mode then return the cursor to the saved position with rc
}

static void	cut(t_interface *ui, char *buf, char **line)
{
	(void)ui;
	(void)buf;
	(void)line;
	// same as above except you need to b_zero the differene of the start and
	// end index in line starting from the start index after copying over to the
	// buffer
}

static void	paste(t_interface *ui, char *buf, char **line)
{
	int i;

	i = 0;
	while (buf[i])
		insert(buf[i], line, ui);
	ft_bzero(buf, ui->ccp_end - ui->ccp_start);
	ui->ccp_start = 0;
	ui->ccp_end = 0;


	// the above should work but the below has been slightly modified from the
	// normal insert and would allow with a few more tweaks the ability to write
	// the whole buf at once
	
	// ft_memmove((void*)(*line + ui->line_index + ui->ccp_end - ui->ccp_start)
	// 			, (void*)(*line + ui->ccp_start)
	// 			, ui->ccp_end - ui->ccp_start); // should be the same as calling strlen of buf
	// // (*line)[ui->line_index] = c;
	// ui->line_len += ui->ccp_start - ui->ccp_end;
	// ui->line_index += ui->ccp_start - ui->ccp_end;
	// tputs(tgetstr("sc", NULL), 1, ft_termprint);
	// tputs(tgetstr("cd", NULL), 1, ft_termprint);
	// tputs(tgetstr("vi", NULL), 1, ft_termprint);
	// ft_printf("%s\n", *line + ui->line_index - 1);
	// tputs(tgetstr("rc", NULL), 1, ft_termprint);
	// tputs(tgetstr("nd", NULL), 1, ft_termprint);
	// tputs(tgetstr("ve", NULL), 1, ft_termprint);

}

static void	select_ccp(unsigned long c, t_interface *ui, char **line)
{
	int				i;
	int				start;

	// first time set start and end index to current subsequent calls should
	// check for one to be grater than the other and set them accordingly
	start = ui->line_index;
	i = move_index(c, *line, ui);
	if (ERR(i) || i == INVALID)
		return ;
	ui->line_index = move_cursor(c, *line, ui, i);
	if (start == ui->line_index)
		return ;
	else if (start > ui->line_index)
	{
		ui->ccp_start = ui->line_index;
		ui->ccp_end = start;
	}
	i = -1;
	ui->ccp_start = start ? start - 1 : start;
	ui->ccp_end = ui->line_index ? ui->line_index - 1 : ui->line_index;
	// save current cursor posiiton
	tputs(tgetstr("sc", NULL), 1, ft_termprint);
	// hide cursor
	// tputs(tgetstr("vi", NULL), 1, ft_termprint);
	// clear screen
	clear_all_lines(*line, ui);
	// rewrite commands
	while (++i < ui->line_len)
	{
		// if newline, print w prompt normally
		if ((*line)[i] == '\n')
			write(STDIN, "\n> ", 3);
		else
		{
			// if inside quoted area, enable highlighting
			if (i >= ui->ccp_start && i <= ui->ccp_end)
				tputs(tgetstr("so", NULL), 1, ft_termprint);
			// print character
			write(STDIN, &(*line)[i], 1);
			// turn off highlighting
			tputs(tgetstr("se", NULL), 1, ft_termprint);
		}
	}
	tputs(tgetstr("ve", NULL), 1, ft_termprint);
	tputs(tgetstr("rc", NULL), 1, ft_termprint);
}

// if line index is not immediately adjacent to either the start or end index or
// is between the 2 reset start and end and clear the buffer

void		cut_copy_paste(unsigned long c, t_interface *ui, char **line)
{
	static char	buff[4096];
	
	if (c == SHIFT_LEFT || c == SHIFT_RIGHT || c == SHIFT_UP || c == SHIFT_DOWN)
		select_ccp(c, ui, line);
	else if (buff[0] && c == COPY)
		copy(ui, buff, line);
	else if (buff[0] && c == CUT)
		cut(ui, buff, line);
	else if (buff[0] && c == PASTE)
		paste(ui, buff, line);
}
