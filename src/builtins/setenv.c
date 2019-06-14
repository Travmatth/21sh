/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 21:11:08 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/13 19:37:12 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		env_usage(void)
{
	ft_putendl("setenv usage: `setenv NAME=VALUE` or `setenv NAME VALUE`");
	return (ERROR_CHILD_EXIT);
}

int		add_env_var(char *name, char *val)
{
	int		i;
	char	**tmp;
	char	*new;

	i = 0;
	while (g_environ[i])
		i += 1;
	if (!(tmp = (char**)ft_memalloc(sizeof(char*) * (i + 2))))
		return (ERROR_CHILD_EXIT);
	new = ft_strnew(LEN(name, 0) + LEN(val, 0) + 1);
	tmp[i] = ft_strcat(ft_strcat(ft_strcat(new, name), "="), val);
	while (--i >= 0)
		tmp[i] = g_environ[i];
	free(g_environ);
	g_environ = tmp;
	return (NORMAL_CHILD_EXIT);
}

int		set_env_var(char *name, char *val)
{
	int		i;
	char	*tmp;
	char	*old_val;

	if ((!name || !name[0] || !val) && env_usage())
		return (ERROR_CHILD_EXIT);
	i = 0;
	while (g_environ[i])
	{
		if (ft_strnequ(name, g_environ[i], LEN(name, 0)))
		{
			old_val = ft_strchr(g_environ[i], '=') + 1;
			tmp = ft_swap(g_environ[i], old_val, val);
			free(g_environ[i]);
			g_environ[i] = tmp;
			return (NORMAL_CHILD_EXIT);
		}
		else if (!g_environ[i + 1])
			return (add_env_var(name, val));
		i += 1;
	}
	return (ERROR_CHILD_EXIT);
}

int		set_env(char **argv)
{
	char	*name;
	char	*value;

	if (argv[2])
	{
		name = argv[1];
		value = argv[2];
	}
	else if ((value = ft_strchr(argv[1], '=')))
	{
		name = argv[1];
		value[0] = '\0';
		value += 1;
	}
	else
	{
		name = argv[1];
		value = "";
	}
	return (set_env_var(name, value));
}

int		builtin_setenv(int argc, char **argv)
{
	int		status;

	status = NORMAL_CHILD_EXIT;
	if (!argv[1])
		status = builtin_env(argc, argv);
	else if (argv[2] && argv[3])
		status = env_usage();
	else if (argv[1])
		status = set_env(argv);
	return (status);
}
