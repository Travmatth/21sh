/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/03 14:32:20 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/03 18:02:33 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

int			apportion_state(t_handle **handlers, size_t *limit, char *line)
{
	char	**tmp;
	size_t	size;

	if (!(tmp = ft_strsplit(line, ',')))
		return (ERROR);
	*limit = ft_atoi(tmp[1]);
	size = (*limit + 1) * sizeof(t_handle);
	if (!(*handlers = (t_handle*)ft_memalloc(size)))
		return (ERROR);
	return (SUCCESS);
}

int			process_handle(t_handle *handle, char *line)
{
	char		**handle_rule;

	if (!(handle_rule = ft_strsplit(line, '#')))
		return (ERROR);
	handle->lhs = handle_rule[0];
	if (!(handle->rhs = ft_strsplit(handle_rule[1], ','))
		|| !(handle->lookahed = ft_strsplit(handle_rule[2], ',')))
		return (ERROR);
	return (SUCCESS);
}

t_handle	**init_handles(int fd, t_handle **handlers, char **line)
{
	int			status;
	size_t		i;
	size_t		j;
	size_t		limit;

	i = 0;
	while (42)
	{
		j = 0;
		if (ERR((status = get_next_line(fd, line))))
			return (NULL);
		else if (NONE(status))
			return (handlers);
		if ((*line)[0] == '@'
			&& ERR(apportion_state(&handlers[i], &limit, *line)))
			return (NULL);
		else if ((*line)[0] != 'a')
		{
			while (j < limit)
			{
				free(*line);
				get_next_line(fd, line);
				if (ERR(process_handle(&handlers[i][j++], *line)))
					return (NULL);
			}
		}
	}
}

void	init_environ(int argc, char **argv, char **environ)
{
	int			i;
	int			total;

	i = 0;
	while (environ[i])
		i += 1;
	total = i + (argc > 1 ? argc - 1 : 0);
	if (!(g_environ = (char**)ft_memalloc(sizeof(char*) * (total + 1))))
		return ;
	i = -1;
	while (environ[++i])
		if (!(g_environ[i] = ft_strdup(environ[i])))
			return ;
	i = 0;
	while (--argc)
		if (!(g_environ[(total ? total - 1 : total) + i++] =
			ft_strdup(argv[argc])))
			return ;
}

void	init_parser(void)
{
	int			fd;
	size_t		size;
	char		*line;
	t_handle 	**handlers;

	if (ERR((fd = open("misc/parse_handlers.txt", O_RDONLY))))
		return ;
	line = NULL;
	get_next_line(fd, &line);
	size = ((size_t)ft_atoi(line) + 1) * sizeof(t_handle*);
	if (NONE((handlers = (t_handle**)ft_memalloc(size))))
		return ;
	g_handlers = init_handles(fd, handlers, &line);
}
