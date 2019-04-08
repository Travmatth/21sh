/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_splitting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:47:39 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/06 13:49:45 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

/*
** Field Splitting
** After parameter expansion (Parameter Expansion), command substitution
** (Command Substitution), and arithmetic expansion (Arithmetic Expansion), the
** shell shall scan the results of expansions and substitutions that did not
** occur in double-quotes for field splitting and multiple fields can result.
**
** The shell shall treat each character of the IFS as a delimiter and use the
** delimiters as field terminators to split the results of parameter expansion,
** command substitution, and arithmetic expansion into fields.
**
** If the value of IFS is a <space>, <tab>, and <newline>, or if it is unset,
** any sequence of <space>, <tab>, or <newline> characters at the beginning or
** end of the input shall be ignored and any sequence of those characters
** within the input shall delimit a field. For example, the input:
**
** <newline><space><tab>foo<tab><tab>bar<space>
**
** yields two fields, foo and bar.
**
** If the value of IFS is null, no field splitting shall be performed.
** Otherwise, the following rules shall be applied in sequence. The term "IFS
** white space" is used to mean any sequence (zero or more instances) of
** white-space characters that are in the IFS value (for example, if IFS
** contains <space>/ <comma>/ <tab>, any sequence of <space> and <tab>
** characters is considered IFS white space).
**
** IFS white space shall be ignored at the beginning and end of the input.
**
** Each occurrence in the input of an IFS character that is not IFS white space,
** along with any adjacent IFS white space, shall delimit a field, as described
** previously.
**
** Non-zero-length IFS white space shall delimit a field.
*/

int		count_fields(char *str)
{
	int		i;
	int		count;
	char	quote;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (IS_IFS(str[i]) && !escaped(str, i))
			i += 1;
		if (!str[i])
			break ;
		count += 1;
		if ((str[i] == '\'' || str[i] == '"') && !escaped(str, i))
		{
			quote = str[i++];
			while (str[i] != quote || escaped(str, i))
				i += 1;
		}
		else
			while (str[i] && (!IS_IFS(str[i]) || escaped(str, i)))
				i += 1;
	}
	return (count);
}

int		field_splitting(char ***fields, char *parameter)
{
	int		i;
	int		start;
	int		size;
	char	**arr;
	char	quote;

	i = 0;
	size = count_fields(parameter);
	if (!(arr = ft_memalloc(sizeof(char*) * (size + 1))))
		return (ERROR);
	size = 0;
	while (parameter[i])
	{
		while (parameter[i] && (IS_IFS(parameter[i])) && !escaped(parameter, i))
			i += 1;
		if (!parameter[i])
			break ;
		start = i;
		if ((parameter[i] == '\'' || parameter[i] == '"') && !escaped(parameter, i))
		{
			quote = parameter[i++];
			while (parameter[i] && (parameter[i] != quote || escaped(parameter, i)))
				i += 1;
			if (!(arr[size++] = ft_strsub(parameter, start, ++i)))
				return (ERROR);
		}
		else
		{
			while (parameter[i] && (!IS_IFS(parameter[i]) || escaped(parameter, i)))
				i += 1;
			if (!(arr[size++] = ft_strsub(parameter, start, i)))
				return (ERROR);
		}
	}
	*fields = arr;
	return (SUCCESS);
}
