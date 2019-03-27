/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/03 14:32:20 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/26 18:27:31 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

t_prod	*init_prods(int fd, t_prod *prods, char **line)
{
	int			status;
	size_t		i;
	t_prod		*prod;
	char		**prod_rule;

	i = 0;
	while (42)
	{
		free(*line);
		if (ERR((status = get_next_line(fd, line))))
			return (NULL);
		else if (NONE(status))
			return (prods);
		prod = &prods[i++];
		if (!(prod_rule = ft_strsplit(*line, '#')))
			return (NULL);
		prod->lhs = prod_rule[0];
		if (prod_rule[1] && !(prod->rhs = ft_strsplit(prod_rule[1], ' ')))
			return (NULL);
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

int		init_shell(void)
{
	int		fd;
	char	*tty_id;

	if (!(tty_id = get_env_var("SHELL_TTY")))
		return (NIL);
	else if (ERR((fd = open(tty_id, O_RDWR)))
		|| ERR(dup2(fd, STDIN))
		|| ERR(dup2(fd, STDOUT))
		|| ERR(dup2(fd, STDERR))
		|| ERR(close(fd)))
		return (ERROR);
	return (SUCCESS);
}

void	init_parser(void)
{
	int		fd;
	size_t	size;
	char	*line;
	t_prod	*productions;

	if (ERR((fd = open("misc/productions.txt", O_RDONLY))))
		return ;
	line = NULL;
	get_next_line(fd, &line);
	size = ((size_t)ft_atoi(line) + 1) * sizeof(t_prod);
	if (NONE((productions = (t_prod*)ft_memalloc(size))))
		return ;
	g_prods = init_prods(fd, productions, &line);
}
