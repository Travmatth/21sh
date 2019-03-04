/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/03 18:02:28 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
	** http://pubs.opengroup.org/onlinepubs/009695399
** /utilities/xcu_chap02.html#tag_02_03
** Shell grammar parsing steps
** 1: read from input
*/

int		main(int argc, char **argv, char **environ)
{
	char	*command;

	init_parser();
	init_environ(argc, argv, environ);
	if (!g_handlers || !g_environ)
		return (0);
	while (42)
	{
		signal(SIGINT, sig_handler);
		write(STDOUT, "$> ", 3);
		if (get_next_line(STDIN, &command) > 0)
		{
			if (ERR(execute_commands(command)))
				break ;
			free(command);
		}
		else
			break ;
	}
	return (0);
}
