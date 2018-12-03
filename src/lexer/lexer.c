/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 14:37:15 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/02 23:47:56 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** call the appropriate lexing step within given state and lexing ctx
*/

void	tokenize_switch(t_lexctx *ctx, char **tokens, char *cmd, int record)
{
	if (ctx->in_op && !escaped(cmd, ctx->i)
		&& can_form_op(cmd, ctx->in_op, &ctx->i, END)) ctx->i += 1;
	else if (ctx->in_op && !can_form_op(cmd, ctx->in_op, &ctx->i, END))
		delimit_op_token(ctx, tokens, cmd, record);
	else if (!escaped(cmd, ctx->i) && (cmd[ctx->i] == '\\'
		|| cmd[ctx->i] == '\''
		|| cmd[ctx->i] == '"'))
		form_quoted_token(ctx, tokens, cmd, record);
	else if (!escaped(cmd, ctx->i) && (cmd[ctx->i] == '$'
		|| cmd[ctx->i] == '`'))
		delimit_param_token(ctx, tokens, cmd, record);
	else if (can_form_op(cmd, ctx->in_op, &ctx->i, BEGIN))
		form_op_token(ctx, tokens, cmd, record);
	else if (IS_WS(cmd[ctx->i]))
		delimit_token(ctx, tokens, cmd, record);
	else if (ctx->in_word)
		continue_token(ctx, tokens, cmd, record);
	else if (cmd[ctx->i] == '#')
		delimit_comment_token(ctx, tokens, cmd, record);
	else
		form_token(ctx, tokens, cmd);
}

/*
** iterate through char *command, allocate into a series of char **tokens
*/

int		tokenize(char *cmd, char **tokens, int record, size_t *tok_count)
{
	t_lexctx	ctx;

	if (!cmd)
		return (NIL);
	ft_bzero(&ctx, sizeof(t_lexctx));
	while (!NONE(cmd[ctx.i]) && !ctx.err)
		tokenize_switch(&ctx, tokens, cmd, record);
	if (record && ctx.in_word && !(tokens[ctx.count - 1] =
		ft_strsub(cmd, ctx.i - ctx.in_word, ctx.in_word + 1)))
		return (ERROR);
	else if (record && ctx.in_op && !(tokens[ctx.count - 1] =
		ft_strsub(cmd, ctx.i - ctx.in_op, ctx.in_op + 1)))
		return (ERROR);
	*tok_count = ctx.count;
	return (SUCCESS);
}

/*
** if open quote is detected
** prompt user for more input that will close the quote
*/

int		close_quote_prompt(char *cmd, size_t i, size_t *tok_count)
{
	(void)cmd;
	(void)i;
	(void)tok_count;
	return (SUCCESS);
}

/*
** break a char *command into a series of char **tokens to be parsed
*/

int		lexer(char *cmd, char ***tokens)
{
	size_t	i;
	size_t	tok_count;
	char	**toks;

	i = 0;
	toks = NULL;
	tok_count = 0;
	if (NONE(tokenize(cmd, toks, FALSE, &tok_count)))
	{
		if (ERR((i = close_quote_prompt(cmd, i, &tok_count))))
			return (ERROR);
	}
	if (!OK(tok_count))
		return (NIL);
	else if (!(toks = (char**)ft_memalloc(sizeof(char*) * (tok_count + 1))))
		return (ERROR);
	else if (NONE(tokenize(cmd, toks, TRUE, &tok_count))
		|| ERR(normalize_tokens(toks)))
		return (NIL);
	*tokens = toks;
	return (SUCCESS);
}
