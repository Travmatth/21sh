/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_delete.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 19:48:47 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/12 17:14:42 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		insert(char c, char **line, t_interface *ui, char **tmp)
{
	int		next;

	if (c == '\n' && accept_line(tmp) && OK(accept(line, ui, tmp, 0)))
		return (NIL);
	else if (violates_line_len(1, *tmp, ui) || (c == '\n' && NOT_END(ui)))
	{
		write(STDOUT, "\a", 1);
		return (SUCCESS);
	}
	tputs(tgetstr("vi", NULL), 1, ft_termprint);
	ft_memmove((void*)(*tmp + ui->line_index + 1)
				, (void*)(*tmp + ui->line_index)
				, INPUT_LEN - ui->line_index - 1);
	(*tmp)[ui->line_index] = c;
	next = ui->line_index + 1;
	set_cursor(ui, 0);
	clear_all_lines(ui);
	ui->line_len += 1;
	if ((!ui->line_index && ERR(init_uiline(ui)))
		|| ERR(calculate_uilines(*tmp, ui)))
		return (ERROR);
	write_line(ui, *tmp);
	set_cursor(ui, next);
	tputs(tgetstr("ve", NULL), 1, ft_termprint);
	return (SUCCESS);
}

int		delete(char *line, t_interface *ui)
{
	int		next;
	int		is_newline;

	is_newline = ui->line_index && line[ui->line_index - 1] == '\n' ? 1 : 0;
	next = ui->line_index - (is_newline ? 2 : 1);
	if (!ui->line_index || line[ui->line_index] == '\n' || next < 0)
	{
		write(STDOUT, "\a", 1);
		return (SUCCESS);
	}
	tputs(tgetstr("vi", NULL), 1, ft_termprint);
	ft_memmove(&line[next], &line[ui->line_index], ui->line_len - next);
	ui->line_len -= is_newline ? 2 : 1;
	ft_strclr(&line[ui->line_len + 1]);
	clear_all_lines(ui);
	calculate_uilines(line, ui);
	ui->line_index = 0;
	write_line(ui, line);
	set_cursor(ui, next);
	tputs(tgetstr("ve", NULL), 1, ft_termprint);
	return (SUCCESS);
}
