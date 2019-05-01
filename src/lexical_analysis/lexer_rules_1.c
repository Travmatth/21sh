/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_rules_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/27 12:44:27 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/30 18:21:37 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

extern t_list	*g_missing;

/*
** When rule 4 of lex_switch determines that a given command is not properly
** closed the opening sequence should be saved so that it may the necessary
** closing characters can be later determined
*/

int		lex_quote(char **str, int start, int end)
{
	int		status;
	short	type;

	if (NONE((status = ERR(end) ? NIL : SUCCESS)))
	{
		if (SNGL_QUOTE((*str), start))
			type = QUOTE;
		else if (DBL_QUOTE((*str), start))
			type = DQUOTE;
		else if (PARAM_EXP((*str), start))
			type = BRACE_SUB;
		else if (ARITH_EXP((*str), start))
			type = MATH_SUB;
		else if (CMD_SUB((*str), start))
			type = CMD_SUB;
		else if (BACKTICK((*str), start))
			type = BQUOTE;
		else if (BACKSLASH((*str), start))
			type = BACKSLASH_ESC;
		else
		{
			ft_printf(ERR_UNCLOSED_STR, *str);
			type = NIL;
		}
		if (ERR(push_missing_symbol(type, &g_missing)))
			status = ERROR;
	}
	return (status);
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
	char	**in;
	short	i;

	skip = ERROR;
	i = ctx->i;
	in = &ctx->input;
	if (
		(BACKTICK((*in), i) && OK((ctx->status = backtick(in, i, &skip, lex_quote))))
		|| (ARITH_EXP((*in), i) && OK((ctx->status = arith_exp(in, i, &skip, lex_quote))))
		|| (CMD_SUB((*in), i) && OK((ctx->status = command_sub(in, i, &skip, lex_quote))))
		|| (PARAM_EXP((*in), i) && OK((ctx->status = param_exp(in, i, &skip, lex_quote))))
	)
		skip += 1;
	else if (c == '$')
		skip = 1;
	if (NONE(ctx->status))	
		ctx->missing = g_missing;
	if ((!token->type && ERR(create_new_tok(token, ctx, LEXER_WORD)))
		|| ERR(append_to_tok(c, token)))
		ctx->status = ERROR;
	if (OK(ctx->status) && !ft_bufappend(token->value, &(*in)[ctx->i + 1], skip - 1))
		ctx->status = ERROR;
	if (NONE(ctx->status))	
	{
		ft_lstmerge(&ctx->missing, g_missing);
		g_missing = NULL;
	}
	ctx->i += OK(ctx->status) ? skip : 0;
}
