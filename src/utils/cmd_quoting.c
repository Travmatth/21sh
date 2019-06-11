/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_quoting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 19:30:01 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/10 21:25:59 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		first_word(char *str, int i)
{
	int		is;

	is = TRUE;
	while (--i > 0)
	{
		if (i && str[i - 1] == '\\')
			i -= 1;
		else if (str[i] == ';' || str[i] == '&')
			return (TRUE);
		else if (IS_WHITESPACE(str[i]))
			continue ;
		return (FALSE);
	}
	return (is);
}

int		parse_all(char **str, size_t start, size_t *end, t_quote f)
{
	int		status;

	status = SUCCESS;
	if ((P_BACKSLASH(status, str, f, start, end))
		|| (P_QUOTE(status, str, f, start, end))
		|| (P_DQUOTE(status, str, f, start, end))
		|| (P_BACKTICK(status, str, f, start, end))
		|| (P_ARITH(status, str, f, start, end))
		|| (P_CMD(status, str, f, start, end))
		|| (P_EPARAM(status, str, f, start, end))
		|| (P_SUBSHELL(status, str, f, start, end))
		|| (P_CURSHELL(status, str, f, start, end)))
		return (status);
	return (status);
}

int		subshell(char **str, size_t start, size_t *end, t_quote f)
{
	size_t	i;
	int		status;
	int		found;

	status = init_exp(&i, start, &found);
	while (OK(status) && (*str)[++i])
	{
		if (OK(status) && (*str)[i] == ')')
			status = dbl_quote_switch(end, &found, i - start);
		else if (OK(status)
			&& OK((status = parse_all(str, i, end, f)))
			&& *end != (size_t)ERROR)
		{
			i += *end;
			*end = ERROR;
		}
	}
	status = found ? SUCCESS : NIL;
	status = ERR(status) ? ERROR : status;
	return (f ? f(str, start, *end) : status);
}

int		cur_shell(char **str, size_t start, size_t *end, t_quote f)
{
	size_t	i;
	int		status;
	int		found;

	status = init_exp(&i, start, &found);
	if (!(*str)[start + 1])
	{
		*end += 1;
		return (f ? f(str, start, *end) : status);
	}
	while (OK(status) && (*str)[++i])
	{
		if (OK(status) && (*str)[i] == '}')
			status = dbl_quote_switch(end, &found, i - start);
		else if (OK(status)
			&& OK((status = parse_all(str, i, end, f)))
			&& *end != (size_t)ERROR)
		{
			i += *end;
			*end = ERROR;
		}
	}
	status = found ? SUCCESS : NIL;
	status = ERR(status) ? ERROR : status;
	return (f ? f(str, start, *end) : status);
}
