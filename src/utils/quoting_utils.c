/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoting_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 15:31:44 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/25 15:38:42 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		dbl_quote_switch(size_t *end, int *found, size_t len)
{
	*found = TRUE;
	*end = len;
	return (NIL);
}

size_t	*exp_p(char **str, size_t start, int *found, size_t *end)
{
	size_t	i;

	i = 1;
	while (IS_VAR_CHAR((*str)[i]) && IS_VAR_CHAR((*str)[i + 1]))
		i += 1;
	*found = TRUE;
	*end = i - start;
	return (end);
}

int		param_exp_end(size_t *end, size_t len)
{
	*end = len;
	return (SUCCESS);
}

int		arith_exp_switch(size_t *end, size_t len, int *found)
{
	*end = len;
	*found = TRUE;
	return (NIL);
}

int		switch_arith_exp(char **str, size_t i, size_t *count, int *found)
{
	if ((*str)[i] == '(')
		*count += 1;
	else if (*count && (*str)[i] == ')')
		*count -= 1;
	else if (TWO_CLOSING((*str), i))
	{
		*found = TRUE;
		return (NIL);
	}
	return (SUCCESS);
}
