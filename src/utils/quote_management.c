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

int		quote(char **str, int start, int *end, t_quote f)
{
	int		i;
	int		status;
	char	*string;

	string = *str;
	i = start;
	*end = ERROR;
	while (string[++i])
	{
		if (string[i] == '\'')
		{
			*end = i - start;
			break ;
		}
	}
	status = ERR(*end) ? NIL : SUCCESS;
	return (f ? f(str, start, *end) : status);
}

int		backtick(char **str, int start, int *end, t_quote f)
{
	int		i;
	int		status;
	char	*string;

	string = *str;
	i = start;
	*end = ERROR;
	while (string[++i])
	{
		if (string[i] == '`')
		{
			*end = i - start;
			break ;
		}
	}
	status = ERR(*end) ? NIL : SUCCESS;
	return (f ? f(str, start, *end) : status);
}

int		dbl_quote(char **str, int start, int *end, t_quote f)
{
	int		i;
	int		s;
	int		status;

	i = start;
	while ((*str)[++i])
	{
		if ((*str)[i] == '"' && !escaped(*str, i))
		{
			*end = i - start;
			break ;
		}
		else if (((*str)[i] == '\'' || (*str)[i] == '$') && !escaped(*str, i))
		{
			if ((SNGL_QUOTE(*str, i) && !OK((s = quote(str, i, end, f))))
				|| (PARAM_EXP(*str, i) && !OK((s = param_exp(str, i, end, f))))
				|| (ARITH_EXP(*str, i) && !OK((s = arith_exp(str, i, end, f))))
				|| (CMD_SUB(*str, i) && !OK((s = command_sub(str, i, end, f))))
				|| (BACKTICK(*str, i) && !OK((s = backtick(str, i, end, f)))))
				break ;
			i += *end + 1;
			*end = ERROR;
		}
	}
	status = ERR(*end) ? NIL : SUCCESS;
	status = ERR(s) ? ERROR : status;
	return (f ? f(str, start, *end) : status);
}

int		param_exp(char **str, int start, int *end, t_quote f)
{
	int		i;
	int		s;
	int		status;

	i = start;
	if ((*str)[i + 1] != '{')
	{
		i += 1;
		while (IS_VAR_CHAR((*str)[i]))
			i += 1;
		*end = i - start;
		return (SUCCESS);
	}
	while (!escaped(*str, i) && (*str)[++i])
	{
		if ((*str)[i] == '}')
		{
			*end = i - start;
			break ;
		}
		else if (!escaped(*str, i) && ((*str)[i] == '\'' || (*str)[i] == '$'))
		{
			if ((SNGL_QUOTE(*str, i) && !OK((s = quote(str, i, end, f))))
				|| (DBL_QUOTE(*str, i) && !OK((s = dbl_quote(str, i, end, f))))
				|| (ARITH_EXP(*str, i) && !OK((s = arith_exp(str, i, end, f))))
				|| (PARAM_EXP(*str, i) && !OK((s = param_exp(str, i, end, f))))
				|| (CMD_SUB(*str, i) && !OK((s = command_sub(str, i, end, f))))
				|| (BACKTICK(*str, i) && !OK((s = backtick(str, i, end, f)))))
				break ;
			i += *end + 1;
			*end = ERROR;
		}
	}
	status = ERR(*end) ? NIL : SUCCESS;
	status = ERR(s) ? ERROR : status;
	return (f ? f(str, start, *end) : status);
}

int		command_sub(char **str, int start, int *end, t_quote f)
{
	int		i;
	int		s;
	int		status;

	i = ++start;
	while ((*str)[i])
	{
		if ((*str)[i] == ')' && !escaped(*str, i))
		{
			*end = i - start;
			break ;
		}
		else if (((*str)[i] == '\'' || (*str)[i] == '$') && !escaped(*str, i))
		{
			if ((SNGL_QUOTE((*str), i) && !OK((s = quote(str, i, end, f))))
				|| (DBL_QUOTE((*str), i) && !OK((s = dbl_quote(str, i, end, f))))
				|| (ARITH_EXP((*str), i) && !OK((s = arith_exp(str, i, end, f))))
				|| (CMD_SUB((*str), i) && !OK((s = command_sub(str, i, end, f))))
				|| (BACKTICK((*str), i) && !OK((s = backtick(str, i, end, f))))
				|| ((*str)[i] == '$' && !OK((s = param_exp(str, i, end, f)))))
				break ;
			i += *end;
			*end = ERROR;
		}
		else
			i += 1;
	}
	status = ERR(*end) ? NIL : SUCCESS;
	status = ERR(s) ? ERROR : status;
	return (f ? f(str, start, *end) : status);
}

int		arith_exp(char **str, int start, int *end, t_quote f)
{
	int		i;
	int		s;
	int		status;
	int		count;

	i = start + 3;
	count = 0;
	while ((*str)[i])
	{
		if ((*str)[i] == '(')
			count += 1;
		else if (count && (*str)[i] == ')')
			count -= 1;
		else if ((*str)[i] == ')' && (*str)[i + 1] == ')' && !escaped(*str, i))
		{
			*end = i - start + 1;
			break ;
		}
		else if (((*str)[i] == '\'' || (*str)[i] == '$') && !escaped(*str, i))
		{
			if ((ARITH_EXP((*str), i) && !OK((s = arith_exp(str, i, end, f))))
				|| (CMD_SUB((*str), i) && !OK((s = command_sub(str, i, end, f))))
				|| (BACKTICK((*str), i) && !OK((s = backtick(str, i, end, f))))
				|| ((*str)[i] == '$' && !OK((s = param_exp(str, i, end, f)))))
				break ;
			i += *end + 1;
			*end = ERROR;
		}
		else
			i += 1;
	}
	status = ERR(*end) ? NIL : SUCCESS;
	status = ERR(s) ? ERROR : status;
	return (f ? f(str, start, *end) : status);
}
