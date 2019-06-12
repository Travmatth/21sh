/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:42:31 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 16:55:59 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		accept_line(char **str)
{
	size_t	i;
	size_t	end;
	int		status;

	i = 0;
	end = ERROR;
	status = SUCCESS;
	while (OK(status) && (*str)[i])
	{
		if ((P_BACKSLASH(status, str, NULL, i, (&end)))
			|| (P_QUOTE(status, str, NULL, i, (&end)))
			|| (P_BACKTICK(status, str, NULL, i, (&end)))
			|| (P_DQUOTE(status, str, NULL, i, (&end)))
			|| (P_ARITH(status, str, NULL, i, (&end)))
			|| (P_CMD(status, str, NULL, i, (&end)))
			|| (P_EPARAM(status, str, NULL, i, (&end)))
			|| (P_SUBSHELL(status, str, NULL, i, (&end)))
			|| (P_CURSHELL(status, str, NULL, i, (&end))))
			i += end;
		i += 1;
	}
	return (status);
}

int		init_accept(char **line, t_interface *ui, char *tmp, size_t *i)
{
	*i = 0;
	set_cursor(ui, ui->line_len);
	write(STDOUT, "\n", 1);
	write_to_history(tmp, ui);
	if (ERR(push_history(&ui->h_list.hst, tmp))
		|| !(*line = ft_strnew(ui->line_len)))
		return (ERROR);
	return (SUCCESS);
}

int		accept(char **str, t_interface *ui, char **tmp, size_t len)
{
	size_t	i;
	size_t	end;
	int		status;

	status = init_accept(str, ui, *tmp, &i);
	while (OK(status) && (*tmp)[i] && (end = ERROR))
	{
		if ((*tmp)[i] == '\\' && (*tmp)[i + 1] == '\n' && (i += 2))
			continue ;
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
