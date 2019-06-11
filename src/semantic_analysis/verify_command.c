/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verify_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 14:21:56 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/30 17:38:58 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Map given command to its appropriate funciton pointer
*/

t_builtin	g_builtins[] =
{
	{"echo", builtin_echo},
	{"cd", builtin_cd},
	{"setenv", builtin_setenv},
	{"unsetenv", builtin_unsetenv},
	{"env", builtin_env},
	{"exec", builtin_exec}
};

int		load_exec(t_simple *simple)
{
	char	**new;

	simple->is_exec = TRUE;
	if (!--simple->argc)
		new = NULL;
	else if (!(new = ft_memalloc(sizeof(char*) * simple->argc)))
		return (ERROR);
	if (new)
		ft_memcpy(new, &simple->command[1], sizeof(char*) * simple->argc);
	free(simple->command);
	simple->command = new;
	return (SUCCESS);
}

/*
** Determine whether given command is a shell builtin function
*/

int		load_builtin(t_simple *simple)
{
	int		i;
	int		status;

	i = -1;
	status = FALSE;
	while (simple->command[simple->argc])
		simple->argc += 1;
	while (++i < TOTAL_BUILTINS)
	{
		if (IS_A(g_builtins[i].cmd, simple->command[0]))
		{
			if (IS_A("exec", g_builtins[i].cmd))
				status = load_exec(simple);
			else
				status = TRUE;
			simple->builtin = g_builtins[i].f;
			break ;
		}
	}
	return (status);
}

/*
** Determine whether given command (and its full path) is valid file & correct
** execution rights are set
*/

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

/*
** Iterate through given paths and construct a full pathname that can be tested
** for validity. If given full pathname is valid, rewrites command, otherwise
** returns whether command is invalid or unable to be executed
*/

int		find_command(char **command, char **paths, int i, int found)
{
	int		result;
	char	*tmp;
	char	*full_name;

	if (ft_strequ(".", *command) || ft_strequ("..", *command))
		return (NIL);
	if (!NONE((result = find_exec(*command))))
		return (result);
	while (!found && --i >= 0)
	{
		tmp = ft_strjoin(paths[i], "/");
		full_name = ft_strjoin(tmp, *command);
		free(tmp);
		if (ERR((result = find_exec(full_name))))
			found = 1;
		else if (OK(result))
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

/*
** Determine whether given command array begins with valid command. If command
** is builtin, return this information, otherwise determine whether command is
** contained within directories specified in the current $PATH. If command is
** found, rewrite command to include full path of file.
*/

int		verify_command(t_simple *simple)
{
	int		j;
	int		found;
	char	**paths;
	int		status;

	found = 0;
	status = load_builtin(simple);
	if (!NONE(status) || !simple->argc)
		return (OK(SUCCESS) ? SUCCESS : ERROR);
	if (!(paths = ft_strsplit(get_env_var("PATH"), ':')))
		return (ERROR);
	j = 0;
	while (paths && paths[j])
		j += 1;
	if (ERR((status = find_command(simple->command, paths, j, found))))
		ft_printf("sh: permission denied: %s\n", simple->command[0]);
	else if (NONE(status))
		ft_printf("sh: command not found: %s\n", simple->command[0]);
	ft_freearr(paths, TRUE);
	return (status);
}
