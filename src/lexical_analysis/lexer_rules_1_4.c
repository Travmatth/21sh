/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_rules_1_4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/27 12:44:27 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 16:38:56 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

extern t_list	*g_missing;

/*
** 1. If the end of input is recognized the current token shall be delimited.
** If there is no current token, the end-of-input indicator shall be returned as
** the token.
*/

void	rule_1(t_token *token, t_lctx *ctx, t_list **tokens)
{
	t_list	*node;

	if (!token->type)
	{
		create_new_tok(token, ctx, EOI);
		append_to_tok('\0', token);
		ctx->stop = TRUE;
	}
	node = NULL;
	ctx->status = push_token(token, node, tokens, ctx);
}

/*
** 2. If the previous character was used as part of an operator and the current
** character is not quoted and can be used with the current characters to form
** an operator, it shall be used as part of that (operator) token.
*/

int		rule_2(char c, t_token *token, t_lctx *ctx)
{
	int		next;

	if (!OP_DFA_ACCEPTING((next = next_op_state(c, ctx->op_state))))
		return (ctx->status = NIL);
	if (!(ft_bufappend(token->value, &c, sizeof(char))))
		return (ctx->status = ERROR);
	ctx->op_state = next;
	token->type = next;
	ctx->status = SUCCESS;
	return (SUCCESS);
}

/*
** 3. If the previous character was used as part of an operator and the current
** character cannot be used with the current characters to form an operator,
** the operator containing the previous character shall be delimited.
*/

void	rule_3(t_token *token, t_list **tokens, t_lctx *ctx)
{
	t_list	*node;

	node = NULL;
	if (!ERR(ctx->status)
		&& OK((ctx->status = push_token(token, node, tokens, ctx))))
		ctx->op_state = START;
}

/*
** 4. If the current character is backslash, single-quote, or double-quote
** ( '\', '", or ' )' and it is not quoted, it shall affect quoting for
** subsequent characters up to the end of the quoted text. The rules for
** quoting are as described in Quoting. During token recognition no
** substitutions shall be actually performed, and the result token shall
** contain exactly the characters that appear in the input (except for
** <newline> joining), unmodified, including any embedded or enclosing
** quotes or substitution operators, between the quote mark and the end of the
** quoted text. The token shall not be delimited by the end of the quoted field.
*/

void	rule_4(char *input, t_token *token, t_lctx *ctx)
{
	size_t	skip;

	skip = ERROR;
	if (NONE(token->type) && ERR(create_new_tok(token, ctx, LEXER_WORD)))
		ctx->status = ERROR;
	if (OK(ctx->status) && BACKSLASH(ctx->input, ctx->i))
		ctx->status = backslash(&ctx->input, ctx->i, &skip, lex_quote);
	else if (OK(ctx->status) && DBL_QUOTE(ctx->input, ctx->i))
		ctx->status = dbl_quote(&ctx->input, ctx->i, &skip, lex_quote);
	else if (OK(ctx->status) && SNGL_QUOTE(ctx->input, ctx->i))
		ctx->status = quote(&ctx->input, ctx->i, &skip, lex_quote);
	if (OK(ctx->status))
	{
		ft_bufappend(token->value, &input[ctx->i], skip + 1);
		ctx->i += skip + 1;
		ctx->status = SUCCESS;
	}
	else if (NONE(ctx->status))
	{
		ft_lstmerge(&ctx->missing, g_missing);
		g_missing = NULL;
	}
}
