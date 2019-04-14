/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathname_expansion.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:44:02 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/13 16:44:24 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

/*
** Pathname Expansion
** After field splitting, if set -f is not in effect, each field in the
** resulting command line shall be expanded using the algorithm described in
** Pattern Matching Notation, qualified by the rules in Patterns Used for
** Filename Expansion.
*/

int		find_pathname_end(char *str, int *skip)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\\')
			i += 1;
		else if (str[i] == '}')
		{
			ft_printf("Semantic Error: pathname expansion not implemented\n");
			return (NIL);
		}
		i += 1;
	}
	*skip = *skip + i;
	return (SUCCESS);
}

int		expand_pathname(char **field)
{
	int		i;
	char	*str;
	int		status;

	i = 0;
	str = *field;
	status = SUCCESS;
	while (str[i])
	{
		if (str[i] == '\\')
			i += 1;
		else if (str[i] == '{')
		{
			if (!OK((status = find_pathname_end(&str[i], &i))))
				break ;
		}
		i += 1;
	}
	return (status);
}

int		pathname_expansion(char ***fields)
{
	int		i;
	int		status;

	i = 0;
	while ((*fields)[i])
		if (!OK((status = expand_pathname(&(*fields)[i++]))))
			break ;
	return (status);
}
