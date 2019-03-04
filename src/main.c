/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/23 20:06:46 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/02 20:14:18 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

// void	complete_command(t_ast *ast, t_ectx *ctx)
// {
// 	if (t->ast->form == 'complete_command : list separator')
// 	{
// 		list(ast->list, ctx);
// 		separator(ast->separator, ctx);
// 	}
// 	else if (t->ast->form == 'complete_command : list')
// 	{
// 		list(ast->list, ctx);
// 	}
// }

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

void	init_parser(void)
{
	int			i;
	int			j;
	int			fd;
	size_t		size;
	size_t		limit;
	char		*line;
	char		**tmp;
	t_handler	**handlers;

	if (ERR((fd = open("misc/parse_handlers.txt", O_RDONLY))))
		return ;
	line = NULL;
	get_next_line(fd, &line);
	size = ((size_t)ft_atoi(line) + 1) * sizeof(t_handler*);
	if (NONE((handlers = (t_handler**)ft_memalloc(size))))
		return ;
	i = 0;
	while (42)
	{
		j = 0;
		get_next_line(fd, &line);
		if (line[0] == '@')
		{
			tmp = ft_strsplit(line, ',');
			limit = ft_atoi(tmp[1]);
			size = (limit + 1) * sizeof(t_handler);
			if (NONE((handlers[i] = ft_memalloc(size))))
				return ;
		}
		else
		{
			while (j < limit)
			{
				get_next_line(fd, &line);
				tmp = ft_strsplit(line, '#');
				handlers[i][j].lhs = tmp[0];
				handlers[i][j].rhs = ft_strsplit(tmp[1], ',');
				handlers[i][j++].lookahed = ft_strsplit(tmp[2], ',');
			}
		}
	}
	g_handlers = handlers;
}

/*
** http://pubs.opengroup.org/onlinepubs/009695399
** /utilities/xcu_chap02.html#tag_02_03
** Shell grammar parsing steps
** 1: read from input
*/

int		main(int argc, char **argv, char **environ)
{
	char	*command;

	init_environ(argc, argv, environ);
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
