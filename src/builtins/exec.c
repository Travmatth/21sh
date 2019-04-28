/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 17:57:05 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/27 18:43:56 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		builtin_exec(int argc, char **argv)
{
	if (argc > 1)
	{
		ft_printf("here\n");
		execve(argv[1], &argv[1], g_environ);
		ft_printf("there\n");
		return (ERROR);
	}
	return (SUCCESS);
}
