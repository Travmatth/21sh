/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 12:59:41 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/16 16:13:11 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Process backslash escapes of form \, calls specified callback with *end set to
** ERROR if expansion not properly closed
*/

int		backslash(char **str, size_t start, size_t *end, t_quote f)
{
	size_t	i;
	int		status;
	char	*string;
	int		found;

	found = FALSE;
	string = *str;
	i = start;
	*end = ERROR;
	if (string[++i])
	{
		found = TRUE;
		*end = i - start;
	}
	status = found ? SUCCESS : NIL;
	return (f ? f(str, start, *end) : status);
}

/*
** Process single quotes of form '', calls specified callback with *end set to
** ERROR if expansion not properly closed
*/

int		quote(char **str, size_t start, size_t *end, t_quote f)
{
	size_t	i;
	int		status;
	char	*string;
	int		found;

	found = FALSE;
	string = *str;
	i = start;
	*end = ERROR;
	while (string[++i])
	{
		if (string[i] == '\'')
		{
			found = TRUE;
			*end = i - start;
			break ;
		}
	}
	status = found ? SUCCESS : NIL;
	return (f ? f(str, start, *end) : status);
}

/*
** Process backticks ``, calls specified callback with *end set to ERROR if
** expansion not properly closed
*/

int		backtick(char **str, size_t start, size_t *end, t_quote f)
{
	size_t	i;
	int		status;
	char	*string;
	int		found;

	found = FALSE;
	string = *str;
	i = start;
	*end = ERROR;
	while (string[++i])
	{
		if (string[i] == '`')
		{
			found = TRUE;
			*end = i - start;
			break ;
		}
	}
	status = found ? SUCCESS : NIL;
	return (f ? f(str, start, *end) : status);
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
		if (
			(BACKSLASH((*str), i) && !ERR((s = backslash(str, i, end, f))))
			|| (ARITH_EXP((*str), i) && !ERR((s = arith_exp(str, i, end, f))))
			|| (CMD_SUB((*str), i) && !ERR((s = command_sub(str, i, end, f))))
			|| (BACKTICK((*str), i) && !ERR((s = backtick(str, i, end, f))))
			|| (PARAM_EXP((*str), i) && !ERR((s = param_exp(str, i, end, f))))
		)
		{
			i += *end;
			*end = ERROR;
		}
		else if (OK(s) && (*str)[i] == '"')
		{
			found = TRUE;
			*end = i - start;
			s = NIL;
		}
	}
	status = found ? SUCCESS : NIL;
	status = ERR(s) ? ERROR : status;
	return (f ? f(str, start, *end) : status);
}

/*
** Process parameter expansions of form ${} and $_, allows embedded arithmetic
** expansions, command substitution, backticks and parameter expansions, single
** and double quotes. Calls specified callback with *end set to ERROR if expansion
** not properly closed
*/

int		param_exp(char **str, size_t start, size_t *end, t_quote f)
{
	size_t	i;
	int		found;
	int		s;
	int		status;

	found = FALSE;
	i = start;
	s = SUCCESS;
	if ((*str)[i + 1] != '{')
	{
		i += 1;
		while (IS_VAR_CHAR((*str)[i]) && IS_VAR_CHAR((*str)[i + 1]))
			i += 1;
		found = TRUE;
		*end = i - start;
		return (f ? f(str, start, *end) : SUCCESS);
	}
	i += 2;
	while (OK(s) && (*str)[i])
	{
		if (
			(BACKSLASH((*str), i) && !ERR((s = backslash(str, i, end, f))))
			|| (DBL_QUOTE((*str), i) && !ERR((s = dbl_quote(str, i, end, f))))
			|| (SNGL_QUOTE((*str), i) && !ERR((s = quote(str, i, end, f))))
			|| (ARITH_EXP((*str), i) && !ERR((s = arith_exp(str, i, end, f))))
			|| (CMD_SUB((*str), i) && !ERR((s = command_sub(str, i, end, f))))
			|| (BACKTICK((*str), i) && !ERR((s = backtick(str, i, end, f))))
			|| (PARAM_EXP((*str), i) && !ERR((s = param_exp(str, i, end, f)))))
		{
			i += *end;
			*end = ERROR;
		}
		else if (OK(s) && (*str)[i] == '}')
		{
			*end = i - start;
			break ;
		}
		i += 1;
	}
	status = found ? SUCCESS : NIL;
	status = ERR(s) ? ERROR : status;
	return (f ? f(str, start, *end) : status);
}


/*
** Process command substitution of form $(), allows embedded arithmetic
** expansions, command substitution, backticks and parameter expansions, single
** and double quotes. Calls specified callback with *end set to ERROR if expansion
** not properly closed
*/

int		command_sub(char **str, size_t start, size_t *end, t_quote f)
{
	size_t	i;
	int		s;
	int		status;
	int		found;

	found = FALSE;
	i = start + 2;
	s = SUCCESS;
	while (OK(s) && (*str)[i])
	{
		if (
			(BACKSLASH((*str), i) && !ERR((s = backslash(str, i, end, f))))
			|| (DBL_QUOTE((*str), i) && !ERR((s = dbl_quote(str, i, end, f))))
			|| (SNGL_QUOTE((*str), i) && !ERR((s = quote(str, i, end, f))))
			|| (ARITH_EXP((*str), i) && !ERR((s = arith_exp(str, i, end, f))))
			|| (CMD_SUB((*str), i) && !ERR((s = command_sub(str, i, end, f))))
			|| (BACKTICK((*str), i) && !ERR((s = backtick(str, i, end, f))))
			|| (PARAM_EXP((*str), i) && !ERR((s = param_exp(str, i, end, f)))))
		{
			i += *end;
			*end = ERROR;
		}
		else if (OK(s) && (*str)[i] == ')')
		{
			*end = i - start;
			found = TRUE;
			s = NIL;
		}
		i += 1;
	}
	status = found ? SUCCESS : NIL;
	status = ERR(s) ? ERROR : status;
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

	i = start + 3;
	count = 0;
	found = FALSE;
	s = SUCCESS;
	while (OK(s) && (*str)[i])
	{
		if (
			(BACKSLASH((*str), i) && !ERR((s = backslash(str, i, end, f))))
			|| (ARITH_EXP((*str), i) && !ERR((s = arith_exp(str, i, end, f))))
			|| (CMD_SUB((*str), i) && !ERR((s = command_sub(str, i, end, f))))
			|| (BACKTICK((*str), i) && !ERR((s = backtick(str, i, end, f))))
			|| (PARAM_EXP((*str), i) && !ERR((s = param_exp(str, i, end, f)))))
		{
			i += *end;
			*end = ERROR;
		}
		else if (OK(s) && (*str)[i] == '(')
			count += 1;
		else if (OK(s) && count && (*str)[i] == ')')
			count -= 1;
		else if (OK(s) && TWO_CLOSING((*str), i))
		{
			found = TRUE;
			*end = i - start + 1;
			s = NIL;
		}
		i += 1;
	}
	status = found ? SUCCESS : NIL;
	status = ERR(s) ? ERROR : status;
	return (f ? f(str, start, *end) : status);
}
