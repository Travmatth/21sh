/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 21:10:05 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 21:43:01 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		builtin_echo(int argc, char **argv)
{
	int		i;
	int		skip;

	(void)argc;
	skip = 0;
	i = 1;
	if (argv[1] && ft_strequ("-n", argv[1]))
	{
		skip = 1;
		i = 2;
	}
	while (argv[i])
	{
		ft_putstr(argv[i++]);
		if (argv[i])
			write(STDOUT, " ", 1);
	}
	if (!skip)
		write(STDOUT, "\n", 1);
	return (NORMAL_CHILD_EXIT);
}
