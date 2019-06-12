/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_line.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/09 18:14:34 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 18:03:56 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

t_uiline	*create_uiline(void)
{
	t_uiline	*ui_line;

	if (!(ui_line = ft_memalloc(sizeof(t_uiline))))
		return (NULL);
	ft_bzero(ui_line, sizeof(t_uiline));
	ui_line->start = ERROR;
	ui_line->end = ERROR;
	return (ui_line);
}

int			init_uiline(t_interface *ui)
{
	t_uiline	*ui_line;

	if (!(ui_line = create_uiline()))
		return (ERROR);
	ui_line->start = 0;
	ui->ui_line = ui_line;
	return (SUCCESS);
}

int			add_uiline(t_interface *ui)
{
	t_uiline	*cur;
	t_uiline	*ui_line;

	if (!(ui_line = create_uiline()))
		return (ERROR);
	ui_line->start = ui->line_index;
	cur = ui->ui_line;
	while (cur && cur->next)
		cur = cur->next;
	cur->end = ui->line_index - 1;
	cur->next = ui_line;
	ui_line->prev = cur;
	return (SUCCESS);
}

int			crawl_uilines(char *buf, t_uiline *ui_line, int i)
{
	ui_line->start = i;
	while (buf[i])
	{
		if (buf[i] == '\n')
			break ;
		i += 1;
	}
	ui_line->end = i;
	if (buf[i] == '\n')
	{
		if (!ui_line->next && !(ui_line->next = create_uiline()))
			return (ERROR);
		ui_line->next->prev = ui_line;
		return (crawl_uilines(buf, ui_line->next, i + 1));
	}
	free_uiline(&ui_line->next);
	return (SUCCESS);
}

int			calculate_uilines(char *buf, t_interface *ui)
{
	t_uiline	*cur;

	if (!ui->ui_line && ERR(init_uiline(ui)))
		return (ERROR);
	cur = ui->ui_line;
	while (cur)
	{
		cur->start = ERROR;
		cur->end = ERROR;
		cur = cur->next;
	}
	cur = ui->ui_line;
	return (crawl_uilines(buf, cur, 0));
}
