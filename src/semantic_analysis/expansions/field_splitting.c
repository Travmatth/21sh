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

int		count_fields(char **str)
{
	int		i;
	size_t	skip;
	int		status;
	int		count;

	i = 0;
	count = 0;
	status = SUCCESS;
	while (OK(status) && ((*str)[i]))
	{
		skip = 0;
		while (IS_IFS((*str)[i]))
			i += 1;
		count += 1;
		while ((*str)[i] && (!IS_IFS((*str)[i]) || escaped(*str, i)))
		{
			if ((SNGL_QUOTE((*str), i)
					&& OK((status = quote(str, i, &skip, NULL))))
				|| (DBL_QUOTE((*str), i)
					&& OK((status = dbl_quote(str, i, &skip, NULL)))))
				i += skip + 1;
			else
				i += 1;
		}
	}
	return (count);
}

int		field_splitting(char ***fields, char **parameter)
{
	int		i;
	size_t	skip;
	int		status;
	int		start;
	int		size;
	char	**arr;

	i = 0;
	status = SUCCESS;
	size = count_fields(parameter);
	if (!(arr = ft_memalloc(sizeof(char*) * (size + 1))))
		return (ERROR);
	size = 0;
	while (OK(status) && (*parameter)[i])
	{
		skip = 0;
		while (IS_IFS((*parameter)[i]))
			i += 1;
		start = i;
		while ((*parameter)[i] && (!IS_IFS((*parameter)[i]) || escaped(*parameter, i)))
		{
			if ((SNGL_QUOTE((*parameter), i)
					&& OK((status = quote(parameter, i, &skip, NULL))))
				|| (DBL_QUOTE((*parameter), i)
					&& OK((status = dbl_quote(parameter, i, &skip, NULL)))))
				i += skip + 1;
			else
				i += 1;
		}
		if (OK(status) && (i - start) && (!(arr[size++] = ft_strsub(*parameter, start, i))))
			return (ERROR);
	}
	*fields = arr;
	return (status);
}
