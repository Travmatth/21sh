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

int		join_unexpanded(char **new, char **str)
{
	size_t	i;
	int		in;
	char	*string;
	char	*next;
	char	*tmp;

	i = 0;
	in = FALSE;
	string = *str;
	while (string[i])
	{
		if (in && string[i] == '\'')
			in = FALSE;
		else if (string[i] == '\'')
			in = TRUE;
		else if (!in && string[i] == '$')
			break ;
		i += 1;
	}
	if (!(tmp = ft_strsub(string, 0, i)))
		return (ERROR);
	*str += i;
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

int		manage_expansions(char **new, char **str)
{
	int		status;
	char	*next;
	char	*tmp;
	size_t	i;

	i = 0;
	if (!ft_strncmp("$$", *str, 2) || !ft_strncmp("${${", *str, 2))
	{
		ft_printf("Semantic Error: Bad Parameter Expansion");
		status = NIL;
	}
	else if (!ft_strncmp("$(", *str, 2))
		status = skip_parens(*str, &i);
	else if (enclosed(*str, '-'))
		status = use_defaults_param_expansion(&next, *str, &i);
	else if (enclosed(*str, '='))
		status = assign_defaults_param_expansion(&next, *str, &i);
	else if (enclosed(*str, '?'))
		status = error_unset_param_expansion(&next, *str, &i);
	else if (enclosed(*str, '+'))
		status = alternative_param_expansion(&next, *str, &i);
	else
		status = plain_param_expansion(&next, *str, &i);
	if (OK(status) && !(tmp = ft_strjoin(*new, next)))
		return (ERROR);
	else if (OK(status))
	{
		free(*new);
		*new = tmp;
	}
	*str += i + 1;
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

int		parameter_expansion(char **parameter)
{
	char	*new;
	char	*single;
	char	*param;
	char	*str;
	int		status;

	new = NULL;
	status = SUCCESS;
	str = *parameter;
	while (OK(status) && *str)
	{
		single = ft_strchr(str, '\'');
		param = ft_strchr(str, '$');
		// if plain string, append remaining string to new
		if (!single && !param)
			status = join_unexpanded(&new, &str);
		// if no more quote left, process next parameter expansion onto new
		else if (!single && param)
			status = manage_expansions(&new, &str);
		// if no parameters left, append remaining string to new
		else if (single && !param)
			status = join_unexpanded(&new, &str);
		// if quotes precedes next expansion, append quote to new  
		else if (single < param)
			status = join_unexpanded(&new, &str);
		// if expansion precedes next quote, append expansion to new  
		else
			status = manage_expansions(&new, &str);
	}
	free(*parameter);
	*parameter = new;
	return (status);
}
