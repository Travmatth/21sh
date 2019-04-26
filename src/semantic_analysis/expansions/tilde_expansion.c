/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 12:48:07 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/15 18:02:38 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/shell.h"

/*
** Create copy of given string, less the beginning and ending characters
*/

char	*ft_stripquote(char *str)
{
	size_t	len;
	char	*new;

	len = LEN(str, 0);
	new = ft_strnew(len - 2);
	ft_memcpy(new, &str[1], len - 2);
	return (new);
}

/*
** Enclosed given string with specified quote character
*/

char	*ft_quotestr(char *str, char quote)
{
	size_t	len;
	char	*new;

	if (!str)
		return (NULL);
	len = LEN(str, 0);
	if (!(new = ft_strnew(len + 2)))
		return (NULL);
	new[0] = quote;
	return (ft_strncat(ft_strcat(new, str), &quote, 1));
}

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
