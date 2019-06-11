/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_delete.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 19:48:47 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/10 21:51:38 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		insert(char c, char **line, t_interface *ui, char **tmp)
{
	int		next;

	if (c == '\n' && accept_line(tmp) && OK(accept(line, ui, tmp, 0)))
		return (NIL);
	else if (violates_line_len(1, *tmp, ui) || (c == '\n' && ui->line_index != ui->line_len))
	{
		write(STDOUT, "\a", 1);
		return (SUCCESS);
	}
	ft_memmove((void*)(*tmp + ui->line_index + 1), (void*)(*tmp + ui->line_index), INPUT_LEN - ui->line_index - 1);
	(*tmp)[ui->line_index] = c;
	if ((!ui->line_index && ERR(init_uiline(ui))))
		return (ERROR);
	ui->line_len += 1;
	next = ++ui->line_index;
	if (ERR(calculate_uilines(*tmp, ui)))
		return (ERROR);
	tputs(tgetstr("vi", NULL), 1, ft_termprint);
	clear_all_lines(ui);
	write_line(ui, *tmp);
	ui->line_index = 0;
	set_cursor(ui, next);
	tputs(tgetstr("ve", NULL), 1, ft_termprint);
	return (SUCCESS);
}

int		delete(unsigned long c, char *line, t_interface *ui)
{
	int		i;
	int		next;
	int		is_newline;

	if ((c != DEL && c != DEL2)
		|| ui->line_index <= 0
		|| line[ui->line_index] == '\n')
		return (SUCCESS);
	is_newline = ui->line_index && line[ui->line_index - 1] == '\n' ? 1 : 0;
	next = ui->line_index - (is_newline ? 2 : 1);
	tputs(tgetstr("vi", NULL), 1, ft_termprint);
	ft_memmove((void*)(line + next)
		, (void*)(line + ui->line_index)
		, INPUT_LEN - next);
	ui->line_len -= is_newline ? 2 : 1;
	i = ui->line_len;
	while (line[i])
		line[i++] = '\0';
	clear_all_lines(ui);
	calculate_uilines(line, ui);
	ui->line_index = 0;
	write_line(ui, line);
	set_cursor(ui, next);
	tputs(tgetstr("ve", NULL), 1, ft_termprint);
	return (SUCCESS);
}
