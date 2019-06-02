/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 19:31:47 by dysotoma          #+#    #+#             */
/*   Updated: 2019/06/02 10:58:29 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void		move_word_left(char **line, t_interface *ui)
{
	if (ui->line_index == ui->line_len)
	{
		tputs(tgetstr("le", NULL), 1, ft_termprint);
		ui->line_index--;
	}
	while (ui->line_index && !IS_WHITESPACE((*line)[ui->line_index]))
	{
		tputs(tgetstr("le", NULL), 1, ft_termprint);
		ui->line_index--;
	}
	while (ui->line_index && IS_WHITESPACE((*line)[ui->line_index]))
	{
		tputs(tgetstr("le", NULL), 1, ft_termprint);
		ui->line_index--;
	}
	while ((ui->line_index > 0)
		&& !IS_WHITESPACE((*line)[ui->line_index]))
	{
		tputs(tgetstr("le", NULL), 1, ft_termprint);
		ui->line_index--;
	}
}

void		move_word_right(char **line, t_interface *ui)
{
	// if word is not end of line, move to space after word
	while ((ui->line_index != ui->line_len) && !IS_WHITESPACE((*line)[ui->line_index]))
	{
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
		ui->line_index++;
	}
	// if space is not end of line, move past all spaces
	while ((ui->line_index != ui->line_len) && IS_WHITESPACE((*line)[ui->line_index]))
	{
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
		ui->line_index++;
	}
}

void		movement(unsigned long c, char **line, t_interface *interface)
{
	if (c == LEFT
		&& interface->line_index > 0
		&& tputs(tgetstr("le", NULL), 1, ft_termprint) == 0)
		interface->line_index--;
	else if (c == RIGHT
		&& (*line)[interface->line_index]
		&& tputs(tgetstr("nd", NULL), 1, ft_termprint) == 0)
		interface->line_index++;
	else if (c == CTL_LEFT)
		move_word_left(line, interface);
	else if (c == CTL_RIGHT)
		move_word_right(line, interface);
	else if ((c == UP || c == DOWN))
		history(c, line, &interface->h_list, interface);
}
