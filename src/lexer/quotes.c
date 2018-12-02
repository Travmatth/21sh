/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 21:44:04 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/01 16:22:16 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	set_outside_word(char **str, char *next, int *inside_word, int *current)
{
	if (**str == ' ' || **str == '\t')
		*next = 0;
	else if (**str == '\'')
	{
		*next = '\'';
		*current += 1;
		*inside_word = 1;
	}
	else if (**str == '"')
	{
		*next = '"';
		*current += 1;
		*inside_word = 1;
	}
	else
	{
		*next = ' ';
		*current += 1;
		*inside_word = 1;
	}
}

void	set_inside_word(char **str, char *next, int *inside_word)
{
	if (**str == *next || (*next == ' ' && **str == '\t'))
	{
		*next = 0;
		*inside_word = 0;
	}
	else
		*inside_word = 1;
}

int		count_params(char *command)
{
	char	*str;
	char	next;
	int		inside_word;
	int		current;

	str = command;
	next = 0;
	current = 0;
	inside_word = 0;
	while ((str && *str))
	{
		if (*str == '\\')
			str += 1;
		else if (inside_word)
			set_inside_word(&str, &next, &inside_word);
		else
			set_outside_word(&str, &next, &inside_word, &current);
		str += 1;
	}
	return ((next == '\'' || next == '\"') ? -1 : current);
}
