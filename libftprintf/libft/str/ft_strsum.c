/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsum.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/08 17:28:56 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/08 20:07:20 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
** ft_strsum - concatenate a null-delimited array of char pointers
**
** DESCRIPTION
** REWRITE
** The strcat() and strncat() functions append a copy of the null-terminated
** string s2 to the end of the null-terminated string s1, then add a termi-
** nating `\0'.  The string s1 must have sufficient space to hold the
** result.
**
** The source and destination strings should not overlap, as the behavior is
** undefined.
**
** RETURN VALUES
** The strcat() and strncat() functions return the pointer s1.
** REWRITE
*/

char	*free_str(char *str)
{
	free(str);
	return (NULL);
}

char	*ft_strsum(char **strings, char *fill)
{
	int		i;
	int		size;
	char	*str;
	char	*tmp;

	size = 0;
	while (strings[size])
		size += 1;
	i = 0;
	tmp = NULL;
	if (!size || !(str = ft_strdup(strings[i++])))
		return (NULL);
	while (i < size)
	{
		if (!(tmp = ft_strjoin(str, fill)))
			return (free_str(str));
		free(str);
		if (!(str = ft_strjoin(tmp, strings[i])))
			return (free_str(tmp));
		free(tmp);
		i += 1;
	}
	return (str);
}
