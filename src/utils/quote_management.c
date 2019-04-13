/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 12:59:41 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/08 14:49:24 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

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

int		dbl_quote(char **str, size_t start, size_t *end, t_quote f)
{
	size_t	i;
	int		s;
	int		status;
	int		found;

	found = FALSE;
	i = start;
	while ((*str)[++i])
	{
		if ((*str)[i] == '\\' && (*str)[i + 1])
			i += 1;
		else if ((*str)[i] == '"')
		{
			found = TRUE;
			*end = i - start;
			break ;
		}
		else if (
			(ARITH_EXP((*str), i) && ANY((s = arith_exp(str, i, end, f))))
			// || (CMD_SUB((*str), i) && ANY((s = command_sub(str, i, end, f))))
			// || (BACKTICK((*str), i) && ANY((s = backtick(str, i, end, f))))
			// || (PARAM_EXP((*str), i) && ANY((s = param_exp(str, i, end, f))))
		)
		{
			if (!OK(s))
				break ;
			i += *end;
			*end = ERROR;
		}
	}
	status = found ? SUCCESS : NIL;
	status = ERR(s) ? ERROR : status;
	return (f ? f(str, start, *end) : status);
}

int		param_exp(char **str, size_t start, size_t *end, t_quote f)
{
	size_t	i;
	int		found;
	int		s;
	int		status;

	found = FALSE;
	i = start;
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
	while ((*str)[i])
	{
		if ((*str)[i] == '\\' && (*str)[i + 1])
			i += 1;
		if ((*str)[i] == '}')
		{
			*end = i - start;
			break ;
		}
		else if (
			(DBL_QUOTE((*str), i) && ANY((s = dbl_quote(str, i, end, f))))
			|| (SNGL_QUOTE((*str), i) && ANY((s = quote(str, i, end, f))))
			|| (ARITH_EXP((*str), i) && ANY((s = arith_exp(str, i, end, f))))
			|| (CMD_SUB((*str), i) && ANY((s = command_sub(str, i, end, f))))
			|| (BACKTICK((*str), i) && ANY((s = backtick(str, i, end, f))))
			|| (PARAM_EXP((*str), i) && ANY((s = param_exp(str, i, end, f)))))
		{
			if (!OK(s))
				break ;
			i += *end;
			*end = ERROR;
		}
		i += 1;
	}
	status = found ? SUCCESS : NIL;
	status = ERR(s) ? ERROR : status;
	return (f ? f(str, start, *end) : status);
}


int		command_sub(char **str, size_t start, size_t *end, t_quote f)
{
	size_t	i;
	int		s;
	int		status;
	int		found;

	found = FALSE;
	i = start + 2;
	while ((*str)[i])
	{
		if ((*str)[i] == '\\' && (*str)[i + 1])
			i += 1;
		if ((*str)[i] == ')')
		{
			*end = i - start;
			found = TRUE;
			break ;
		}
		else if (
			(DBL_QUOTE((*str), i) && ANY((s = dbl_quote(str, i, end, f))))
			|| (SNGL_QUOTE((*str), i) && ANY((s = quote(str, i, end, f))))
			|| (ARITH_EXP((*str), i) && ANY((s = arith_exp(str, i, end, f))))
			|| (CMD_SUB((*str), i) && ANY((s = command_sub(str, i, end, f))))
			|| (BACKTICK((*str), i) && ANY((s = backtick(str, i, end, f))))
			|| (PARAM_EXP((*str), i) && ANY((s = param_exp(str, i, end, f)))))
		{
			if (!OK(s))
				break ;
			i += *end;
			*end = ERROR;
		}
		i += 1;
	}
	status = found ? SUCCESS : NIL;
	status = ERR(s) ? ERROR : status;
	return (f ? f(str, start, *end) : status);
}

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
	while ((*str)[i])
	{
		if ((*str)[i] == '\\' && (*str)[i + 1])
			i += 1;
		else if ((*str)[i] == '(')
			count += 1;
		else if (count && (*str)[i] == ')')
			count -= 1;
		else if (TWO_CLOSING((*str), i))
		{
			found = TRUE;
			*end = i - start + 1;
			break ;
		}
		else if (
			(ARITH_EXP((*str), i) && ANY((s = arith_exp(str, i, end, f))))
			|| (CMD_SUB((*str), i) && ANY((s = command_sub(str, i, end, f))))
			|| (BACKTICK((*str), i) && ANY((s = backtick(str, i, end, f))))
			|| (PARAM_EXP((*str), i) && ANY((s = param_exp(str, i, end, f)))))
		{
			if (!OK(s))
				break ;
			i += *end;
			*end = ERROR;
		}
		i += 1;
	}
	status = found ? SUCCESS : NIL;
	status = ERR(s) ? ERROR : status;
	return (f ? f(str, start, *end) : status);
}
