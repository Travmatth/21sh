/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dysotoma <dysotoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 19:31:47 by dysotoma          #+#    #+#             */
/*   Updated: 2019/06/05 22:07:26 by dysotoma         ###   ########.fr       */
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

int			is_previous_line(char *line, int start_index, int *col)
{
	int		i;

	i = --start_index;
	while (i)
	{
		if (line[i] == '\n')
		{
			*col = start_index - i;
			return (TRUE);
		}
		i -= 1;
	}
	return (FALSE);
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

	if (!is_previous_line(*line, ui->line_index, &col))
		return ;
	i = col;
	while (i--)
		tputs(tgetstr("le", NULL), 1, ft_termprint);
	tputs(tgetstr("up", NULL), 1, ft_termprint);
	i = -1;
	while (++i < col
		&& (*line)[i + 1]
		&& (*line)[i + 2]
		&& (*line)[i + 2] != '\n')
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
}

int			is_following_line(char *line, int start_index, int *col)
{
	int		i;

	i = --start_index;
	if (!is_previous_line(line, start_index, col))
		*col = start_index - 1;
	while (i)
	{
		if (line[i] == '\n')
			return (TRUE);
		i -= 1;
	}
	return (FALSE);
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

	if (!is_following_line(*line, ui->line_index, &col))
		return ;
	i = col;
	while (i--)
		tputs(tgetstr("le", NULL), 1, ft_termprint);
	tputs(tgetstr("do", NULL), 1, ft_termprint);
	i = -1;
	while (++i < col && (*line)[i])
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
	if (col)
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
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
	else if ((*c == UP || *c == DOWN))
		history(*c, line, &ui->h_list, ui);
	else if (*c == SHIFT_LEFT || *c == SHIFT_RIGHT || *c == SHIFT_UP ||
		*c == SHIFT_DOWN || *c == (int)'C' || *c == (int)'V' || *c == (int)'X')
		cut_copy_paste(c, ui, line);
}
