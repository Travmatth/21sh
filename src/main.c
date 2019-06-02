/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/02 13:51:28 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

/*
** If env variable SHELL_LEAKS is defined, hang to allow leak checking until
** character 'x' pressed followed by return
*/

void	hang_for_leaks(void)
{
	char	c;

	c = 0;
	while (read(STDIN, &c, 1))
	{
		if (c == 'x')
			break ;
	}
}

/*
** Accept next line from STDIN and execute.
** Exit on ERROR, NIL will fail gracefully and accept next line of input
*/

int		event_loop(t_interface *ui, char *leaks)
{
	char		*input;
	int			status;
	int			end;

	end = FALSE;
	while (!end && !ERR(status))
	{
		input = NULL;
		status = signal(SIGINT, sig_handler) != SIG_ERR ? SUCCESS : ERROR;
		status = OK(status) && ERR(write(STDOUT, "$> ", 3)) ? ERROR : status;
		if (OK(status) && OK(interface(&input, ui)))
		{
			if (IS_A("exit", input))
				end = TRUE;
			else
				status = parse_execute_input(input);
			free(input);
			if (leaks)
				break ;
		}
	}
	return (status);
}

/*
** Initialize environment and parse data structures
*/

int		main(int argc, char **argv, char **environ)
{
	int			status;
	char		*leaks;
	t_interface	ui;

	if (ERR(init_parser())
		|| ERR(init_environ(argc, argv, environ))
		|| ERR(init_shell(&ui)))
		return (1);
	leaks = get_env_var("SHELL_LEAKS");
	status = event_loop(&ui, leaks);
	free_prods();
	status = ERR(restore_shell(&ui)) || ERR(status) ? 1 : 0;
	if (leaks)
		hang_for_leaks();
	return (status);
}
