/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/25 13:15:28 by dysotoma          #+#    #+#             */
/*   Updated: 2019/06/08 22:09:07 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	clear_all_lines(char *line, t_interface *ui)
{
	int		i;
	int		col;

	i = ui->line_index;
	i = line[i] == '\n' ? i - 1 : i;
	tputs(tgetstr("vi", NULL), 1, ft_termprint);
	while (i > 0 && line_exists(line, i, PREV))
	{
		tputs(tgetstr("up", NULL), 1, ft_termprint);
		col = current_column(line, i);
		i -= col + 1;
		i = line[i] == '\n' ? i - 1 : i;
		i = i < 0 ? 0 : i;
	}
	tputs(tgetstr("cr", NULL), 1, ft_termprint);
	tputs(tgetstr("cd", NULL), 1, ft_termprint);
	tputs(tgetstr("ve", NULL), 1, ft_termprint);
	write(STDOUT, "$> ", 3);
}

void	write_to_history(char line[INPUT_LEN], t_interface *ui)
{
	int			i;
	t_history	*cur;

	i = 0;
	cur = ui->h_list.hst;
	while (cur && cur->next)
		cur = cur->next;
	if (line[i] && !ui->from_history && cur &&
		(!ui->h_list.hst || ft_strcmp(line, cur->content) != 0))
	{
		write(ui->h_list.fd, ":", 1);
		while (i < ui->line_len)
		{
			write(ui->h_list.fd, line + i, 1);
			if (line[i] == '\\' || i + 1 == ui->line_len)
				write(ui->h_list.fd, "\n", 1);
			i++;
		}
		ui->from_history = FALSE;
	}
}

int		push_history(t_history **history, char *content)
{
	t_history	*new;

	if (!content || !content[0])
		return (NIL);
	if (!(new = init_h_node(content)))
		return (ERROR);
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
	ui->from_history = next ? TRUE : FALSE;
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
	ui->line_index = next ? ui->line_index : 0;
}

void	history(unsigned long c, char *line, t_h_list *h_list, t_interface *ui)
{
	char		*next;
	t_history	*orig;

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
		orig = h_list->hst;
		if (h_list->hst && h_list->hst->next)
			h_list->hst = h_list->hst->next;
		if (h_list->hst && h_list->hst != orig)
			next = h_list->hst->content;
	}
	ui->line_index = 0;
	print_history(ui, line, next);
	ui->line_len = ui->line_index;
}
