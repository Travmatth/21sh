/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/15 18:09:34 by tmatthew         ###   ########.fr       */
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
** Initialize environment and parse data structures, Accept next line
** from STDIN and execute. Exit on ERROR, NIL will fail gracefully and accept
** next line of input
*/

int		main(int argc, char **argv, char **environ)
{
	char	*leaks;
	char	*input;
	int		status;
	int		end;
	
	if (ERR(init_parser())
		|| ERR(init_environ(argc, argv, environ))
		|| ERR(init_shell()))
		return (1);
	end = FALSE;
	leaks = get_env_var("SHELL_LEAKS");
	while (!end && !ERR(status))
	{
		input = NULL;
		status = signal(SIGINT, sig_handler) != SIG_ERR ? SUCCESS : ERROR;
		status = OK(status) && ERR(write(STDOUT, "$> ", 3)) ? ERROR : status;
		// if (OK(status) && OK(interface(&input)))
		if (OK(status) && OK(get_next_line(STDIN, &input)))
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
	free_prods();
	status = ERR(restore_shell()) || ERR(status) ? 1 : 0;
	if (leaks)
		hang_for_leaks();
	return (status);
}
