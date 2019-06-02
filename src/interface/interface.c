/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interface.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/06 15:42:31 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/01 19:07:28 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void	insert(char c, char **line, t_interface *interface)
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
		write(1, &c, 1);
}

static void	delete(unsigned long c, char **line, t_interface *interface)
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

void		history(unsigned long c
					, char **line
					, t_h_list *h_list
					, t_interface *interface)
{
	if (h_list->hst)
	{
		if (c == UP && h_list->hst)
		{
			*line = h_list->hst->content;
			if (h_list->hst->prev)
				h_list->hst = h_list->hst->prev;
		}
		else if (c == DOWN && h_list->hst)
		{
			if (h_list->hst->next)
				h_list->hst = h_list->hst->next;
			*line = h_list->hst->next ? h_list->hst->content : h_list->old;
		}
		tputs(tgetstr("vi", NULL), 1, ft_termprint);
		while (interface->line_index > 0)
			movement(LEFT, line, interface);
		tputs(tgetstr("cd", NULL), 1, ft_termprint);
		ft_printf("%s", *line);
		tputs(tgetstr("ve", NULL), 1, ft_termprint);
		interface->line_len = ft_strlen(*line);
		interface->line_index = interface->line_len;
	}
}

/*
** Read single character from STDIN. Break reading on CTRL-C and delete input
** on BACKSPACE, otherwise append character to line and echo to STDOUT
*/

int		interface(char **line, t_interface *ui)
{
	static char			tmp[INPUT_LEN];
	unsigned long		next;
	int					status;
	size_t				len;
	int					in_word;

	len = 0;
	ft_bzero(tmp, INPUT_LEN);
	*line = tmp;
	status = prep_terminal(ui->tty, ~(ICANON | ISIG | ECHO), 1, 0);
	in_word = FALSE;
	ui->line_index = 0;
	ui->line_len = 0;
	while (OK(status))
	{
		next = 0;
		read(STDIN, &next, 6);
		movement(next, line, ui);
		if (next == INTR || next == RETURN)
		{
			status = write(STDOUT, "\n", 1);
			write_to_history(line, ui);
			status = OK(status) && push_history(&ui->h_list.hst, *line);
			*line = ft_strdup(tmp);
			status = !*line ? ERROR : status;
			break ;
		}
		else if (next == DEL || next == DEL2)
			delete(next, line, ui);
		if (PRINTABLE_CHAR(next))
			insert((char)next, line, ui);
	}
	return (ERR(restore_terminal(&ui->tty[1])) ? ERROR : status);
}
