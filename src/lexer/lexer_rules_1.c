/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_rules_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/27 12:44:27 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/04 14:36:17 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

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
	if (ERR(push_token(token, node, tokens, ctx)))
		ctx->status = ERROR;
	ctx->status = SUCCESS;
}

/*
** 2. If the previous character was used as part of an operator and the current
** character is not quoted and can be used with the current characters to form
** an operator, it shall be used as part of that (operator) token.
*/

int		rule_2(char c, t_token *token, t_lctx *ctx)
{
	int		next;

	if (!ACCEPTING((next = next_op_state(c, ctx->op_state))))
		return (ctx->status = NIL);
	if (!(ft_bufappend(token->value, &c, sizeof(char))))
		return (ctx->status = ERROR);
	ctx->i += 1;
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
	if (ERR(push_token(token, node, tokens, ctx)))
		ctx->status = ERROR;
	ctx->status = SUCCESS;
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

void	rule_4(char c, char *input, t_token *token, t_lctx *ctx)
{
	size_t	end;

	end = 0;
	if (NONE(token->type) && ERR(create_new_tok(token, ctx, LEXER_WORD)))
		ctx->status = ERROR;
	if (c == '\\')
		end = 2;
	else if (c == '"' || c == '\'')
	{
		if (!OK((end = c == '"'
			? find_dquote(input, ctx->i, ctx)
			: find_quote(input, ctx->i, ctx))))
		{
			ctx->status = end;
			return ;
		}
		ctx->in_word = TRUE;
	}
	ft_bufappend(token->value, &input[ctx->i], end + 1);
	ctx->i += end + 1;
	ctx->status = SUCCESS;
}

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
	int		skip;

	if ((c == '`' && !OK((skip = find_bquote(ctx->input, ctx->i, ctx))))
			|| (c == '$' && !OK((skip = find_subst(ctx->input, ctx->i, ctx)))))
	{
		ctx->status = skip;
		return ;
	}
	if ((!token->type && ERR(create_new_tok(token, ctx, LEXER_WORD)))
		|| ERR(append_to_tok(c, token)))
	{
		ctx->status = ERROR;
		return ;
	}
	if (!ft_bufappend(token->value, &ctx->input[ctx->i + 1], skip - 1))
	{
		ctx->status = ERROR;
		return ;
	}
	ctx->i += skip;
	ctx->status = SUCCESS;
}
