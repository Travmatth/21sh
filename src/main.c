/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/22 15:45:33 by tmatthew         ###   ########.fr       */
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

int		event_loop(char *leaks)
{
	int		end;
	int		status;
	char	*input;

	end = FALSE;
	while (!end && !ERR(status))
	{
		input = NULL;
		status = signal(SIGINT, sig_handler) != SIG_ERR ? SUCCESS : ERROR;
		status = OK(status) && ERR(write(STDOUT, "$> ", 3)) ? ERROR : status;
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
	if (leaks)
		hang_for_leaks();
	return (status);
}

/*
** Initialize environment and parse data structures
*/

int		main(int argc, char **argv, char **environ)
{
	int		status;

	if (ERR(init_parser())
		|| ERR(init_environ(argc, argv, environ))
		|| ERR(init_shell()))
		return (1);
	free_prods();
	status = event_loop(get_env_var("SHELL_LEAKS"));
	status = ERR(restore_shell()) || ERR(status) ? 1 : 0;
	return (status);
}
