/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 17:01:40 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 17:51:46 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	copy(t_interface *ui, char *buf, char *line)
{
	int		i;

	if (!ui->select)
		return ;
	ui->select = FALSE;
	write(STDOUT, STEADY_CURSOR, 5);
	if (ERR(ui->ccp_start) || ERR(ui->ccp_end))
		return ;
	ft_strclr(buf);
	i = ui->ccp_end - ui->ccp_start;
	ft_memcpy(buf, &(line[ui->ccp_start]), i);
	i = ui->ccp_start;
	clear_all_lines(ui);
	init_select(ui);
	write_line(ui, line);
	ui->line_index = 0;
	set_cursor(ui, i);
}

void	copy_line(t_interface *ui, char *buf, char *line)
{
	t_uiline	*cur;
	int			len;

	if (ui->select || !(cur = current_uiline(ui, ui->line_index)))
	{
		write(STDOUT, "\a", 1);
		return ;
	}
	ft_strclr(buf);
	set_cursor(ui, 0);
	len = cur->end - cur->start;
	len += line[cur->end] == '\n' ? 1 : 0;
	ft_memcpy(buf, &(line[cur->start]), len);
}
