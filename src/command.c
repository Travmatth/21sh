/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:40:36 by tmatthew          #+#    #+#             */
/*   Updated: 2018/11/30 14:49:33 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/shell.h"

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

/*
** need to write a wrapper function that deals with translating jobs
** into series of commands to fork/execve/waitpid
*/

void	execute_command(char **av)
{
	pid_t	pid;

	if (NONE((pid = fork())))
	{
		execve(av[0], av, g_environ);
		ft_printf("fork error: %s", av[0]);
		_exit(1);
	}
	else if (OK(pid))
		waitpid(pid, NULL, 0);
	else if (ERR(pid))
		ft_printf("fork error: %s", av[0]);
}

void	execute_cmd(char **command)
{
	int		j;
	int		found;
	char	**paths;
	int		result;

	found = 0;
	paths = ft_strsplit(get_env_var("PATH"), ':');
	j = 0;
	while (paths && paths[j])
		j += 1;
	result = find_command(&command[0], paths, j, found);
	if (ERR(result))
		ft_printf("sh: permission denied: %s\n", command[0]);
	else if (OK(result))
		execute_command(command);
	else
		ft_printf("sh: command not found: %s\n", command[0]);
	ft_freearr(paths);
}

/*
** 6: execute function
** 7: optionally wait for command to complete and collect exit status
*/

int		execute_commands(char *command)
{
	char	**commands;
	int		i;
	int		parsed;
	t_job	job;

	i = -1;
	commands = ft_strsplit(command, ';');
	while (commands[++i])
	{
		g_processes += 1;
		parsed = prepare_job(commands[i], &job, i);
		g_processes -= 1;
	}
	ft_freearr(commands);
	return (1);
}
