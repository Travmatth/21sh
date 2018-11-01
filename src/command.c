/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:40:36 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/30 20:49:39 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		find_exec(char *command)
{
	int			result;
	struct stat	attribs;

	result = access(command, X_OK);
	if (!result)
		return (1);
	if (!stat(command, &attribs))
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

void	execute_command(char **av)
{
	pid_t	pid;

	if (!(pid = fork()))
		execve(av[0], av, g_environ);
	else if (pid > 0)
		waitpid(pid, NULL, 0);
	else if (pid < 0)
		ft_printf("fork error");
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
	if (result == -1)
		ft_printf("sh: permission denied: %s\n", command[0]);
	else if (result)
		execute_command(command);
	else
		ft_printf("sh: command not found: %s\n", command[0]);
	ft_freearr(paths);
}

int		execute_commands(char *command)
{
	char	**commands;
	char	**argv;
	int		i;
	int		parsed;

	i = -1;
	commands = ft_strsplit(command, ';');
	while (commands[++i])
	{
		g_processes += 1;
		parsed = prepare_command(commands, &argv, i);
		if (ERR(parsed))
			ft_putstr("sh: please balance parentheses\n");
		if (parsed && ft_strequ("exit", argv[0]))
			return (0);
		else if (parsed && builtin_command(argv))
			;
		else if (parsed)
			execute_cmd(argv);
		ft_freearr(argv);
		g_processes -= 1;
	}
	ft_freearr(commands);
	return (1);
}
