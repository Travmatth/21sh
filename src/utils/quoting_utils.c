/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoting_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 15:31:44 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/29 13:38:32 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		init_exp(size_t *i, size_t start, int *found)
{
	*i = start;
	*found = FALSE;
	return (SUCCESS);
}

int		dbl_quote_switch(size_t *end, int *found, size_t len)
{
	*found = TRUE;
	*end = len;
	return (NIL);
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
		return (SUCCESS);
	}
	return (NIL);
}
