/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 19:31:47 by dysotoma          #+#    #+#             */
/*   Updated: 2019/06/02 15:28:40 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void	move_word_left(char **line, t_interface *ui)
{
	if (ui->line_index == ui->line_len)
	{
		tputs(tgetstr("le", NULL), 1, ft_termprint);
		ui->line_index--;
	}
	while ((ui->line_index > 0) && IS_WHITESPACE((*line)[ui->line_index - 1]))
	{
		tputs(tgetstr("le", NULL), 1, ft_termprint);
		ui->line_index--;
	}
	while ((ui->line_index > 0)
		&& !IS_WHITESPACE((*line)[ui->line_index - 1]))
	{
		tputs(tgetstr("le", NULL), 1, ft_termprint);
		ui->line_index--;
	}
}

static void	move_word_right(char **line, t_interface *ui)
{
	while ((ui->line_index != ui->line_len)
		&& !IS_WHITESPACE((*line)[ui->line_index]))
	{
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
		ui->line_index++;
	}
	while ((ui->line_index != ui->line_len)
		&& IS_WHITESPACE((*line)[ui->line_index]))
	{
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
		ui->line_index++;
	}
}

void		movement(unsigned long c, char **line, t_interface *interface)
{
	if (c == LEFT
		&& interface->line_index > 0
		&& !ERR(tputs(tgetstr("le", NULL), 1, ft_termprint)))
		interface->line_index--;
	else if (c == RIGHT
		&& (*line)[interface->line_index]
		&& !ERR(tputs(tgetstr("nd", NULL), 1, ft_termprint)))
		interface->line_index++;
	else if (c == CTL_LEFT)
		move_word_left(line, interface);
	else if (c == CTL_RIGHT)
		move_word_right(line, interface);
	else if ((c == UP || c == DOWN))
		history(c, line, &interface->h_list, interface);
}
