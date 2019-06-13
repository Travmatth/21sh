/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:55:40 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/12 18:13:30 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

void	handle_rest(char **str, char *new, int index[2])
{
	if ((*str)[index[I]] == '\\')
	{
		index[I] += 1;
		new[index[J]] = (*str)[index[I]];
		index[I] += 1;
		index[J] += 1;
	}
	else
	{
		new[index[J]] = (*str)[index[I]];
		index[I] += 1;
		index[J] += 1;
	}
}

void	remove_quote_switch(char *quote, char **str, char *new, int index[2])
{
	if (!*quote && ((*str)[index[I]] == '"' || (*str)[index[I]] == '\''))
	{
		*quote = (*str)[index[I]];
		index[I] += 1;
	}
	else if (*quote && (*str)[index[I]] == *quote)
	{
		index[I] += 1;
		*quote = 0;
	}
	else if (*quote && (*str)[index[I]] == '\\' && (*str)[index[I] + 1] == 'n')
	{
		new[index[J]] = '\n';
		index[I] += 2;
		index[J] += 1;
	}
	else if (*quote && (*str)[index[I]] == '\\' && (*str)[index[I] + 1] == 't')
	{
		new[index[J]] = '\t';
		index[I] += 2;
		index[J] += 1;
	}
	else
		handle_rest(str, new, index);
}

/*
** Iterate through string and create new string, skipping any escape characters
** and unescaped quote characters
*/

int		remove_quotes(char **str)
{
	int		index[2];
	size_t	len;
	char	*new;
	char	quote;

	len = LEN(*str, 0);
	if (!(new = ft_strnew(len)))
		return (ERROR);
	index[I] = 0;
	index[J] = 0;
	quote = 0;
	while ((*str)[index[I]])
		remove_quote_switch(&quote, str, new, index);
	free(*str);
	*str = new;
	return (SUCCESS);
}

/*
** Quote Removal
** The quote characters (<backslash>, single-quote, and double-quote) that were
** present in the original word shall be removed unless they have
** themselves been quoted.
*/

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
