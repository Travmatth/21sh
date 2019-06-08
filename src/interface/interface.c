/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:42:31 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/07 22:38:31 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	insert(char c, char **line, t_interface *interface)
{
	ft_memmove((void*)(*line + interface->line_index + 1)
				, (void*)(*line + interface->line_index)
				, INPUT_LEN - interface->line_index - 1);
	(*line)[interface->line_index] = c;
	interface->line_len++;
	interface->line_index++;
	if (interface->line_index != interface->line_len)
	{
		tputs(tgetstr("sc", NULL), 1, ft_termprint);
		tputs(tgetstr("cd", NULL), 1, ft_termprint);
		tputs(tgetstr("vi", NULL), 1, ft_termprint);
		ft_printf("%s\n", *line + interface->line_index - 1);
		tputs(tgetstr("rc", NULL), 1, ft_termprint);
		tputs(tgetstr("nd", NULL), 1, ft_termprint);
		tputs(tgetstr("ve", NULL), 1, ft_termprint);
	}
	else
	{
		write(STDIN, &c, 1);
		if (c == '\n')
			write(STDOUT, "> ", 2);
	}
}

void	delete(unsigned long c, char **line, t_interface *interface)
{
	if (c == DEL && line && interface->line_index > 0)
	{
		ft_memmove((void*)(*line + interface->line_index - 1), (void*)(*line +
		interface->line_index), INPUT_LEN - interface->line_index - 1);
		(*line)[interface->line_len] = '\0';
		interface->line_index--;
		interface->line_len--;
		tputs(tgetstr("le", NULL), 1, ft_termprint);
		ft_printf("%s%s%s%s%s%s", tgetstr("sc", NULL), tgetstr("vi", NULL),
		tgetstr("cd", NULL), *line + interface->line_index,
		tgetstr("rc", NULL), tgetstr("ve", NULL));
	}
	else if (c == DEL2 && line && interface->line_index != interface->line_len)
	{
		ft_memmove((void*)(*line + interface->line_index), (void*)(*line +
		interface->line_index + 1), INPUT_LEN - interface->line_index - 1);
		(*line)[interface->line_len] = '\0';
		interface->line_len--;
		ft_printf("%s%s%s%s%s%s", tgetstr("sc", NULL), tgetstr("vi", NULL),
		tgetstr("cd", NULL), *line + interface->line_index,
		tgetstr("rc", NULL), tgetstr("ve", NULL));
	}
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
			delete(next, line, ui);
		else if (OK(status) && (next == '\n' || PRINTABLE_CHAR(next)))
			insert((char)next, line, ui);
	}
	return (ERR(restore_terminal(&ui->tty[1])) ? ERROR : status);
}
