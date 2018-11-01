/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 21:44:04 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/01 16:30:31 by tmatthew         ###   ########.fr       */
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

char	*closing_char(char **command, char *next, int outside)
{
	char	*tmp;
	char	*first;

	if (!*next)
	{
		*next = (**command == '"') ? '"' : 0;
		*next = (**command == '\'') ? '\'' : *next;
		*next = !*next ? ' ' : *next;
	}
	if (outside && IS_SEP(**command))
		*command += 1;
	tmp = ft_strchr(*command, *next);
	if (*next == ' ' && (first = ft_strchr(*command, '\t')))
	{
		tmp = tmp ? tmp : first;
		tmp = tmp && first && (first < tmp) ? first : tmp;
	}
	if (tmp && tmp != *command && *(tmp - 1) == '\\')
	{
		tmp += 1;
		tmp = closing_char(&tmp, next, 0);
	}
	return (tmp);
}

char	**remove_quotations(char *command, int ac)
{
	char	**av;
	int		i;
	char	*tmp;
	char	next;
	int		len;

	i = 0;
	av = (char**)ft_memalloc(sizeof(char*) * (ac + 1));
	while (command && *command)
	{
		next = 0;
		while (IS_SEP(*command) && IS_SEP(command[1]))
			command += 1;
		if (IS_SEP(*command) && !command[1])
			break ;
		tmp = closing_char(&command, &next, 1);
		if (!tmp)
			tmp = ft_strchr(command, '\0');
		len = (tmp ? tmp : command) - command;
		tmp = ft_strsub(command, 0, len);
		command += len + 1;
		av[i++] = ft_strfilter(remove_slash, tmp);
		free(tmp);
	}
	return (av);
}
