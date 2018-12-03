/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_delimiters.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 14:37:15 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/03 01:11:41 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** end op token context and record token
*/

void	delimit_op_token(t_lexctx *ctx, char **tokens, char *cmd, int record)
{
	char	*tmp;

	if (record)
	{
		if (!(tmp =
			ft_strsub(cmd, ctx->i - ctx->in_op - 1, ctx->in_op + 1)))
			ctx->err = TRUE;
		tokens[ctx->count - 1] = tmp;
	}
	ctx->in_op = 0;
}

/*
** end param token, record token
*/

void	delimit_param_token(t_lexctx *ctx, char **tokens, char *cmd, int record)
{
	if (NONE((ctx->j = find_sub_end(cmd[ctx->i], &cmd[1], &ctx->j))))
		ctx->err = TRUE;
	if (record)
	{
		if (!(tokens[ctx->count - 1] = ft_strsub(cmd, ctx->i + 1, ctx->j)))
			ctx->err = TRUE;
	}
	ctx->i += ctx->j;
	ctx->count += 1;
}

/*
** end and record token
*/

void	delimit_token(t_lexctx *ctx, char **tokens, char *cmd, int record)
{
	if (record)
	{
		if (ctx->in_word)
		{
			if (!(tokens[ctx->count - 1] = ft_strsub(cmd,
				ctx->i - ctx->in_word, ctx->in_word)))
				ctx->err = TRUE;
		}
		else if (ctx->in_op)
		{
			if (!(tokens[ctx->count - 1] = ft_strsub(cmd
				, ctx->i - ctx->in_op, ctx->in_op)))
				ctx->err = TRUE;
		}
	}
	ctx->i += 1;
	ctx->in_op = 0;
	ctx->in_word = 0;
}

/*
** end and record comment token
*/

void	delimit_comment_token(t_lexctx *ctx, char **tokens, char *cmd, int record)
{
	(void)tokens;
	(void)cmd;
	if (record)
	{
		if (NONE((ctx->j = find_sub_end(cmd[ctx->i], &cmd[1], &ctx->j))))
			ctx->err = TRUE;
	}
	ctx->i  += ctx->j + 1;
}
