/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 14:37:15 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/11 15:23:05 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** 11. The current character is used as the start of a new word.
*/

void	rule_11(char c, t_lctx *ctx, t_token *token)
{
	ctx->status = create_new_tok(token, ctx, LEXER_WORD);
	if (OK(ctx->status))
		ctx->status = append_to_tok(c, token);
}

void	lex_switch(char c, t_token *token, t_list **tokens, t_lctx *ctx)
{
	if (!c)
		rule_1(token, ctx, tokens);
	else if (ctx->op_state && !escaped(ctx->input, ctx->i)
		&& rule_2(c, token, ctx))
		ctx->i += 1;
	else if (ctx->op_state)
		rule_3(token, tokens, ctx);
	else if (!escaped(ctx->input, ctx->i)
		&& (c == '\\' || c == '\'' || c == '"'))
		rule_4(c, ctx->input, token, ctx);
	else if (!escaped(ctx->input, ctx->i) && IS_QUOTED(c, ctx))
		rule_5(c, token, ctx);
	else if (can_form_op(c))
		rule_6(c, token, tokens, ctx);
	else if (c == '\n' && !escaped(ctx->input, ctx->i))
		rule_7(token, tokens, ctx);
	else if ((c == ' ' || c == '\t') && !escaped(ctx->input, ctx->i))
		rule_8(token, tokens, ctx);
	else if (ctx->in_word)
		rule_9(c, token, tokens, ctx);
	else if (c == '#')
		rule_10(ctx);
	else if ((ctx->i += 1))
		rule_11(c, ctx, token);
}

/*
** -exec p (char*)((t_token*)(*tokens)->content)->value->buf
*/

int		lexical_analysis(char *input, t_list **tokens, t_list **missing)
{
	char	c;
	t_lctx	ctx;
	t_token	token;

	(void)missing;
	if (!init_lexer_ctx(input, &ctx, &token))
		return (NIL);
	while (!ctx.stop && OK(ctx.status))
	{
		c = input[ctx.i];
		lex_switch(c, &token, tokens, &ctx);
	}
	if (NONE(ctx.status))
		*missing = ctx.missing;
	return (ctx.status);
}
