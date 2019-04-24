/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/03 14:32:20 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/23 15:21:33 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Given the file descriptor of the productions.txt file, process lines of
** productions by creating a struct containing left hand side of
** nonterminal symbols and an array containing the right hand side each
** nonterminal is derived by the given production rule
*/

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

/*
** Process existing environment variables of environment 21sh was launched in
** and store internally for use as internal environment, for use in builtins
** and launching child processes with execve
*/

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

/*
** If 21sh is launched in an environment with `SHELL_TTY` set to a valid ttyid
** the value of this environment variable is used as the tty that 21sh
** will write/read to/from
*/

int		init_shell(void)
{
	int		fd;
	char	*tty_id;

	if ((tty_id = get_env_var("SHELL_TTY")))
	{
		if (ERR((fd = open(tty_id, O_RDWR)))
			|| ERR(dup2(fd, STDIN))
			|| ERR(dup2(fd, STDOUT))
			|| ERR(dup2(fd, STDERR))
			|| ERR(close(fd)))
			return (ERROR);
	}
	return (SUCCESS);
}

/*
** Initialize variables needed by syntactic parser by opening production rules
** and setting global g_prods to processed grammar production rules
*/

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
