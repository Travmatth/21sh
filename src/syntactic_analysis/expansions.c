/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/19 15:34:51 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/19 15:35:26 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		remove_slash(char elem, size_t i, char *str, int *stop)
{
	int		out;

	(void)stop;
	out = 1;
	if (elem == '\\')
		out = IS_SEP(str[i + 1]) ? 0 : 1;
	return (out);
}

void	expand_command(char **command)
{
	char	*var;
	char	*tmp;
	char	*cmd;

	while (ft_strchr(*command, '~'))
	{
		var = get_env_var("HOME");
		tmp = ft_swap(*command, "~", var);
		free(*command);
		*command = tmp;
	}
	while ((tmp = ft_strchr(*command, '$')))
	{
		cmd = ft_strchr(tmp, ' ') ? ft_strchr(tmp, ' ') : ft_strchr(tmp, '\0');
		tmp = ft_strsub(tmp, 0, cmd - tmp);
		var = get_env_var(tmp + 1);
		cmd = ft_swap(*command, tmp, var ? var : "");
		free(*command);
		free(tmp);
		*command = cmd;
	}
}

/*
** manage escaped characters in given arg
** turn \n && \t -> <newline> && <tab> and strip other leading \
*/

int		normalize_tokens(char **args)
{
	(void)args;
	return (NIL);
}
