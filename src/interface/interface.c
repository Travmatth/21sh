/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:42:31 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/09 20:54:24 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		accept(char **str, t_interface *ui, char **tmp, size_t len)
{
	size_t	i;
	size_t	end;
	int		status;

	status = init_accept(str, ui, *tmp, &i);
	while (OK(status) && (*tmp)[i] && (end = ERROR))
	{
		if ((*tmp)[i] == '\\' && (*tmp)[i + 1] == '\n')
			i += 2;
		else if ((P_BACKSLASH(status, tmp, NULL, i, (&end)))
			|| (P_QUOTE(status, tmp, NULL, i, (&end)))
			|| (P_DQUOTE(status, tmp, NULL, i, (&end)))
			|| (P_BACKTICK(status, tmp, NULL, i, (&end)))
			|| (P_ARITH(status, tmp, NULL, i, (&end)))
			|| (P_CMD(status, tmp, NULL, i, (&end)))
			|| (P_EPARAM(status, tmp, NULL, i, (&end))))
		{
			ft_memcpy(&((*str)[len]), &((*tmp)[i]), end + 1);
			i += end;
			len += end;
		}
		(*str)[len++] = (*tmp)[i];
		i += (*tmp)[i] ? 1 : 0;
	}
	return (NIL);
}

int		insert(char c, char **line, t_interface *ui, char **tmp)
{
	int		next;

	if (c == '\n' && accept_line(tmp))
		return (accept(line, ui, tmp, 0));
	else if (violates_line_len(1, *tmp, ui)
		|| (c == '\n' && ui->line_index != ui->line_len))
	{
		write(STDOUT, "\a", 1);
		return (SUCCESS);
	}
	ft_memmove((void*)(*tmp + ui->line_index + 1)
				, (void*)(*tmp + ui->line_index)
				, INPUT_LEN - ui->line_index - 1);
	(*tmp)[ui->line_index] = c;
	if ((!ui->line_index && ERR(init_uiline(ui))))
		return (ERROR);
	ui->line_len += 1;
	next = ui->line_index + 1;
	if (ERR(calculate_uilines(*tmp, ui)))
		return (ERROR);
	write_line(ui, *tmp);
	set_cursor(ui, next);
	return (SUCCESS);
}

void	delete(unsigned long c, char *line, t_interface *ui)
{
	int		next;

	if ((c != DEL && c != DEL2)
		|| ui->line_index <= 0
		|| line[ui->line_index] == '\n')
		return ;
	next = ui->line_index - 1;
	set_cursor(ui, next);
	ft_memmove((void*)(line + next)
		, (void*)(line + ui->line_index)
		, INPUT_LEN - next);
	ui->line_len -= 1;
	next = ui->line_len;
	while (line[next])
		line[next++] = '\0';
	calculate_uilines(line, ui);
	write_line(ui, line);
}

int		init_interface(char *tmp, t_interface *ui)
{
	int		i;

	i = 0;
	while (tmp[i])
		tmp[i++] = '\0';
	ui->line_index = 0;
	ui->line_len = 0;
	init_select(ui);
	free_uiline(&ui->ui_line);
	return (prep_terminal(ui->tty, ~(ICANON | ISIG | ECHO), 1, 0));
}

int		interface(char **line, char **tmp, t_interface *ui)
{
	unsigned long		next;
	int					status;
	int					target;

	status = init_interface(*tmp, ui);
	while (OK(status) && !(next = 0))
	{
		status = ERR(read(STDIN, &next, 6)) ? ERROR : status;
		if (!NONE((status = modify_cli(next, ui, *tmp))))
			continue ;
		else if (!OK(status) && !ui->select && next == INTR)
			status = OK(status) ? NIL : status;
		else if (!ERR((target = move_index(next, *tmp, ui))))
			target != INVALID ? move_cursor(next, ui, target) : 0;
		else if ((next == DEL || next == DEL2))
			delete(next, *tmp, ui);
		else if (next == '\n' || PRINTABLE_CHAR(next))
			status = insert((char)next, line, ui, tmp);
	}
	return (ERR(restore_terminal(&ui->tty[1])) ? ERROR : status);
}
