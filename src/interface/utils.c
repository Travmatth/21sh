/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/08 12:50:52 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 16:55:40 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void		free_uiline(t_uiline **ui_line)
{
	if (*ui_line)
	{
		free_uiline(&(*ui_line)->next);
		(*ui_line)->prev = NULL;
		(*ui_line)->next = NULL;
		free(*ui_line);
		*ui_line = NULL;
	}
}
