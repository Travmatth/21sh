/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/03 14:32:20 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/07 16:33:48 by tmatthew         ###   ########.fr       */
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

int		init_environ(int argc, char **argv, char **environ)
{
	int			i;
	int			total;

	i = 0;
	while (environ[i])
		i += 1;
	total = i + (argc > 1 ? argc - 1 : 0);
	if (!(g_environ = (char**)ft_memalloc(sizeof(char*) * (total + 1))))
		return ERROR;
	i = -1;
	while (environ[++i])
		if (!(g_environ[i] = ft_strdup(environ[i])))
			return ERROR;
	i = 0;
	while (--argc)
		if (!(g_environ[(total ? total - 1 : total) + i++] =
			ft_strdup(argv[argc])))
			return ERROR;
	return SUCCESS;
}

/*
** If 21sh is launched in an environment with `SHELL_TTY` set to a valid ttyid
** the value of this environment variable is used as the tty that 21sh
** will write/read to/from. Initialize termcaps & clears screen, sets
** cursor position to the top of the screen
*/

int		init_shell(void)
{
	int		fd;
	char	*tty_id;
	char	buf[32];
	char	*type;

	if ((tty_id = get_env_var("SHELL_TTY")))
	{
		if (ERR((fd = open(tty_id, O_RDWR)))
			|| ERR(dup2(fd, STDIN))
			|| ERR(dup2(fd, STDOUT))
			|| ERR(dup2(fd, STDERR))
			|| ERR(close(fd)))
			return (ERROR);
	}
	if (NONE((type = get_env_var("TERM")))
		|| ERR(tgetent(buf, type))
		|| ERR(tputs(tgetstr("ti", NULL), 1, ft_termprint))
		|| ERR(tputs(tgoto(tgetstr("cm", NULL), 0, 0), 1, ft_termprint))
		|| ERR(tputs(tgetstr("cd", NULL), 1, ft_termprint)))
		return (ERROR);
	return (SUCCESS);
}

/*
** Initialize variables needed by syntactic parser by opening production rules
** and setting global g_prods to processed grammar production rules
*/

int		init_parser(void)
{
	int		fd;
	size_t	size;
	char	*line;
	t_prod	*productions;

	line = NULL;
	if (ERR((fd = open("misc/productions.txt", O_RDONLY)))
		|| !OK(get_next_line(fd, &line)))
		return ERROR;
	size = ((size_t)ft_atoi(line) + 1) * sizeof(t_prod);
	if (!(productions = (t_prod*)ft_memalloc(size))
		|| !(g_prods = init_prods(fd, productions, &line)))
		return ERROR;
	return SUCCESS;
}

int		restore_shell(void)
{
	if (ERR(tputs(tgetstr("te", NULL), 1, ft_termprint)))
		return (ERROR);
	return (SUCCESS);
}
