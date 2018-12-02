/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 21:44:04 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/01 19:39:09 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

# define END 1
# define BEGIN 0

// if we encounter another opening candidate
// we need to restart search w/ that sequence

int		find_sub_end(char c, char *complete_cmd, size_t *j)
{
	(void)c;
	(void)complete_cmd;
	(void)j;
	return (SUCCESS);
}

int		escaped(char *complete_cmd, size_t i)
{
	if (i == 0)
		return (FALSE);
	else if (complete_cmd[i - 1] == '\\')
		return (TRUE);
	return (FALSE);
}

/*
** given compete_cmd with op starting at i, determine whether operator is valid
*/

int		can_form_op(char *complete_cmd, int in_op, size_t *i, int position)
{
	(void)complete_cmd;
	(void)in_op;
	(void)i;
	(void)position;
	return (NIL);
}

int		count_params(char *complete_cmd, size_t *i, size_t *tok_count)
{
	size_t	j;
	int		count;
	int		in_op;
	int		in_word;

	if (!complete_cmd)
		return (NIL);
	count = 0;
	in_op = 0;
	in_word = FALSE;
	while (!NONE(complete_cmd[*i]))
	{
		if (in_op && !escaped(complete_cmd, *i) && can_form_op(complete_cmd, in_op, i, END))
			*i += 1;
		else if (in_op && !can_form_op(complete_cmd, in_op, i, END))
			in_op = 0;
		else if (!escaped(complete_cmd, *i) && (*complete_cmd == '\\'
			|| *complete_cmd == '\''
			|| *complete_cmd == '"'))
		{
			if (NONE((j = find_next(*complete_cmd, &complete_cmd[1], &j))))
				return (ERROR);
			*i += j;
		}
		else if (!escaped(complete_cmd, *i) && (*complete_cmd == '$' || *complete_cmd == '`'))
		{
			if (NONE((j = find_sub_end(*complete_cmd, &complete_cmd[1], &j))))
				return (ERROR);
			*i += j;
		}
		else if (can_form_op(complete_cmd, in_op, i, BEGIN))
		{
			i += 1;
			in_op += 1;
		}
		else if (*complete_cmd == ' ' || *complete_cmd == '\n')
		{
			i += 1;
			in_word = FALSE;
		}
		else if (in_word)
			i += 1;
		else if (*complete_cmd == '#')
		{
			if (NONE((j = find_sub_end(*complete_cmd, &complete_cmd[1], &j))))
				return (ERROR);
			*i += j;
		}
		else
			count += 1;
	}
	if (NONE(complete_cmd))
		count += 1;
	*tok_count = count;
	return (SUCCESS);
}
