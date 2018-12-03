/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_funcs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 14:37:15 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/02 23:47:56 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** record quote context
*/

void	form_quoted_token(t_lexctx *ctx, char **tokens, char *cmd, int record)
{
	if (NONE((find_next(cmd[ctx->i], &cmd[ctx->i], &ctx->j))))
		ctx->err = TRUE;
	if (record)
	{
		if (!(tokens[ctx->count] = ft_strsub(cmd, ctx->i , ctx->j)))
			ctx->err = TRUE;
	}
	ctx->i += ctx->j + 1;
	ctx->count += 1;
}

/*
** start param token context
*/

void	form_op_token(t_lexctx *ctx, char **tokens, char *cmd, int record)
{
	if (record)
	{
		if (ctx->in_word)
		{
			if (!(tokens[ctx->count - 1] = ft_strsub(cmd
				, ctx->i - ctx->in_word, ctx->in_word)))
				ctx->err = TRUE;
		}
	}
	ctx->in_word = 0;
	ctx->count += 1;
	ctx->i += 1;
	ctx->in_op += 1;
}

/*
** set lexing context to new token
*/

void	continue_token(t_lexctx *ctx, char **tokens, char *cmd, int record)
{
	(void)cmd;
	(void)record;
	(void)tokens;
	ctx->in_word += 1;
	ctx->i += 1;
}

/*
** set lexing context to new token
*/

void	form_token(t_lexctx *ctx, char **tokens, char *cmd)
{
	(void)tokens;
	(void)cmd;
	ctx->in_word += 1;
	ctx->count += 1;
	ctx->i += 1;
}
