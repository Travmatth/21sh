/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_copy_paste.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/05 16:12:47 by dysotoma          #+#    #+#             */
/*   Updated: 2019/06/07 00:08:31 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void	copy(t_interface *ui, char *buf, char **line)
{
	int		i;

	(void)buf;
	(void)line;
	i = 0;
	// turn cursor invisible
	//save cursor at starting position of the copying area
	// From the starting index copy the line into the buffer
	// up to the end index
	ui->ccp_start = 0;
	ui->ccp_end = 0;
	ui->select = FALSE;
}

static void	cut(t_interface *ui, char *buf, char **line)
{
	(void)buf;
	(void)line;
	ui->select = FALSE;
	ui->ccp_start = 0;
	ui->ccp_end = 0;
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
	ui->select = FALSE;


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
	// tputs(tgetstr("vi", NULL), 1, ft_termprint);
	i = -1;
	clear_all_lines(line, ui);
	while (++i < ui->line_len)
	{
		if (line[i] == '\\' && line[i + 1] == '\n')
			write(STDIN, "\\\n> ", 4);
		else
		{
			if (i >= ui->ccp_start && i < ui->ccp_end)
				tputs(tgetstr("so", NULL), 1, ft_termprint);
			write(STDIN, &line[i], 1);
			tputs(tgetstr("se", NULL), 1, ft_termprint);
		}
	}
	// tputs(tgetstr("ve", NULL), 1, ft_termprint);
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
	target = move_index(movement, line, ui);
	if (target == INVALID || (!escaped(line, target) && !line[target]))
		return ;
	if (target != original)
		target = move_cursor(movement, line, ui, target);
	// if start is not recorded, do so
	ui->ccp_start = ERR(ui->ccp_start) ? original : ui->ccp_start;
	// if start is less than target, we're selecting forward
	if (ui->ccp_start <= target)
		ui->ccp_end = target;
	// if start is greater than target, we're selecting backward
	else if (ui->ccp_start > target)
	{
		ui->ccp_end = ui->ccp_start;
		ui->ccp_start = target;
	}
	write_line(ui, line);
}

// if line index is not immediately adjacent to either the start or end index or
// is between the 2 reset start and end and clear the buffer

void		cut_copy_paste(unsigned long c, t_interface *ui, char **line)
{
	static char	buff[4096];

	if (c == SHIFT_LEFT || c == SHIFT_RIGHT || c == SHIFT_UP || c == SHIFT_DOWN)
		select_ccp(c, ui, *line);
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
