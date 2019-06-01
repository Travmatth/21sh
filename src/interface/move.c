/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 19:31:47 by dysotoma          #+#    #+#             */
/*   Updated: 2019/05/31 17:46:15 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void		movement(unsigned long c, char **line, t_interface *interface)
{
	if (c == LEFT
		&& interface->line_index > 0
		&& tputs(tgetstr("le", NULL), 1, ft_termprint) == 0)
		interface->line_index--;
	else if (c == RIGHT
		&& (*line)[interface->line_index]
		&& tputs(tgetstr("nd", NULL), 1, ft_termprint) == 0)
		interface->line_index++;
	else if ((c == UP || c == DOWN))
		history(c, line, &interface->h_list, interface);
}
