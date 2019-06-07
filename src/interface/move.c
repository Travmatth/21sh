/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 19:31:47 by dysotoma          #+#    #+#             */
/*   Updated: 2019/06/06 14:24:43 by tmatthew         ###   ########.fr       */
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

static void	move_line_up(char **line, t_interface *ui)
{
	int		i;
	int		col;
	int		first;

	if (!line_exists(*line, ui->line_index, PREV))
		return ;
	i = current_column(*line, ui->line_index);
	col = i;
	ui->line_index -= (i ? i + 1 : i);
	while (i--)
		tputs(tgetstr("le", NULL), 1, ft_termprint);
	tputs(tgetstr("up", NULL), 1, ft_termprint);
	ui->line_index -= current_column(*line, --ui->line_index);
	first = !line_exists(*line, ui->line_index, PREV) ? 1 : 0;
	while (first--)
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
	while (++i < col
		&& (*line)[ui->line_index]
		&& (*line)[ui->line_index] != '\n')
	{
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
		ui->line_index += 1;
	}
}

static void	move_line_down(char **line, t_interface *ui)
{
	int		i;
	int		col;
	int		first;

	if (!line_exists(*line, ui->line_index, NEXT))
		return ;
	i = current_column(*line, ui->line_index);
	col = i;
	ui->line_index -= i;
	first = 2;
	while (i--)
		tputs(tgetstr("le", NULL), 1, ft_termprint);
	tputs(tgetstr("do", NULL), 1, ft_termprint);
	ui->line_index += next_column(*line, ui->line_index);
	while (first--)
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
	while (++i < col
		&& (*line)[ui->line_index]
		&& (*line)[ui->line_index] != '\n')
	{
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
		ui->line_index += 1;
	}
}

void		movement(unsigned long *c, char **line, t_interface *ui)
{
	if (*c == LEFT
		&& ui->line_index > 0
		&& !ERR(tputs(tgetstr("le", NULL), 1, ft_termprint)))
		ui->line_index--;
	else if (*c == RIGHT
		&& (*line)[ui->line_index]
		&& !ERR(tputs(tgetstr("nd", NULL), 1, ft_termprint)))
		ui->line_index++;
	else if (*c == CTL_LEFT)
		move_word_left(line, ui);
	else if (*c == CTL_RIGHT)
		move_word_right(line, ui);
	else if (*c == CTL_DOWN)
		move_line_down(line, ui);
	else if (*c == CTL_UP)
		move_line_up(line, ui);
	else if ((*c == UP || *c == DOWN) && ui->line_index == ui->line_len)
		history(*c, line, &ui->h_list, ui);
	else if (*c == SHIFT_LEFT || *c == SHIFT_RIGHT || *c == SHIFT_UP ||
		*c == SHIFT_DOWN || *c == (int)'C' || *c == (int)'V' || *c == (int)'X')
		cut_copy_paste(c, ui, line);
}

void		move_index(unsigned long *c, char **line, t_interface *ui)
{
	if (*c == LEFT && ui->line_index > 0)
		ui->line_index--;
	else if (*c == RIGHT && (*line)[ui->line_index])
		ui->line_index++;
	else if (*c == CTL_LEFT)
		move_word_index_left(line, ui);
	else if (*c == CTL_RIGHT)
		move_word_index_right(line, ui);
	else if (*c == CTL_DOWN)
		move_line_index_down(line, ui);
	else if (*c == CTL_UP)
		move_line_index_up(line, ui);
}
