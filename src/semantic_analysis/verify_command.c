/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verify_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 14:21:56 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/06 16:37:56 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

t_builtin	g_builtins[] =
{
	{"echo", builtin_echo, 4},
	{"cd", builtin_cd, 2},
	{"setenv", builtin_setenv, 6},
	{"unsetenv", builtin_unsetenv, 8},
	{"env", builtin_env, 3},
};

int		find_exec(char *command)
{
	int			result;
	int			status;
	struct stat	attribs;

	status = stat(command, &attribs);
	result = access(command, X_OK);
	if (!result && S_ISREG(attribs.st_mode))
		return (1);
	if (!status && S_ISREG(attribs.st_mode))
	{
		if (~(attribs.st_mode & S_IXUSR))
			return (-1);
	}
	return (0);
}

int		find_command(char **command, char **paths, int i, int found)
{
	int		result;
	char	*tmp;
	char	*full_name;

	if (ft_strequ(".", *command) || ft_strequ("..", *command))
		return (0);
	if ((result = find_exec(*command)) == -1 || result == 1)
		return (result);
	while (!found && --i >= 0)
	{
		tmp = ft_strjoin(paths[i], "/");
		full_name = ft_strjoin(tmp, *command);
		free(tmp);
		if ((result = find_exec(full_name)) == -1)
			found = 1;
		else if (result == 1)
		{
			found = 1;
			free(*command);
			*command = full_name;
		}
		else
			free(full_name);
	}
	return (result);
}

int		is_builtin(char *command)
{
	int		i;

	i = 0;
	while (i < 5)
	{
		if (ft_strnequ(g_builtins[i].cmd, command, g_builtins[i].len))
			return (TRUE);
		i += 1;
	}
	return (FALSE);
}

int		verify_command(char **command)
{
	int		j;
	int		found;
	char	**paths;
	int		status;

	found = 0;
	if (is_builtin(*command))
		return (BUILTIN);
	if (!(paths = ft_strsplit(get_env_var("PATH"), ':')))
		return (ERROR);
	j = 0;
	while (paths && paths[j])
		j += 1;
	if (ERR((status = find_command(command, paths, j, found))))
		ft_printf("sh: permission denied: %s\n", *command);
	else if (NONE(status))
		ft_printf("sh: command not found: %s\n", *command);
	ft_freearr(paths);
	return (status);
}
