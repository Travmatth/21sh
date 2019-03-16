/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 14:40:36 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/14 15:51:31 by tmatthew         ###   ########.fr       */
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

/*
** need to write a wrapper function that deals with translating jobs
** into series of commands to fork/execve/waitpid
*/

int		execute_command(char **av)
{
	int		status;
	pid_t	pid;

	status = ERROR;
	if (NONE((pid = fork())))
	{
		execve(av[0], av, g_environ);
		ft_printf("fork error: %s", av[0]);
		_exit(1);
	}
	else if (OK(pid))
		waitpid(pid, &status, NIL);
	else if (ERR(pid))
		ft_printf("fork error: %s", av[0]);
	return (WEXITSTATUS(status));
}

int		execute_cmd(char **command)
{
	int		j;
	int		found;
	char	**paths;
	int		result;
	int		status;

	found = 0;
	paths = ft_strsplit(get_env_var("PATH"), ':');
	j = 0;
	status = ERROR;
	while (paths && paths[j])
		j += 1;
	result = find_command(&command[0], paths, j, found);
	if (ERR(result))
		ft_printf("sh: permission denied: %s\n", command[0]);
	else if (OK(result))
		status = execute_command(command);
	else
		ft_printf("sh: command not found: %s\n", command[0]);
	ft_freearr(paths);
	return (status);
}

/*
** Shell grammar parsing steps
** 2: break input into tokens: words and operators
** 3: parse input into simple commands and compound commands
** 4: perform expansions
** 5: register redirections and removes operators from parameter list
*/

int		builtin_command(char **argv)
{
	int		i;
	int		ac;

	i = 0;
	while (i < 5)
	{
		if (ft_strnequ(g_builtins[i].cmd, argv[0], g_builtins[i].len))
		{
			ac = 0;
			while (argv[ac])
				ac += 1;
			i = g_builtins[i].f(ac, argv);
			ac -= 1;
			return (i);
		}
		i += 1;
	}
	return (0);
}

/*
** 6: execute function
** 7: optionally wait for command to complete and collect exit status
*/

int		execute_commands(char *complete_cmd)
{
	int		status;
	t_ast	ast;

	ft_bzero(&ast, sizeof(t_ast));
	if (!OK((status = prepare_ast(complete_cmd, &ast))))
		return (status);
	return (traverse_ast(&ast));
}
