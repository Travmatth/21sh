/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 14:37:15 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/27 13:06:47 by tmatthew         ###   ########.fr       */
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
			i += 1;
			cmd += 1;
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

/*
** find the next token in the complete_command
*/

int		escaped(char *input, size_t i)
{
	if (i == 0)
		return (FALSE);
	else if (input[i - 1] == '\\')
		return (TRUE);
	return (FALSE);
}

int		init_lexer_ctx(char *input, t_lctx *ctx)
{
	if (!input)
		return (ERROR);
	ctx->input = input;
	ctx->status = NIL;
	ctx->i = 0;
	ctx->j = 0;
	ctx->stop = 0;
	ctx->op_state = 0;
	ctx->in_word = 0;
	ctx->missing = NIL;
	return (SUCCESS);
}

int		create_new_tok(char c, t_token *token, t_lctx *ctx, int type)
{
	char	*buf;

	token->type = type;
	if (type == WORD)
		ctx->in_word = TRUE;
	if (!(buf = ft_memalloc(sizeof(char) * (type == WORD ? 10 : 4))))
		return (ERROR);
	ft_memcpy(buf, &c, sizeof(char));
	if (!(token->value = ft_bufnew(buf, sizeof(char), sizeof(char) * 10)))
		return (ERROR);
	return (SUCCESS);
}

int		push_token(t_token *token, t_list *node, t_list **tokens, t_lctx *ctx)
{
	if (!(node = ft_lstnew(token, sizeof(t_token))))
		return (ERROR);
	ft_lstpushback(tokens, node);
	ctx->in_word = FALSE;
	ctx->op_state = FALSE;
	token->type = NIL;
	token->value = NULL;
	return (SUCCESS);
}
