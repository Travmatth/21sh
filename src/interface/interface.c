/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:42:31 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/10 21:44:54 by tmatthew         ###   ########.fr       */
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
		else if (OK((status = parse_all(tmp, i, (&end), NULL)))
			&& end != (size_t)ERROR)
		{
			ft_memcpy(&((*str)[len]), &((*tmp)[i]), end + 1);
			i += end;
			len += end;
		}
		if (OK(status))
		{
			(*str)[len++] = (*tmp)[i];
			i += (*tmp)[i] ? 1 : 0;
		}
	}
	return (!(*tmp)[i] ? SUCCESS : NIL);
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
	int					cont;

	status = init_interface(*tmp, ui);
	while (OK(status) && !(next = 0))
	{
		status = ERR(read(STDIN, &next, 6)) ? ERROR : status;
		if (OK((status = modify_cli(next, ui, *tmp, &cont)))
			&& OK(cont))
			continue ;
		else if (!OK(status) && !ui->select && next == INTR)
			status = OK(status) ? NIL : status;
		else if (!ERR((target = move_index(next, *tmp, ui))))
			target != INVALID ? move_cursor(next, ui, target) : 0;
		else if ((next == DEL || next == DEL2))
			status = delete(next, *tmp, ui);
		else if (next == '\n' || PRINTABLE_CHAR(next))
			status = insert((char)next, line, ui, tmp);
	}
	return (ERR(restore_terminal(&ui->tty[1])) ? ERROR : status);
}
