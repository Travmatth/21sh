/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 19:23:40 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/30 18:41:27 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

t_builtin	g_builtins[] =
{
	{"echo", builtin_echo, 4},
	{"cd", builtin_cd, 2},
	{"setenv", builtin_setenv, 6},
	{"unsetenv", builtin_unsetenv, 8},
	{"env", builtin_env, 3},
};

int		remove_slash(char elem, size_t i, char *str, int *stop)
{
	int		out;

	(void)stop;
	out = 1;
	if (elem == '\\')
		out = IS_SEP(str[i + 1]) ? 0 : 1;
	return (out);
}

char	**tokenize_commands(char **commands)
{
}

void	expand_command(char **command)
{
	char	*var;
	char	*tmp;
	char	*cmd;

	while (ft_strchr(*command, '~'))
	{
		var = get_env_var("HOME");
		tmp = ft_swap(*command, "~", var);
		free(*command);
		*command = tmp;
	}
	while ((tmp = ft_strchr(*command, '$')))
	{
		cmd = ft_strchr(tmp, ' ') ? ft_strchr(tmp, ' ') : ft_strchr(tmp, '\0');
		tmp = ft_strsub(tmp, 0, cmd - tmp);
		var = get_env_var(tmp + 1);
		cmd = ft_swap(*command, tmp, var ? var : "");
		free(*command);
		free(tmp);
		*command = cmd;
	}
}

/*
** Shell grammar parsing steps
** 1: read from input
*/

/*
** Shell grammar parsing steps
** 2: break input into tokens: words and operators
** 3: parse input into simple commands and compound commands
** 4: perform expansions
** 5: register redirections and removes operators from parameter list
*/

/*
** manage escaped characters in given arg, turn \n -> <newline> and strip other \
*/

void	normalize_args(char **args)
{
}

/*
** if open quote is detected, prompt user for more input that will close the quote
*/

int		close_quote_prompt(char **commands)
{
}

/*
** find next whitespace in commands string not preceded by a backslash
*/

int		find_ws(char *commands, size_t *offset)
{
	while (commands && commands[i])
	{
		if (IS_WS(*commands))
		{
			*offset += i;
			return (SUCCESS);
		}
		commands += 1;
	}
	return (ERROR);
}

/*
** find next character c in commands string not preceded by a backslash
*/

int		find_next(char c, char *commands, size_t *offset)
{
	size_t	i;

	i = 1;
	while (commands && commands[i])
	{
		if (*commands == c && i == 1)
		{
			*offset += i;
			return (SUCCESS);
		}
		else if (*commands == c && commands[i - 1] == '\\')
		{
			if (ERR(find_next(c, &commands[i], offset))
				return (ERROR);
			*offset += i;
			return (SUCCESS);
		}
		commands += 1;
	}
	return (ERROR);
}

/*
** determine whether character c in commands string is quote not preceded by a backslash
*/

int		is_quote(char *commands, size_t n)
{
	if (n == 0 && IS_WS(*commands))
		return (SUCCESS);
	else if (n > 0 && *(commands - 1) != '\\' && IS_WS(*commands))
		return (SUCCESS);
	return (NIL);
}

/*
** transform a given command string into an array of tokens to be parsed
*/

int		tokenize(char *commands, int arg_count, size_t len, char ***args)
{
	size_t	offset;
	int		s;
	int		i;
	size_t	n;

	i = -1;
	offset = 0;
	if (!commands)
		return (ERROR);
	while (commands[n] && ++i < arg_count)
	{
		while (IS_WS(commands[n]))
			commands += 1;
		if (!commands[1])
			break ;
		else if (!is_quote(&commands[n], i) && (s = find_ws(commands, &offset)))
			continue ;
		else if (commands[n] == '\'' && (s = find_next('\'', &commands[n], &offset)))
			continue ;
		else if (commands[n] == '"' && (s = find_next('"', &commands[n], &offset)))
			continue ;
		if (ERR(s) && ERR(close_quote_prompt(commands[n])))
			return (ERROR);
		*args[i] = ft_strsub(commands, n, offset);
		n += offset;
	}
	return (SUCCESS);
}

int		prepare_job(char *commands, t_job *job, int i)
{
	int		arg_count;
	char	**cmd_args;
	char	**args;

	arg_count = count_params(commands);
	if (!OK(arg_count))
		return (ERR(arg_count) ? ERROR : NIL);
	args = NULL;
	if (!(args = (char**)ft_memalloc(sizeof(char) * (arg_count + 1)))
		return (NIL);
	else if (ERR(tokenize(commands, arg_count, LEN(commands, 0), &args)))
		return (NIL);
	else if (ERR(normalize_args(args)))
		return (NIL);
}

int		builtin_command(char **argv)
{
	int		i;
	int		ac;

	i = 0;
	while (i < 5)
	{
		if (ft_strnequ(g_builtins[i].cmd, argv[0], g_builtins[i].len))
		{
			ac = 0;
			while (argv[ac])
				ac += 1;
			i = g_builtins[i].f(ac, argv);
			ac -= 1;
			return (i);
		}
		i += 1;
	}
	return (0);
}
