/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 12:50:52 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/08 20:57:08 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		accept_line(char *line, int i)
{
	int is_accept;

	is_accept = TRUE;
	while (i > 0)
	{
		if (line[i] != '\\')
			break ;
		is_accept = !is_accept;
		i -= 1;
	}
	return (is_accept);
}
