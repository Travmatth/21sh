/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:48:07 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/04 12:51:53 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

/*
** Tilde Expansion
** A "tilde-prefix" consists of an unquoted <tilde> character at the beginning
** of a word, followed by all of the characters preceding the first unquoted
** <slash> in the word, or all the characters in the word if there is no
** <slash> ... The tilde-prefix is replaced by the value of the variable HOME.
** The pathname resulting from tilde expansion shall be treated as if quoted to
** prevent it being altered by field splitting and pathname expansion.
*/

int		tilde_expansion(char **parameter)
{
	char	*str;
	char	*var;
	char	*tmp;

	if (TILDE_PREFIX(*parameter) && !(str = ft_quotestr(ft_strdup(*parameter))))
		return (ERROR);
	else if (TILDE_PREFIX(*parameter))
	{
		if (!(var = get_env_var("HOME"))
			|| !(tmp = ft_swap(str, "~", var)))
			return (NIL);
		free(str);
		str = tmp;
	}
	else if (!TILDE_PREFIX(*parameter) && !(str = ft_strdup(*parameter)))
		return (ERROR);
	free(*parameter);
	*parameter = str;
	return (SUCCESS);
}
