/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/18 17:57:05 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 21:41:44 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		builtin_exec(int argc, char **argv)
{
	if (argc)
	{
		execve(argv[0], argv, g_environ);
		return (ERROR_CHILD_EXIT);
	}
	return (NORMAL_CHILD_EXIT);
}
