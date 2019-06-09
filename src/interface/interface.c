/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:42:31 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/08 23:05:28 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	insert(char c, char *line, t_interface *ui)
{
	int		next;

	if (violates_line_len(1, line, ui)
		|| (c == '\n' && ui->line_index != ui->line_len))
	{
		write(STDOUT, "\a", 1);
		return ;
	}
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

	if ((c != DEL && c != DEL2)
		|| ui->line_index <= 0
		|| line[ui->line_index] == '\n')
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

int		init_interface(t_interface *ui, char tmp[INPUT_LEN], size_t *len)
{
	*len = 0;
	ft_bzero(tmp, INPUT_LEN);
	ui->line_index = 0;
	ui->line_len = 0;
	init_select(ui);
	return (prep_terminal(ui->tty, ~(ICANON | ISIG | ECHO), 1, 0));
}

int		accept(char **line, t_interface *ui, char *tmp, size_t len)
{
	int		ignore;
	int		i;

	set_cursor(ui, tmp, ui->line_len);
	write(STDOUT, "\n", 1);
	write_to_history(tmp, ui);
	if (ERR(push_history(&ui->h_list.hst, tmp))
		|| !(*line = ft_strnew(ui->line_len)))
		return (ERROR);
	i = 0;
	ignore = FALSE;
	while (*line && i < ui->line_len)
	{
		if (!ignore && tmp[i] == '\\' && (tmp)[i + 1] != '\n')
			ignore = TRUE;
		else if (!ignore && tmp[i] == '\\' && (tmp)[i + 1] == '\n')
			i += 2;
		if (ignore)
			ignore = FALSE;
		(*line)[len++] = tmp[i++];
	}
	return (SUCCESS);
}

int		interface(char **line, t_interface *ui)
{
	char				tmp[INPUT_LEN];
	unsigned long		next;
	int					status;
	size_t				len;
	int					target;

	status = init_interface(ui, tmp, &len);
	while (OK(status) && !(next = 0))
	{
		status = ERR(read(STDIN, &next, 6)) ? ERROR : status;
		if (modify_cli(next, ui, tmp))
			continue ;
		else if (!OK(status) && !ui->select && next == INTR)
			status = OK(status) ? NIL : status;
		else if (!ERR((target = move_index(next, tmp, ui))))
			target != INVALID ? move_cursor(next, tmp, ui, target) : 0;
		else if (next == '\n' && accept_line(tmp, ui->line_len - 1)
			&& ((status = accept(line, ui, tmp, len))))
			break ;
		else if ((next == DEL || next == DEL2))
			delete(next, tmp, ui);
		else if ((next == '\n' || PRINTABLE_CHAR(next)))
			insert((char)next, tmp, ui);
	}
	return (ERR(restore_terminal(&ui->tty[1])) ? ERROR : status);
}
