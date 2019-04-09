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
	char	*string;

	string = *str;
	i = start;
	*end = ERROR;
	while (string[++i])
	{
		if (string[i] == '\'')
		{
			*end = i;
			break ;
		}
	}
	return (f(str, start, *end));
}

int		backtick(char **str, int start, int *end, t_quote f)
{
	int		i;
	char	*string;

	string = *str;
	i = start;
	*end = ERROR;
	while (string[++i])
	{
		if (string[i] == '`')
		{
			*end = i;
			break ;
		}
	}
	return (f(str, start, *end));
}

int		dbl_quote(char **str, int start, int *end, t_quote f)
{
	int		i;
	int		s;

	i = start;
	while ((*str)[++i])
	{
		if (!escaped(*str, i) && (*str)[i] == '"')
		{
			*end = i;
			break ;
		}
		else if (!escaped(*str, i) && ((*str)[i] == '\'' || (*str)[i] == '$'))
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
	return (ERR(s) ? s : f(str, start, *end));
}

int		param_exp(char **str, int start, int *end, t_quote f)
{
	int		i;
	int		s;

	i = start;
	while (!escaped(*str, i) && (*str)[++i])
	{
		if ((*str)[i] == '}')
		{
			*end = i;
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
	return (ERR(s) ? s : f(str, start, *end));
}

int		command_sub(char **str, int start, int *end, t_quote f)
{
	int		i;
	int		s;

	i = start;
	while ((*str)[++i])
	{
		if (!escaped(*str, i) && (*str)[i] == ')')
		{
			*end = i;
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
	return (ERR(s) ? s : f(str, start, *end));
}

int		arith_exp(char **str, int start, int *end, t_quote f)
{
	int		i;
	int		s;

	i = start;
	while ((*str)[++i])
	{
		if (!escaped(*str, i) && (*str)[i] == ')' && NEXT_PAREN(*str, i))
		{
			*end = i;
			break ;
		}
		else if (!escaped(*str, i) && ((*str)[i] == '\'' || (*str)[i] == '$'))
		{
			if ((ARITH_EXP(*str, i) && !OK((s = arith_exp(str, i, end, f))))
				|| (PARAM_EXP(*str, i) && !OK((s = param_exp(str, i, end, f))))
				|| (CMD_SUB(*str, i) && !OK((s = command_sub(str, i, end, f))))
				|| (BACKTICK(*str, i) && !OK((s = backtick(str, i, end, f)))))
				break ;
			i += *end + 1;
			*end = ERROR;
		}
	}
	return (ERR(s) ? s : f(str, start, *end));
}
