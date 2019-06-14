/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/13 15:53:58 by tmatthew         ###   ########.fr       */
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

int		event_loop(t_interface *ui, char **buf)
{
	char		*input;
	int			status;
	int			end;

	status = SUCCESS;
	while (!ERR(status))
	{
		input = NULL;
		end = FALSE;
		signal(SIGINT, sig_handler);
		print_newline();
		write(STDOUT, "$> ", 3);
		if (NONE((status = interface(&input, buf, ui, &end))) && !end)
		{
			end = !ft_strncmp("exit", input, 4) ? TRUE : FALSE;
			status = end ? ERROR : parse_execute_input(input);
			free(input);
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
	char		tmp[INPUT_LEN];
	char		*buf;
	t_interface	ui;

	if (ERR(init_parser())
		|| ERR(init_environ(argc, argv, environ))
		|| ERR(init_shell(&ui)))
		return (ERROR_CHILD_EXIT);
	leaks = get_env_var("SHELL_LEAKS");
	buf = (char*)tmp;
	status = event_loop(&ui, &buf);
	free_prods();
	status = ERR(restore_shell(&ui)) || ERR(status) ? 1 : 0;
	if (leaks)
		hang_for_leaks();
	return (status);
}
