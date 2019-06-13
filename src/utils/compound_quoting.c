/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compound_quoting.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 15:10:24 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/13 15:47:41 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

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

	s = init_exp(&i, start, &found);
	while (OK(s) && (*str)[++i])
	{
		if ((P_BACKSLASH(s, str, f, i, end))
			|| (P_ARITH(s, str, f, i, end))
			|| (P_CMD(s, str, f, i, end))
			|| (P_BACKTICK(s, str, f, i, end))
			|| (P_UPARAM(s, str, f, i, end))
			|| (P_EPARAM(s, str, f, i, end)))
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

int		uparam_exp(char **str, size_t start, size_t *end, t_quote f)
{
	size_t	i;

	i = start;
	while (IS_VAR_CHAR((*str)[i + 1]))
		i += 1;
	*end = i - start;
	return (f ? f(str, start, *end) : SUCCESS);
}

/*
** Process parameter expansions of form ${} and $_, allows embedded arithmetic
** expansions, command substitution, backticks and parameter expansions, single
** and double quotes. Calls specified callback with *end set to ERROR if
** expansion not properly closed
*/

int		eparam_exp(char **str, size_t start, size_t *end, t_quote f)
{
	size_t	i;
	int		found;
	int		s;
	int		status;

	s = init_exp(&i, start + 2, &found);
	while (OK(s) && (*str)[i])
	{
		if ((P_BACKSLASH(s, str, f, i, end))
			|| (P_DQUOTE(s, str, f, i, end))
			|| (P_QUOTE(s, str, f, i, end))
			|| (P_ARITH(s, str, f, i, end))
			|| (P_CMD(s, str, f, i, end))
			|| (P_BACKTICK(s, str, f, i, end))
			|| (P_UPARAM(s, str, f, i, end))
			|| (P_EPARAM(s, str, f, i, end)))
		{
			i += *end;
			*end = ERROR;
		}
		else if (OK(s) && END_P(str, i) && param_end(end, --i - start, &found))
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
			|| (P_UPARAM(s, str, f, i, end))
			|| (P_EPARAM(s, str, f, i, end)))
		{
			i += *end + 1;
			*end = ERROR;
		}
		else if (OK(s) && (*str)[i++] == ')')
			s = arith_exp_switch(end, i - start - 1, &found);
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
	s = init_exp(&i, start + 3, &found);
	while (OK(s) && (*str)[i] && !found)
	{
		if ((P_BACKSLASH(s, str, f, i, end))
			|| (P_ARITH(s, str, f, i, end))
			|| (P_CMD(s, str, f, i, end))
			|| (P_BACKTICK(s, str, f, i, end))
			|| (P_UPARAM(s, str, f, i, end))
			|| (P_EPARAM(s, str, f, i, end)))
		{
			i += *end;
			*end = ERROR;
		}
		else if (switch_arith_exp(str, i++, &count, &found))
			*end = i - start;
	}
	status = ERR(s) ? ERROR : found;
	return (f ? f(str, start, *end) : status);
}
