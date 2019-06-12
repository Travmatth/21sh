/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 21:09:37 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 22:04:44 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	*save_cwd(char *target_dir, char **path, char ***dirs)
{
	char	buf[256];
	char	*tmp;

	(target_dir[0] == '~' || ft_strequ("--", target_dir))
		? (*path = ft_swap(target_dir, "~", get_env_var("HOME")))
		: (*path = target_dir);
	(ft_strequ("-", target_dir))
		? (*path = ft_swap(target_dir, "-", get_env_var("OLDPWD")))
		: (NULL);
	tmp = *path;
	if (!(*dirs = ft_strsplit(*path, '/')))
		return (NULL);
	if (ft_strnequ("/", tmp, 1))
	{
		tmp = ft_strjoin("/", *dirs[0]);
		free(*dirs[0]);
		*dirs[0] = tmp;
	}
	ft_bzero((void*)buf, 256);
	getcwd(buf, 256);
	return (ft_strdup(buf));
}

int		update_path_vars(char *old)
{
	char	new[256];

	if (set_env_var("OLDPWD", old))
		return (ERROR_CHILD_EXIT);
	free(old);
	ft_bzero((void*)new, 256);
	getcwd(new, 256);
	if (set_env_var("PWD", new))
		return (ERROR_CHILD_EXIT);
	ft_printf("%s\n", new);
	return (NORMAL_CHILD_EXIT);
}

int		change_next_dir(char *dir, char **dirs, char *path, int i)
{
	struct stat	attribs;

	if (!chdir(dir))
		return (NORMAL_CHILD_EXIT);
	else if (access(dir, F_OK) == -1)
		ft_printf("cd: no such file or directory: %s\n", path);
	else if (access(dir, R_OK) == -1)
		ft_printf("cd: permission denied: %s\n", path);
	else if (!stat(path, &attribs) && !S_ISDIR(attribs.st_mode))
		ft_printf("not a directory: %s\n", path);
	else if (!dirs[i])
		ft_printf("%s\n", path);
	return (ERROR_CHILD_EXIT);
}

int		change_dir(char *target_dir, int i)
{
	char		*path;
	char		**dirs;
	char		*dir;
	char		*old;
	int			status;

	old = save_cwd(target_dir, &path, &dirs);
	while ((dir = dirs[i++]))
	{
		if ((status = change_next_dir(dir, dirs, path, i)))
			continue ;
		else
			break ;
	}
	if (!dirs[(i ? i : 1) - 1] && !status && update_path_vars(old))
		return (ERROR_CHILD_EXIT);
	while (--i >= 0)
		free(dirs[i]);
	free(dirs);
	if (!ft_strequ(path, target_dir))
		free(path);
	return (status);
}

int		builtin_cd(int argc, char **argv)
{
	(void)argc;
	if (argv[1] && argv[2])
	{
		ft_printf("cd: string not in pwd: %s", argv[1]);
		return (ERROR_CHILD_EXIT);
	}
	else if (!argv[1])
		return (change_dir("~", 0));
	else if (argv[1] && argv[1][0] && ft_strcmp(".", argv[1]))
		return (change_dir(argv[1], 0));
	return (ERROR_CHILD_EXIT);
}
