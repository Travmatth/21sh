/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:43:52 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 21:42:18 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	*get_env_var(char *var)
{
	int		i;
	size_t	len;

	i = -1;
	len = LEN(var, 0);
	while (g_environ[++i])
	{
		if (ft_strnequ(g_environ[i], var, len) && g_environ[i][len] == '=')
			return (ft_strchr(g_environ[i], '=') + 1);
	}
	return (NULL);
}

int		builtin_env(int argc, char **argv)
{
	int		i;

	(void)argc;
	(void)argv;
	i = 0;
	while (g_environ[i])
		ft_printf("%s\n", g_environ[i++]);
	return (NORMAL_CHILD_EXIT);
}
