/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/25 13:40:45 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_environ(int argc, char **argv, char **environ)
{
	int			i;
	int			total;

	i = 0;
	while (environ[i])
		i += 1;
	total = i + (argc > 1 ? argc - 1 : 0);
	g_environ = (char**)ft_memalloc(sizeof(char*) * (total + 1));
	i = -1;
	while (environ[++i])
		g_environ[i] = ft_strdup(environ[i]);
	i = 0;
	while (--argc)
		g_environ[(total ? total - 1 : total) + i++] = ft_strdup(argv[argc]);
}

int		main(int argc, char **argv, char **environ)
{
	char	*command;

	init_environ(argc, argv, environ);
	while (1)
	{
		signal(SIGINT, sig_handler);
		write(STDOUT, "$> ", 3);
		if (get_next_line(STDIN, &command) > 0)
		{
			if (execute_commands(command) == 0)
				break ;
			free(command);
		}
		else
			break ;
	}
	if (get_env_var("MINISHELL_LEAKS"))
		while (1)
			;
	return (0);
}
