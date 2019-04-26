/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 21:11:24 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/30 12:25:10 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	remove_var(char *var, int env_size, size_t len)
{
	int		j;
	int		i;
	char	**tmp;

	i = 0;
	j = 0;
	tmp = (char**)ft_memalloc(sizeof(char*) * env_size);
	while (g_environ[i])
	{
		if (ft_strnequ(g_environ[i], var, len))
			free(g_environ[i]);
		else
		{
			tmp[j] = g_environ[i];
			j += 1;
		}
		i += 1;
	}
	free(g_environ);
	g_environ = tmp;
}

void	unset_env(char *var)
{
	int		i;
	int		found;
	size_t	len;

	i = 0;
	found = 0;
	len = LEN(var, 0);
	while (g_environ[i])
	{
		if (ft_strnequ(g_environ[i++], var, len))
			found = 1;
	}
	if (found)
		remove_var(var, i, len);
}

int		builtin_unsetenv(int argc, char **argv)
{
	(void)argc;
	if (!argv[1] || argv[2] || ft_strchr(argv[1], '='))
		ft_putstr("unsetenv usage: `unsetenv NAME`");
	unset_env(argv[1]);
	return (1);
}
