/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/23 14:30:34 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
** Initialize environment and parse data structures, Accept next line
** from STDIN and execute. Exit on ERROR, NIL will fail gracefully and accept
** next line of input
*/

int		main(int argc, char **argv, char **environ)
{
	char	*command;

	init_parser();
	init_environ(argc, argv, environ);
	if (!g_prods || !g_environ || ERR(init_shell()))
		return (1);
	while (TRUE)
	{
		signal(SIGINT, sig_handler);
		write(STDOUT, "$> ", 3);
		if (OK(get_next_line(STDIN, &command))
			&& !IS_A("exit", command)
			&& !ERR(parse_execute_input(command)))
			free(command);
		else
			break ;
	}
	return (0);
}
