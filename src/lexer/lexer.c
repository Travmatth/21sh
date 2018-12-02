/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 14:37:15 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/01 16:07:16 by tmatthew         ###   ########.fr       */
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

char	**tokenize_commands(char **complete_cmd)
{
	(void)complete_cmd;
	return (NULL);
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

/*
** if open quote is detected
** prompt user for more input that will close the quote
*/

int		close_quote_prompt(char *complete_cmd)
{
	(void)complete_cmd;
	return (NIL);
}

/*
** find next whitespace in complete_command not preceded by a backslash
*/

int		find_ws(char *complete_cmd, size_t *offset)
{
	size_t	i;

	i = 1;
	while (complete_cmd[i])
	{
		if (IS_WS(complete_cmd[i]))
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

int		find_next(char c, char *complete_cmd, size_t *offset)
{
	size_t	i;

	i = 1;
	while (complete_cmd && complete_cmd[i])
	{
		if (*complete_cmd == c && i == 1)
		{
			*offset += i;
			return (SUCCESS);
		}
		else if (*complete_cmd == c && complete_cmd[i - 1] == '\\')
		{
			if (ERR(find_next(c, &complete_cmd[i], offset)))
				return (ERROR);
			*offset += i;
			return (SUCCESS);
		}
		complete_cmd += 1;
	}
	return (ERROR);
}

/*
** determine whether character c in complete_command
** is quote not preceded by a backslash
*/

int		is_quote(char *complete_cmd, size_t n)
{
	if (n == 0 && IS_WS(*complete_cmd))
		return (SUCCESS);
	else if (n > 0 && *(complete_cmd - 1) != '\\' && IS_WS(*complete_cmd))
		return (SUCCESS);
	return (NIL);
}

/*
** find the next token in the complete_command
*/

int		tokenize_switch(char *complete_cmd, int i, int n, size_t *offset)
{
	int		s;

	if (!complete_cmd[1])
		return (NIL);
	s = 0;
	if ((!is_quote(&complete_cmd[n], i) && (s = find_ws(complete_cmd, offset)))
		|| IS_SNGL(complete_cmd, s, &complete_cmd[n], offset)
		|| IS_DBL(complete_cmd, s, &complete_cmd[n], offset))
		return (SUCCESS);
	if (s)
		return (ERROR);
	return (NIL);
}

/*
** transform a given complete_command into an array of tokens to be parsed
*/

int		tokenize(char *complete_cmd, int arg_count, size_t len, char **args)
{
	size_t	offset;
	int		status;
	int		i;
	size_t	n;

	(void)len;
	n = 0;
	i = -1;
	offset = 0;
	if (!complete_cmd)
		return (NIL);
	while (complete_cmd[n] && ++i < arg_count)
	{
		while (IS_WS(complete_cmd[n]))
			complete_cmd += 1;
		if (NONE((status = tokenize_switch(complete_cmd, i, n, &offset))))
			break ;
		else if (ERR(status) && ERR(close_quote_prompt(&complete_cmd[n])))
			return (ERROR);
		args[i] = ft_strsub(complete_cmd, n, offset);
		n += offset;
	}
	return (SUCCESS);
}

int		lexer(char *complete_cmd, char ***tokens)
{
	int		tok_count;
	char	**toks;

	tok_count = count_params(complete_cmd);
	if (!OK(tok_count))
		return (ERR(tok_count) ? ERROR : NIL);
	else if (!(toks = (char**)ft_memalloc(sizeof(char*) * (tok_count + 1))))
		return (ERROR);
	else if (NONE(tokenize(complete_cmd, tok_count, LEN(complete_cmd, 0), toks))
		|| ERR(normalize_tokens(toks)))
		return (NIL);
	*tokens = toks;
	return (SUCCESS);
}
