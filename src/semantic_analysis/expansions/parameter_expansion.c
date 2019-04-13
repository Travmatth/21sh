/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 17:03:30 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/07 17:43:12 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

/*
** If a string does not beginjwith a param start ($), it should be appended
** to the expanded output. Text containing single quotes should be appended
** unmodified 
*/

int		join_unexpanded(char **new, char **str, size_t *i)
{
	size_t	end;
	size_t	start;
	int		s;
	char	*next;
	char	*tmp;

	s = SUCCESS;
	start = *i;
	while ((*str)[*i])
	{
		if ((*str)[*i] == '\\')
			*i += 1;
		if ((SNGL_QUOTE((*str), *i) && OK((s = quote(str, *i, &end, NULL))))
			|| (ARITH_EXP((*str), *i) && OK((s = arith_exp(str, *i, &end, NULL))))
			|| (BACKTICK((*str), *i) && OK((s = backtick(str, *i, &end, NULL)))))
			*i += end;
		else if ((*str)[*i] == '$' && start != *i && !escaped(*str, *i))
			break ;
		*i += 1;
	}
	if (!(tmp = ft_strsub(*str, start, *i - start)))
		return (ERROR);
	if (!*new)
	{
		*new = tmp;
		return (SUCCESS);
	}
	if (!(next = ft_strjoin(*new, tmp)))
		return (ERROR);
	free(tmp);
	free(*new);
	*new = next;
	return (SUCCESS);
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
	status = SUCCESS;
	param = *str + *skip;
	if (!ft_strncmp("$$", param, 2) || !ft_strncmp("${${", param, 4))
	{
		ft_printf("Semantic Error: Bad Parameter Expansion");
		status = NIL;
	}
	if (CMD_SUB((*str), i) && OK((status = command_sub(str, 0, &i, NULL))))
		i += 1;
	else if (OK(status) && enclosed(param, '-'))
		status = use_defaults_param_expansion(&next, param, &i);
	else if (OK(status) && enclosed(param, '='))
		status = assign_defaults_param_expansion(&next, param, &i);
	else if (OK(status) && enclosed(param, '?'))
		status = error_unset_param_expansion(&next, param, &i);
	else if (OK(status) && enclosed(param, '+'))
		status = alternative_param_expansion(&next, param, &i);
	else if (OK(status))
		status = plain_param_expansion(&next, param, &i);
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
		if (PARAM_EXP((*str), i) || CMD_SUB((*str), i) || ARITH_EXP((*str), i))
			status = manage_expansions(&new, str, &i);
		else
			status = join_unexpanded(&new, str, &i);
	}
	free(*str);
	*str = new;
	return (status);
}
