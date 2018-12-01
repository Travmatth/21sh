/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 21:44:04 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/30 18:25:54 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

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
