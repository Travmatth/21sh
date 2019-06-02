/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/01 16:09:21 by tmatthew         ###   ########.fr       */
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
	char		*input;
	int			status;
	int			end;
	t_interface	ui;

	if (ERR(init_parser()) || ERR(init_environ(argc, argv, environ))
		|| ERR(init_shell(&ui)))
		return (1);
	end = FALSE;
	while (!end && !ERR(status))
	{
		input = NULL;
		status = signal(SIGINT, sig_handler) != SIG_ERR ? SUCCESS : ERROR;
		status = OK(status) && ERR(write(STDOUT, "$> ", 3)) ? ERROR : status;
		if (OK(status) && OK(interface(&input, &ui)))
		{
			if (IS_A("exit", input))
				end = TRUE;
			else
				status = parse_execute_input(input);
			free(input);
		}
	}
	return (ERR(restore_shell(&ui)) || ERR(status) ? 1 : 0);
}
