/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 14:37:15 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 22:45:25 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Set initial state of lexical state struct
*/

int		init_lexer_ctx(char *input, t_lctx *ctx, t_token *token)
{
	if (!input)
		return (ERROR);
	ctx->input = input;
	ctx->status = SUCCESS;
	ctx->i = 0;
	ctx->j = 0;
	ctx->stop = 0;
	ctx->op_state = 0;
	ctx->in_word = 0;
	ctx->missing = NULL;
	token->type = NIL;
	token->value = NULL;
	return (SUCCESS);
}

/*
** Lexes the given character in accordance with the rules
** given in section 2.3, Token Recognition
*/

void	lex_switch(char c, t_token *token, t_list **tokens, t_lctx *ctx)
{
	if (!c)
		rule_1(token, ctx, tokens);
	else if (ctx->op_state && !escaped(ctx->input, ctx->i)
		&& rule_2(c, token, ctx))
		ctx->i += 1;
	else if (ctx->op_state)
		rule_3(token, tokens, ctx);
	else if (QUOTE_CHAR(c) && !escaped(ctx->input, ctx->i))
		rule_4(ctx->input, token, ctx);
	else if (IS_QUOTED(c, ctx) && !escaped(ctx->input, ctx->i))
		rule_5(c, token, ctx);
	else if (can_form_op(c))
		rule_6(c, token, tokens, ctx);
	else if (c == '\n' && !escaped(ctx->input, ctx->i))
		rule_7(token, tokens, ctx);
	else if (c == ' ' && !escaped(ctx->input, ctx->i))
		rule_8(token, tokens, ctx);
	else if (ctx->in_word)
		rule_9(c, token, tokens, ctx);
	else if (c == '#')
		rule_10(ctx);
	else if ((ctx->i += 1))
		rule_11(c, ctx, token);
}

/*
** Lexical analysis is performed by in-order processing of a given inputted
** command. Command is processed from beginning to end, where each given
** character may alter the state of processing and be added to the current
** token, delimit the current token, start a new token, or denote an opening
** sequence of quote/command substitution characters that need to be included
** in the current token and skipped over during lexical processing. The
** end-of-string null character `\0' is processed twice, once to allow any
** current tokens to be delimited and finally to push the end-of-input to the
** list of tokens. Result is a list of tokens suitable for use in syntactic
** analysis
*/

int		lexical_analysis(char *input, t_list **tokens)
{
	char	c;
	t_lctx	ctx;
	t_token	token;

	if (!init_lexer_ctx(input, &ctx, &token))
		return (NIL);
	while (!ctx.stop && OK(ctx.status))
	{
		c = input[ctx.i];
		lex_switch(c, &token, tokens, &ctx);
	}
	return (ctx.status);
}
