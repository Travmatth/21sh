/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/25 13:15:28 by dysotoma          #+#    #+#             */
/*   Updated: 2019/06/13 17:47:54 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	clear_all_lines(t_interface *ui)
{
	t_uiline	*ui_line;

	ui_line = current_uiline(ui, ui->line_index);
	while (ui_line && ui_line->prev)
	{
		tputs(tgetstr("up", NULL), 1, ft_termprint);
		ui_line = ui_line->prev;
	}
	tputs(tgetstr("cr", NULL), 1, ft_termprint);
	tputs(tgetstr("cd", NULL), 1, ft_termprint);
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
	if (line[i] && (!cur || (ft_strcmp(line, cur->content) != 0)))
	{
		write(ui->h_list.fd, ":", 1);
		while (line[i])
			write(ui->h_list.fd, line + i++, 1);
		write(ui->h_list.fd, "\n", 1);
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
	while (next && next[len])
	{
		if (next[len] == '\n' && !next[len + 1])
			break ;
		write(STDOUT, &next[len], 1);
		if (next[len] == '\n' && next[len + 1])
			write(STDOUT, "> ", 2);
		line[ui->line_index++] = next[len++];
	}
	if (next)
		calculate_uilines(next, ui);
	else
		free_uiline(&ui->ui_line);
	ui->line_index = next ? ui->line_index : 0;
}

void	history(unsigned long c, char *line, t_h_list *h_list, t_interface *ui)
{
	char		*next;
	t_history	*orig;

	if (!h_list->hst)
		return ;
	next = NULL;
	clear_all_lines(ui);
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
