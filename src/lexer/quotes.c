/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 21:44:04 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/19 15:58:44 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** find next whitespace in complete_command not preceded by a backslash
*/

int		find_ws(char *complete_cmd, size_t *offset)
{
	size_t	i;

	i = 1;
	while (complete_cmd[i])
	{
		if (IS_WS(complete_cmd[i]))
		{
			*offset += i;
			return (SUCCESS);
		}
		i += 1;
	}
	return (ERROR);
}

/*
** find next character c in complete_command not preceded by a backslash
*/

int		find_next(char c, char *complete_cmd, size_t *offset)
{
	size_t	i;

	i = 1;
	while (complete_cmd && complete_cmd[i])
	{
		if (*complete_cmd == '\\' && complete_cmd[i + 1] == c)
		{
			if (NONE(find_next(c, &complete_cmd[i + 2], offset)))
				return (NIL);
			*offset += i;
			return (SUCCESS);
		}
		else if (*complete_cmd == c && i == 1)
		{
			*offset += i;
			return (SUCCESS);
		}
		complete_cmd += 1;
	}
	return (NIL);
}

/*
** determine whether character c in complete_command
** is quote not preceded by a backslash
*/

int		is_quote(char *complete_cmd, size_t n)
{
	if (n == 0 && IS_WS(*complete_cmd))
		return (SUCCESS);
	else if (n > 0 && *(complete_cmd - 1) != '\\' && IS_WS(*complete_cmd))
		return (SUCCESS);
	return (NIL);
}
