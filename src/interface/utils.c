/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 12:50:52 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/09 10:20:54 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		accept_line(char **str)
{
	size_t	i;
	size_t	end;
	int		status;

	status = SUCCESS;
	i = 0;
	while (OK(status) && (*str)[i])
	{
		if ((P_BACKSLASH(status, str, NULL, i, (&end)))
			|| (P_QUOTE(status, str, NULL, i, (&end)))
			|| (P_DQUOTE(status, str, NULL, i, (&end)))
			|| (P_BACKTICK(status, str, NULL, i, (&end)))
			|| (P_ARITH(status, str, NULL, i, (&end)))
			|| (P_CMD(status, str, NULL, i, (&end)))
			|| (P_EPARAM(status, str, NULL, i, (&end))))
			i += end;
		i += 1;
	}
	return (status);
}

int		init_accept(char **line, t_interface *ui, char *tmp, size_t *i)
{
	*i = 0;
	set_cursor(ui, tmp, ui->line_len);
	write(STDOUT, "\n", 1);
	write_to_history(tmp, ui);
	if (ERR(push_history(&ui->h_list.hst, tmp))
		|| !(*line = ft_strnew(ui->line_len)))
		return (ERROR);
	return (SUCCESS);
}
