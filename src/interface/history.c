/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/25 13:15:28 by dysotoma          #+#    #+#             */
/*   Updated: 2019/06/07 22:17:27 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	clear_all_lines(char *line, t_interface *ui)
{
	int		i;
	int		col;

	i = ui->line_index;
	tputs(tgetstr("vi", NULL), 1, ft_termprint);
	while (line_exists(line, i, PREV))
	{
		tputs(tgetstr("up", NULL), 1, ft_termprint);
		col = current_column(line, i);
		i -= col + 1;
		i = i < 0 ? 0 : i;
	}
	tputs(tgetstr("cr", NULL), 1, ft_termprint);
	tputs(tgetstr("cd", NULL), 1, ft_termprint);
	tputs(tgetstr("ve", NULL), 1, ft_termprint);
	write(STDOUT, "$> ", 3);
}

void	write_to_history(char *line[INPUT_LEN]
									, t_interface *interface)
{
	int	i;

	i = 0;
	if (**line && (!interface->h_list.hst
		|| ft_strcmp(*line, interface->curr_last_history) != 0))
	{
		write(interface->h_list.fd, ":", 1);
		while (i < interface->line_len)
		{
			write(interface->h_list.fd, *line + i, 1);
			if ((*line)[i] == '\\' || i + 1 == interface->line_len)
				write(interface->h_list.fd, "\n", 1);
			i++;
		}
	}
}

int		push_history(t_history **history, char *content)
{
	t_history	*new;

	if (!content || !content[0])
		return (NIL);
	new = init_h_node(content);
	if (!*history)
		*history = new;
	else
	{
		new->prev = *history;
		(*history)->next = new;
		*history = new;
	}
	return (SUCCESS);
}

void	print_history(t_interface *ui, char *line, char *next)
{
	size_t	len;

	len = 0;
	while (next && next[len])
	{
		if (next[len] == '\n' && !next[len + 1])
			break ;
		if (next[len] == '\n'
			&& !escaped(next, len) && len
			&& next[len - 1] != ';')
		{
			line[ui->line_index++] = ';';
			write(STDOUT, ";", 1);
		}
		write(STDOUT, &next[len], 1);
		if (next[len] == '\n' && next[len + 1])
			write(STDOUT, "> ", 2);
		line[ui->line_index++] = next[len++];
	}
}

void	history(unsigned long c
					, char *line
					, t_h_list *h_list
					, t_interface *ui)
{
	char	*next;

	if (!h_list->hst)
		return ;
	next = NULL;
	clear_all_lines(line, ui);
	ft_bzero(line, ui->line_len);
	if (c == UP && h_list->hst)
	{
		next = h_list->hst->content;
		if (h_list->hst->prev)
			h_list->hst = h_list->hst->prev;
	}
	else if (c == DOWN && h_list->hst)
	{
		if (h_list->hst->next)
			h_list->hst = h_list->hst->next;
		next = h_list->hst->content;
	}
	ui->line_index = 0;
	print_history(ui, line, next);
	ui->line_len = ui->line_index;
}
