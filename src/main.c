/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/25 16:39:47 by tmatthew         ###   ########.fr       */
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
	char	*input;
	int		status;
	
	if (ERR(init_parser())
		|| ERR(init_environ(argc, argv, environ))
		|| ERR(init_shell()))
		return (1);
	status = SUCCESS;
	while (!ERR(status))
	{
		input = NULL;
		if ((signal(SIGINT, sig_handler) != SIG_ERR)
			&& !ERR(write(STDOUT, "$> ", 3))
			&& (OK(get_next_line(STDIN, &input))))
		{
			status = IS_A("exit", input)
				? ERROR 
				: parse_execute_input(input);
			free(input);
		}
	}
	return (0);
}
