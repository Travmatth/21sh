/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_splitting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:47:39 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/30 11:57:41 by tmatthew         ###   ########.fr       */
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

/*
** Determine number of fields contained in string, for use in field-splitting
*/

int		count_fields(char **str, int *count)
{
	int		i;
	size_t	skip;
	int		status;

	i = 0;
	*count = 0;
	status = SUCCESS;
	while (OK(status) && ((*str)[i]))
	{
		skip = 0;
		while (IS_IFS((*str)[i]))
			i += 1;
		*count += 1;
		while ((*str)[i] && !IS_IFS((*str)[i]))
		{
			if ((P_BACKSLASH(status, str, NULL, i, (&skip)))
				|| (P_QUOTE(status, str, NULL, i, (&skip)))
				|| (P_DQUOTE(status, str, NULL, i, (&skip))))
				i += skip + 1;
			else
				i += 1;
		}
	}
	return (status);
}

int		init_field_split(t_field_split *fs, char **parameter)
{
	int		status;

	status = SUCCESS;
	fs->i = 0;
	status = count_fields(parameter, &fs->size);
	if (OK(status) && !(fs->arr = ft_memalloc(sizeof(char*) * (fs->size + 1))))
		status = ERROR;
	fs->size = 0;
	return (status);
}

int		field_splitting(char ***fields, char **parameter)
{
	t_field_split	fs;
	int				status;

	status = init_field_split(&fs, parameter);
	while (OK(status) && (*parameter)[fs.i])
	{
		fs.skip = 0;
		while (IS_IFS((*parameter)[fs.i]))
			fs.i += 1;
		fs.start = fs.i;
		while (NOT_IFS(parameter, fs))
		{
			if ((P_BACKSLASH(status, parameter, NULL, fs.i, (&fs.skip)))
				|| (P_QUOTE(status, parameter, NULL, fs.i, (&fs.skip)))
				|| (P_DQUOTE(status, parameter, NULL, fs.i, (&fs.skip))))
				fs.i += fs.skip + 1;
			else
				fs.i += 1;
		}
		if (OK(status) && STORE_SUBSTRING(parameter, fs))
			return (ERROR);
	}
	*fields = fs.arr;
	free(*parameter);
	return (status);
}
