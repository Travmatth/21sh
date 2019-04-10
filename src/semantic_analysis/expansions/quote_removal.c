/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:55:40 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/06 13:56:15 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

/*
** Quote Removal
** The quote characters (<backslash>, single-quote, and double-quote) that were
** present in the original word shall be removed unless they have
** themselves been quoted.
*/

int		remove_quotes(char **str)
{
	int		i;
	int		j;
	size_t	len;
	char	*new;
	char	quote;

	len = LEN(*str, 0);
	if (!(new = ft_strnew(len)))
		return (ERROR);
	i = 0;
	j = 0;
	quote = 0;
	while ((*str)[i])
	{
		if (!quote && ((*str)[i] == '"' || (*str)[i] == '\''))
			quote = (*str)[i++];
		else if (quote && (*str)[i] == quote)
		{
			i += 1;
			quote = 0;
		}
		else if (quote != '\'' && (*str)[i] == '\\')
		{
			i += 1;
			new[j++] = (*str)[i++];
		}
		else
			new[j++] = (*str)[i++];
	}
	free(*str);
	*str = new;
	return (SUCCESS);
}

int		quote_removal(char ***fields)
{
	int		i;
	int		status;

	i = 0;
	status = SUCCESS;
	while ((*fields)[i])
		if (!OK((status = remove_quotes(&(*fields)[i++]))))
			break ;
	return (status);
}
