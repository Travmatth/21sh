/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 21:44:04 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/25 13:07:56 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	if (**str == *next)
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

char	*next_param(char **command)
{
	char	*tmp;
	char	next;
	int		len;

	while (**command == ' ' || **command == '\t')
		*command += 1;
	if (**command == '"' && (*command += 1))
		next = '"';
	else if (**command == '\'' && (*command += 1))
		next = '\'';
	else
		next = ' ';
	tmp = ft_strchr(*command, next);
	if (tmp && tmp != *command && *(tmp - 1) == '\\')
		tmp = ft_strchr(tmp + 1, next);
	if (!tmp)
		tmp = ft_strchr(*command, '\0');
	len = tmp - *command;
	tmp = ft_strsub(*command, 0, len);
	*command += len + 1;
	return (tmp);
}

char	**remove_quotations(char *command, int ac)
{
	char	**av;
	int		i;

	i = 0;
	av = (char**)ft_memalloc(sizeof(char*) * (ac + 1));
	while (command && *command)
	{
		av[i++] = next_param(&command);
	}
	return (av);
}
