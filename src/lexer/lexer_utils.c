/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 14:37:15 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/04 12:52:02 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** TO BE DONE FINISHED
** remove escape characters in given arg
*/

int		remove_slash(char elem, size_t i, char *str, int *stop)
{
	int		out;

	(void)stop;
	out = 1;
	if (elem == '\\')
		out = IS_SEP(str[i + 1]) ? 0 : 1;
	return (out);
}

/*
** TO BE DONE FINISHED
** manage escaped characters in given arg
*/

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
*/

int		normalize_tokens(char **args)
{
	(void)args;
	return (NIL);
}

/*
** find next whitespace in complete_command not preceded by a backslash
*/

int		find_ws(char *cmd, size_t *offset)
{
	size_t	i;

	i = 1;
	while (cmd[i])
	{
		if (IS_WS(cmd[i]))
		{
			*offset += i;
			return (SUCCESS);
		}
		i += 1;
	}
	return (ERROR);
}

/*
** find next character c in complete_command not preceded by a backslash
*/

int		find_next(char c, char *cmd, size_t *offset)
{
	size_t	i;
	char	t;

	i = 0;
	while (cmd && cmd[1])
	{
		t = *cmd;
		if (*cmd == '\\' && cmd[1] == c)
		{
			if (NONE(find_next(c, &cmd[2], offset)))
				return (NIL);
			*offset += i + 1;
			return (SUCCESS);
		}
		else if (*cmd == c && i != 0)
		{
			*offset += i;
			return (SUCCESS);
		}
		i += 1;
		cmd += 1;
	}
	if (cmd && *cmd == c && i != 0)
	{
		*offset += i;
		return (SUCCESS);
	}
	return (NIL);
}
