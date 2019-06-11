/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_quoting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 15:09:41 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/24 15:10:05 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Process backslash escapes of form \, calls specified callback with *end
** set to ERROR if expansion not properly closed
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
** Process single quotes of form '', calls specified callback with *end set
** to ERROR if expansion not properly closed
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
