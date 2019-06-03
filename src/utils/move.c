/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dysotoma <dysotoma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/27 19:31:47 by dysotoma          #+#    #+#             */
/*   Updated: 2019/06/02 01:51:30 by dysotoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

// static void	move_left()
// {
	
// }

// static void	move_right()
// {
	
// }

// static void	move_up()
// {
	
// }

// static void	move_down()
// {

// }

void		movement(unsigned long c, char **line, t_interface *interface)
{
	
		// ft_printf("line_index = %i | line_len = %i", interface->line_index, interface->line_len);
	if (c == LEFT && interface->line_index > 0 && 
			tputs(tgetstr("le", NULL), 1, ft_termprint) == 0)
		interface->line_index--;
	else if (c == RIGHT && (*line)[interface->line_index] && 
			tputs(tgetstr("nd", NULL), 1, ft_termprint) == 0)
		interface->line_index++;	
	// // if moving from start line add length of prompt
	// else if (c == CTL_UP)
	// // if moving to start line subtract length of prompt
	// else if (c == CTL_DOWN)
	else if (interface->written == 0 && interface->h_list.hst
	&& (c == UP || (c == DOWN && interface->h_list.hst->next)))
		history(c, line, &interface->h_list, interface);
}
