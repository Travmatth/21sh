/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compound_quoting.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 15:10:24 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/25 15:39:02 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		init_exp(size_t *i, size_t start, int *found)
{
	*i = start;
	*found = FALSE;
	return (SUCCESS);
}

/*
** Process double quotes of form "", allows embedded arithmetic
** expansions, command substitution, backticks and parameter expansions.
** Calls specified callback with *end set to ERROR if expansion
** not properly closed
*/

int		dbl_quote(char **str, size_t start, size_t *end, t_quote f)
{
	size_t	i;
	int		s;
	int		status;
	int		found;

	found = FALSE;
	i = start;
	s = SUCCESS;
	while (OK(s) && (*str)[++i])
	{
		if ((P_BACKSLASH(s, str, f, i, end))
			|| (P_ARITH(s, str, f, i, end))
			|| (P_CMD(s, str, f, i, end))
			|| (P_BACKTICK(s, str, f, i, end))
			|| (P_PARAM(s, str, f, i, end)))
		{
			i += *end;
			*end = ERROR;
		}
		else if (OK(s) && (*str)[i] == '"')
			s = dbl_quote_switch(end, &found, i - start);
	}
	status = found ? SUCCESS : NIL;
	status = ERR(s) ? ERROR : status;
	return (f ? f(str, start, *end) : status);
}

/*
** Process parameter expansions of form ${} and $_, allows embedded arithmetic
** expansions, command substitution, backticks and parameter expansions, single
** and double quotes. Calls specified callback with *end set to ERROR if
** expansion not properly closed
*/

int		param_exp(char **str, size_t start, size_t *end, t_quote f)
{
	size_t	i;
	int		found;
	int		s;
	int		status;

	if ((s = init_exp(&i, start + 2, &found)) && (*str)[i + 1] != '{')
		return (f ? f(str, start, *exp_p(str, start, &found, end)) : SUCCESS);
	while (OK(s) && (*str)[i])
	{
		if ((P_BACKSLASH(s, str, f, i, end))
			|| (P_DQUOTE(s, str, f, i, end))
			|| (P_QUOTE(s, str, f, i, end))
			|| (P_ARITH(s, str, f, i, end))
			|| (P_CMD(s, str, f, i, end))
			|| (P_BACKTICK(s, str, f, i, end))
			|| (P_PARAM(s, str, f, i, end)))
		{
			i += *end;
			*end = ERROR;
		}
		else if (OK(s) && (*str)[i++] == '}' && param_exp_end(end, --i - start))
			break ;
	}
	status = ERR(s) ? ERROR : found;
	return (f ? f(str, start, *end) : status);
}

/*
** Process command substitution of form $(), allows embedded arithmetic
** expansions, command substitution, backticks and parameter expansions, single
** and double quotes. Calls specified callback with *end set to ERROR if
** expansion not properly closed
*/

int		command_sub(char **str, size_t start, size_t *end, t_quote f)
{
	size_t	i;
	int		s;
	int		status;
	int		found;

	s = init_exp(&i, start + 2, &found);
	while (OK(s) && (*str)[i])
	{
		if ((P_BACKSLASH(s, str, f, i, end))
			|| (P_DQUOTE(s, str, f, i, end))
			|| (P_QUOTE(s, str, f, i, end))
			|| (P_ARITH(s, str, f, i, end))
			|| (P_CMD(s, str, f, i, end))
			|| (P_BACKTICK(s, str, f, i, end))
			|| (P_PARAM(s, str, f, i, end)))
		{
			i += *end;
			*end = ERROR;
		}
		else if (OK(s) && (*str)[i] == ')')
			s = arith_exp_switch(end, i - start, &found);
		i += 1;
	}
	status = ERR(s) ? ERROR : found;
	return (f ? f(str, start, *end) : status);
}

/*
** Process arithmetic expansions of form $(()), allows embedded arithmetic
** expansions, command substitution, backticks and parameter expansions. Calls
** specified callback with *end set to ERROR if expansion not properly closed
*/

int		arith_exp(char **str, size_t start, size_t *end, t_quote f)
{
	size_t	i;
	int		s;
	size_t	count;
	int		status;
	int		found;

	count = 0;
	s = init_exp(&i, start, &found);
	while (OK(s) && (*str)[i])
	{
		if ((P_BACKSLASH(s, str, f, i, end))
			|| (P_ARITH(s, str, f, i, end))
			|| (P_CMD(s, str, f, i, end))
			|| (P_BACKTICK(s, str, f, i, end))
			|| (P_PARAM(s, str, f, i, end)))
		{
			i += *end;
			*end = ERROR;
		}
		else if (OK(s) && NONE((s = switch_arith_exp(str, i, &count, &found))))
			*end = i - start + 1;
		i += 1;
	}
	status = ERR(s) ? ERROR : found;
	return (f ? f(str, start, *end) : status);
}
