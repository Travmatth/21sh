/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_rules_5_8.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/27 12:45:22 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/28 13:36:45 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** 5: If the current character is an unquoted '$' or '`', the shell shall
** identify the start of any candidates for parameter expansion ( Parameter
** Expansion), command substitution ( Command Substitution), or arithmetic
** expansion ( Arithmetic Expansion) from their introductory unquoted character
** sequences: '$' or "${", "$(" or '`', and "$((", respectively. The shell shall
** read sufficient input to determine the end of the unit to be expanded (as
** explained in the cited sections). While processing the characters, if
** instances of expansions or quoting are found nested within the substitution,
** the shell shall recursively process them in the manner specified for the
** construct that is found. The characters found from the beginning of the
** substitution to its end, allowing for any recursion necessary to recognize
** embedded constructs, shall be included unmodified in the result token,
** including any embedded or enclosing substitution operators or quotes. The
** token shall not be delimited by the end of the substitution.
*/

void	rule_5(char c, t_token *token, t_lctx *ctx)
{
	size_t	skip;
	char	**s;
	short	i;

	skip = ERROR;
	i = ctx->i;
	s = &ctx->input;
	if ((OK(ctx->status) && P_BACKTICK(ctx->status, s, lex_quote, i, (&skip)))
		|| (OK(ctx->status) && P_ARITH(ctx->status, s, lex_quote, i, (&skip)))
		|| (OK(ctx->status) && P_CMD(ctx->status, s, lex_quote, i, (&skip)))
		|| (OK(ctx->status) && P_UPARAM(ctx->status, s, lex_quote, i, (&skip)))
		|| (OK(ctx->status) && P_EPARAM(ctx->status, s, lex_quote, i, (&skip))))
		skip += 1;
	else if (c == '$')
		skip = 1;
	if ((!token->type && ERR(create_new_tok(token, ctx, LEXER_WORD)))
		|| ERR(append_to_tok(c, token)))
		ctx->status = ERROR;
	if (OK(ctx->status)
		&& !ft_bufappend(token->value, &(*s)[ctx->i + 1], skip - 1))
		ctx->status = ERROR;
	if (NONE(ctx->status) && !(g_missing = NULL))
		ft_lstmerge(&ctx->missing, g_missing);
	ctx->i += OK(ctx->status) ? skip : 0;
}

/*
** 6: If the current character is not quoted and can be used as the first
** character of a new operator, the current token (if any) shall be delimited.
** The current character shall be used as the beginning of the next (operator)
** token.
*/

void	rule_6(char c, t_token *token, t_list **tokens, t_lctx *ctx)
{
	t_list	*node;

	ctx->i += 1;
	node = NULL;
	if (token->type && (c == '<' || c == '>'))
		detect_io_number(token);
	if (token->type && ERR(push_token(token, node, tokens, ctx)))
		ctx->status = ERROR;
	ctx->op_state = next_op_state(c, START);
	if (ERR(create_new_tok(token, ctx, ctx->op_state))
		|| ERR(append_to_tok(c, token)))
		ctx->status = ERROR;
	ctx->status = SUCCESS;
}

/*
** 7. If the current character is an unquoted <newline>, the current token shall
** be delimited.
*/

void	rule_7(t_token *token, t_list **tokens, t_lctx *ctx)
{
	rule_8(token, tokens, ctx);
}

/*
** 8. If the current character is an unquoted <blank>, any token containing the
** previous character is delimited and the current character shall be discarded.
*/

void	rule_8(t_token *token, t_list **tokens, t_lctx *ctx)
{
	t_list	*node;

	ctx->i += 1;
	node = NULL;
	if (token->type && ERR(push_token(token, node, tokens, ctx)))
		ctx->status = ERROR;
	ctx->status = SUCCESS;
}
