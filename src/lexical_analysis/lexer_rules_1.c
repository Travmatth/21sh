/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_rules_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/27 12:44:27 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/15 13:53:29 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

extern t_list	*g_missing;
int		lex_quote(char **str, int start, int end)
{
	(void)str;
	(void)start;
	(void)end;
	return (SUCCESS);
}

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
	if (!ERR(ctx->status) && ERR(push_token(token, node, tokens, ctx)))
		ctx->status = ERROR;
	if (!ERR(ctx->status))
	{
		ctx->status = SUCCESS;
		ctx->op_state = START;
	}
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
	int		skip;
	int		status;

	skip = 0;
	status = SUCCESS;
	if (NONE(token->type) && ERR(create_new_tok(token, ctx, LEXER_WORD)))
		ctx->status = ERROR;
	if (c == '\\')
		skip = 2;
	else if (c == '"')
	{
		if (ERR((status = dbl_quote(&ctx->input, ctx->i, &skip, lex_quote))))
			ctx->status = ERROR;
	}
	else if (c == '\'')
	{
		if (ERR((status = quote(&ctx->input, ctx->i, &skip, lex_quote))))
			ctx->status = ERROR;
	}
	if (NONE(status))	
	{
		ctx->status = NIL;
		ctx->missing = g_missing;
	}
	ft_bufappend(token->value, &input[ctx->i], skip + 1);
	ctx->i += skip + 1;
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
	int		status;
	char	**in;

	skip = ERROR;
	status = SUCCESS;
	in = &ctx->input;
	if (c == '`' && OK((status = backtick(in, ctx->i, &skip, lex_quote))))
		skip += 1;
	else if (c == '$')
	{
		if ((TWO_PARENS(ctx->input, ctx->i)
				&& OK((ctx->status = arith_exp(in, ctx->i, &skip, lex_quote))))
			|| (NEXT_PAREN(ctx->input, ctx->i)
				&& OK((ctx->status = command_sub(in, ctx->i, &skip, lex_quote))))
			|| (NEXT_BRACE(ctx->input, ctx->i)
				&& OK((status = param_exp(in, ctx->i, &skip, lex_quote)))))
			skip += 1;
		else if (IS_VAR_CHAR(ctx->input[ctx->i + 1]))	
			skip = 1;
	}
	if (NONE(ctx->status))	
		ctx->missing = g_missing;
	if ((!token->type && ERR(create_new_tok(token, ctx, LEXER_WORD)))
		|| ERR(append_to_tok(c, token)))
		ctx->status = ERROR;
	if (!ft_bufappend(token->value, &(*in)[ctx->i + 1], skip - 1))
		ctx->status = ERROR;
	ctx->i += OK(ctx->status) ? skip : 0;
}
