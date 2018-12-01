/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:43:52 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/30 12:25:10 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

char	*get_env_var(char *var)
{
	int		i;
	size_t	len;

	i = -1;
	len = LEN(var, 0);
	while (g_environ[++i])
	{
		if (ft_strnequ(g_environ[i], var, len))
			return (ft_strchr(g_environ[i], '=') + 1);
	}
	return (NULL);
}
