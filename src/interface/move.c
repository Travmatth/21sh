/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 19:31:47 by dysotoma          #+#    #+#             */
/*   Updated: 2019/06/04 19:37:06 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void	move_word_left(char **line, t_interface *ui)
{
	if (ui->line_index && (*line)[ui->line_index - 1] == '\n')
		return ;
	if (ui->line_index && ui->line_index == ui->line_len)
	{
		tputs(tgetstr("le", NULL), 1, ft_termprint);
		ui->line_index--;
	}
	while ((ui->line_index > 0)
		&& (*line)[ui->line_index - 1] != '\n'
		&& IS_WHITESPACE((*line)[ui->line_index - 1]))
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
	if (ui->line_index != ui->line_len
		&& (*line)[ui->line_index] == '\n')
		return ;
	while ((ui->line_index != ui->line_len)
		&& (*line)[ui->line_index] != '\n'
		&& !IS_WHITESPACE((*line)[ui->line_index]))
	{
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
		ui->line_index++;
	}
	while ((ui->line_index != ui->line_len)
		&& (*line)[ui->line_index] != '\n'
		&& IS_WHITESPACE((*line)[ui->line_index]))
	{
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
		ui->line_index++;
	}
}

/*
** If we are not able to move to a previous line, exit
** If we are able to, move to the beginning of the current line,
** up one row, then right until we reach the current row or a newline
*/

static void	move_line_up(char **line, t_interface *ui)
{
	int		i;
	int		col;

	if (!line_exists(*line, ui->line_len, PREV))
		return ;
	i = current_column(*line, ui->line_index);
	col = i;
	ui->line_index -= (i ? i + 1 : i);
	while (i--)
		tputs(tgetstr("le", NULL), 1, ft_termprint);
	tputs(tgetstr("up", NULL), 1, ft_termprint);
	ui->line_index -= current_column(*line, --ui->line_index);
	i = -1;
	while (++i < col
		&& (*line)[ui->line_index]
		&& (*line)[ui->line_index] != '\n')
	{
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
		ui->line_index += 1;
	}
}

/*
** If we are not able to move down a line, exit
** If we are able to, move to the beginning of the current line,
** down one row, then right until we reach the current row or a newline
*/

static void	move_line_down(char **line, t_interface *ui)
{
	int		i;
	int		col;

	if (!line_exists(*line, ui->line_index, NEXT))
		return ;
	i = current_column(*line, ui->line_index);
	col = i;
	ui->line_index -= i;
	while (i--)
		tputs(tgetstr("le", NULL), 1, ft_termprint);
	tputs(tgetstr("do", NULL), 1, ft_termprint);
	ui->line_index += next_column(*line, ui->line_index);
	i = -1;
	while (++i < col
		&& (*line)[ui->line_index]
		&& (*line)[ui->line_index] != '\n')
	{
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
		ui->line_index += 1;
	}
}

void		movement(unsigned long c, char **line, t_interface *ui)
{
	if (c == LEFT
		&& ui->line_index > 0
		&& (*line)[ui->line_index - 1] != '\n'
		&& !ERR(tputs(tgetstr("le", NULL), 1, ft_termprint)))
		ui->line_index--;
	else if (c == RIGHT
		&& (*line)[ui->line_index + 1]
		&& (*line)[ui->line_index] != '\n'
		&& !ERR(tputs(tgetstr("nd", NULL), 1, ft_termprint)))
		ui->line_index++;
	else if (c == CTL_LEFT)
		move_word_left(line, ui);
	else if (c == CTL_RIGHT)
		move_word_right(line, ui);
	else if (c == CTL_DOWN)
		move_line_down(line, ui);
	else if (c == CTL_UP)
		move_line_up(line, ui);
	else if ((c == UP || c == DOWN))
		history(c, line, &ui->h_list, ui);
}
