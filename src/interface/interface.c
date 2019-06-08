/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:42:31 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/08 14:45:21 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	insert(char c, char *line, t_interface *ui)
{
	int		next;
	int		offset;
	int		cur;
	int		rest;

	offset = line_exists(line, ui->line_index, PREV) ? 2 : 3;
	cur = current_column(line, ui->line_index);
	rest = next_column(line, ui->line_index);
	if (offset + cur + rest >= ui->ws.ws_col)
		return ;
	ft_memmove((void*)(line + ui->line_index + 1)
				, (void*)(line + ui->line_index)
				, INPUT_LEN - ui->line_index - 1);
	line[ui->line_index] = c;
	ui->line_len += 1;
	write_line(ui, line);
	next = ui->line_index + 1;
	set_cursor(ui, line, next);
}

void	delete(unsigned long c, char *line, t_interface *ui)
{
	int		next;
	int		is_newline;

	if ((c != DEL && c != DEL2) || ui->line_index <= 0)
		return ;
	is_newline = ui->line_index && line[ui->line_index - 1] == '\n' ? 1 : 0;
	next = ui->line_index - (is_newline ? 2 : 1);
	set_cursor(ui, line, next);
	ft_memmove((void*)(line + next)
		, (void*)(line + ui->line_index)
		, INPUT_LEN - next);
	ui->line_len -= is_newline ? 2 : 1;
	next = ui->line_len;
	while (line[next])
		line[next++] = '\0';
	write_line(ui, line);
}

int		init_interface(t_interface *ui
							, char **line
							, char tmp[INPUT_LEN]
							, size_t *len)
{
	if (ui->h_list.hst)
		ui->curr_last_history = ui->h_list.hst->content;
	*len = 0;
	ft_bzero(tmp, INPUT_LEN);
	*line = tmp;
	ui->line_index = 0;
	ui->line_len = 0;
	return (prep_terminal(ui->tty, ~(ICANON | ISIG | ECHO), 1, 0));
}

int		accept(char **line, t_interface *ui, char *tmp, size_t len)
{
	int		status;

	status = SUCCESS;
	set_cursor(ui, *line, ui->line_len);
	write(STDOUT, "\n", 1);
	write_to_history(line, ui);
	status = OK(status) && push_history(&ui->h_list.hst, *line);
	*line = ft_strnew(ui->line_len);
	ui->line_index = 0;
	while (*line && ui->line_index != ui->line_len)
	{
		if ((tmp)[ui->line_index] == '\\' && (tmp)[ui->line_index + 1] == '\n')
			ui->line_index += 2;
		else
			(*line)[len++] = tmp[ui->line_index++];
	}
	status = !*line ? ERROR : status;
	return (status);
}

int		interface(char **line, t_interface *ui)
{
	char				tmp[INPUT_LEN];
	unsigned long		next;
	int					status;
	size_t				len;
	int					target;

	status = init_interface(ui, line, tmp, &len);
	while (OK(status) && !(next = 0))
	{
		status = ERR(read(STDIN, &next, 6)) ? ERROR : status;
		if (modify_cli(next, ui, *line))
			continue ;
		else if (!ui->select && next == INTR)
			status = NIL;
		else if (OK(status) && !ERR((target = move_index(next, *line, ui))))
			target != INVALID ? move_cursor(next, *line, ui, target) : 0;
		else if (ACCEPT_LINE(status, next, line, ui)
			&& ((status = accept(line, ui, tmp, len))))
			break ;
		else if (OK(status) && (next == DEL || next == DEL2))
			delete(next, *line, ui);
		else if (OK(status) && (next == '\n' || PRINTABLE_CHAR(next)))
			insert((char)next, *line, ui);
	}
	return (ERR(restore_terminal(&ui->tty[1])) ? ERROR : status);
}
