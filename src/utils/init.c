/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/03 14:32:20 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/06 13:09:43 by tmatthew         ###   ########.fr       */
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
	while (TRUE)
	{
		free(*line);
		if (ERR((status = get_next_line(fd, line))))
			return (NULL);
		else if (NONE(status))
			return (prods);
		prod = &prods[i++];
		if (!(prod_rule = ft_strsplit(*line, '#'))
			|| !(prod->lhs = ft_strdup(prod_rule[0]))
			|| (prod_rule[1] && !(prod->rhs = ft_strsplit(prod_rule[1], ' '))))
			return (NULL);
		ft_freearr(prod_rule, TRUE);
	}
}

/*
** Process existing environment variables of environment 21sh was launched in
** and store internally for use as internal environment, for use in builtins
** and launching child processes with execve. Increments $SHLVL, which tracks
** level of shell nesting.
*/

int		init_environ(int argc, char **argv, char **environ)
{
	int			i;
	int			n;
	char		*shlvl;

	i = 0;
	while (environ[i])
		i += 1;
	n = i + (argc > 1 ? argc - 1 : 0);
	if (!(g_environ = (char**)ft_memalloc(sizeof(char*) * (n + 1))))
		return (ERROR);
	i = -1;
	while (environ[++i])
		if (!(g_environ[i] = ft_strdup(environ[i])))
			return (ERROR);
	i = 0;
	while (--argc)
		if (!(g_environ[(n ? n - 1 : n) + i++] = ft_strdup(argv[argc])))
			return (ERROR);
	if (!(shlvl = get_env_var("SHLVL")))
		shlvl = "0";
	i = ft_atoi(shlvl);
	shlvl = ft_itoa(i + 1);
	set_env_var("SHLVL", shlvl);
	free(shlvl);
	return (SUCCESS);
}

/*
** If 21sh is launched in an environment with `SHELL_TTY` set to a valid ttyid
** the value of this environment variable is used as the tty that 21sh
** will write/read to/from. Initialize termcaps & clears screen, sets
** cursor position to the top of the screen
*/

int		init_shell(t_interface *ui)
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
	ui->ccp_start = ERROR;
	ui->ccp_end = ERROR;
	init_history(&ui->h_list);
	ioctl(STDERR_FILENO, TIOCGWINSZ, ui->ws);
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
		return (ERROR);
	size = ((size_t)ft_atoi(line) + 1) * sizeof(t_prod);
	if (!(productions = (t_prod*)ft_memalloc(size))
		|| !(g_prods = init_prods(fd, productions, &line)))
		return (ERROR);
	return (SUCCESS);
}

/*
** Clean shell state on exit
** free environment variables and interface struct, close open files descriptors
*/

int		restore_shell(t_interface *interface)
{
	if (ERR(tputs(tgetstr("te", NULL), 1, ft_termprint)))
		return (ERROR);
	if ((get_env_var("SHELL_TTY")))
	{
		if (ERR(close(STDIN))
			|| ERR(close(STDOUT))
			|| ERR(close(STDERR)))
			return (ERROR);
	}
	close(interface->h_list.fd);
	free_history(interface->h_list.hst);
	ft_freearr(g_environ, TRUE);
	return (SUCCESS);
}
