/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parameter_expansion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 17:03:30 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/08 23:23:03 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

int		store_unexpanded(char **new, char **str, size_t start, size_t *i)
{
	char	*next;
	char	*tmp;

	if (!(tmp = ft_strsub(*str, start, *i - start)))
		return (ERROR);
	if (!*new && (*new = tmp))
		return (SUCCESS);
	if (!(next = ft_strjoin(*new, tmp)))
		return (ERROR);
	free(tmp);
	free(*new);
	*new = next;
	return (SUCCESS);
}

/*
** If a string does not begin with a param start ($), it should be appended
** to the expanded output. Text containing single quotes should be appended
** unmodified
*/

int		join_unexpanded(char **new, char **str, size_t *i)
{
	size_t	end;
	size_t	start;
	int		status;

	status = SUCCESS;
	start = *i;
	while ((*str)[*i])
	{
		if ((*str)[*i] == '\\')
			*i += 1;
		else if ((P_QUOTE(status, str, NULL, (*i), (&end)))
			|| (P_DQUOTE(status, str, NULL, (*i), (&end)))
			|| (P_ARITH(status, str, NULL, (*i), (&end)))
			|| (P_CMD(status, str, NULL, (*i), (&end)))
			|| (P_BACKTICK(status, str, NULL, (*i), (&end))))
			*i += end;
		else if ((*str)[*i] == '$' && start != *i && !escaped(*str, *i))
			break ;
		*i += 1;
	}
	return (ERR(store_unexpanded(new, str, start, i)) ? ERROR : status);
}

int		switch_expansion(char *param, char **next, size_t *i)
{
	if (enclosed(param, '-'))
		ft_putendl("Semantic Error: default expansion not implemented");
	else if (enclosed(param, '='))
		ft_putendl("Semantic Error: default assign expansion not implemented");
	else if (enclosed(param, '?'))
		ft_putendl("Semantic Error: error unset expansion not implemented");
	else if (enclosed(param, '+'))
		ft_putendl("Semantic Error: alt param expansion not implemented");
	else
		return (plain_param_expansion(next, param, i));
	return (NIL);
}

/*
** If a string begins with a parameter expansion symbol ($), the expansion
** should be performed and its result appended to the expanded output.
*/

int		manage_expansions(char **new, char **str, size_t *skip)
{
	int		status;
	char	*next;
	char	*param;
	char	*tmp;
	size_t	i;

	i = 0;
	next = NULL;
	param = *str + *skip;
	status = switch_expansion(param, &next, &i);
	if (OK(status) && *new && !(tmp = ft_strjoin(*new, next)))
		return (ERROR);
	else if (OK(status) && !*new)
		*new = next;
	else if (OK(status))
	{
		free(*new);
		free(next);
		*new = tmp;
	}
	*skip += i;
	return (status);
}

/*
** Parameter Expansion
** The format for parameter expansion is as follows:
**
** ${expression}
** where expression consists of all characters until the matching '}'. Any '}'
** escaped by a <backslash> or within a quoted string, and characters in
** embedded arithmetic expansions, command substitutions, and variable
** expansions, shall not be examined in determining the matching '}'.
** If the parameter is not enclosed in braces, and is a name, the expansion
** shall use the longest valid name (see XBD Name), whether or not the variable
** represented by that name exists. Otherwise, the parameter is a
** single-character symbol, and behavior is unspecified if that character is
** neither a digit nor one of the special parameters (see Special Parameters).
**
** If a parameter expansion occurs inside double-quotes:
** Pathname expansion shall not be performed on the results of the expansion.
** Field splitting shall not be performed on the results of the expansion.
*/

int		parameter_expansion(char **str)
{
	char	*new;
	int		status;
	size_t	i;

	i = 0;
	new = NULL;
	status = SUCCESS;
	while (OK(status) && (*str)[i])
	{
		if (!ft_strncmp("$$", (*str), 2) || !ft_strncmp("${${", (*str), 4))
		{
			ft_printf("Semantic Error: Bad Parameter Expansion");
			status = NIL;
		}
		else
			status = UPARAM_EXP((*str), i) || EPARAM_EXP((*str), i)
				? manage_expansions(&new, str, &i)
				: join_unexpanded(&new, str, &i);
	}
	free(*str);
	*str = new;
	return (status);
}
