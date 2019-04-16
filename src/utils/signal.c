/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/19 19:33:17 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/15 18:30:44 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Intercept ctrl-c signals and exit processes
*/

void	child_sig_handler(int sig)
{
	if (sig != SIGCHLD)
		return ;
	g_processes -= 1;
	_exit(1);
}

/*
** Intercept ctrl-c signals and ignore in parent process
*/

void	sig_handler(int sig)
{
	if (sig != SIGINT)
		return ;
	if (g_processes <= 0)
		write(STDOUT, "\n$>", 3);
}
