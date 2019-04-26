/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:48:07 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/26 15:25:22 by tmatthew         ###   ########.fr       */
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
	char	*tmp;
	char	*var;

	if (TILDE_PREFIX(*parameter))
	{
		var = get_env_var("HOME");
		if (!var)
			var = "";
		if (!(var = ft_quotestr(var, '"'))
			|| !(tmp = ft_swap(*parameter, "~", var)))
			return (ERROR);
	}
	else if (!(tmp = ft_strdup(*parameter)))
		return (ERROR);
	free(*parameter);
	*parameter = tmp;
	return (SUCCESS);
}
