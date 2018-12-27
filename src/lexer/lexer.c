/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 14:37:15 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/26 19:18:57 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

# define END 1
# define BEGIN 0

// if we encounter another opening candidate
// we need to restart search w/ that sequence

/*
** find the next token in the complete_command
*/

int		escaped(char *input, size_t i)
{
	if (i == 0)
		return (FALSE);
	else if (input[i - 1] == '\\')
		return (TRUE);
	return (FALSE);
}

void	init_lexer_ctx(t_lctx *ctx)
{
	ctx->i = 0;
	ctx->j = 0;
	ctx->stop = 0;
	ctx->op_state = 0;
	ctx->in_word = 0;
}

int		create_new_tok(char c, t_token *token, t_lctx *ctx, int type)
{
	char	*buf;

	token->type = type;
	ctx->in_word = TRUE;
	if (!(buf = ft_memalloc(sizeof(char) * 10)))
		return (ERROR);
	ft_memcpy(buf, &c, sizeof(char));
	if (!(token->value = ft_bufnew(buf, sizeof(char), sizeof(char) * 10)))
		return (ERROR);
	return (SUCCESS);
}

int		push_token(t_token *token, t_list *node, t_list **tokens, t_lctx *ctx)
{
	if (!(node = ft_lstnew(token, sizeof(t_token))))
		return (ERROR);
	ft_lstpushback(tokens, node);
	ctx->in_word = FALSE;
	ctx->op_state = FALSE;
	token->type = NIL;
	token->value = NULL;
	return (SUCCESS);
}

/*
** 1. If the end of input is recognized the current token shall be delimited.
** If there is no current token, the end-of-input indicator shall be returned as
** the token.
*/

int		rule_1(t_token *token, t_lctx *ctx, t_list **tokens)
{
	t_list	*node;

	if (!token->type)
	{
		token->type = EOI;
		token->value = NULL;
		ctx->stop = TRUE;
	}
	if (!(node = ft_lstnew(token, sizeof(t_token))))
		return (ERROR);
	ft_lstpushback(tokens, node);
	token->type = NIL;
	token->value = NULL;
	ctx->op_state = FALSE;
	return (SUCCESS);
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
		return (NIL);
	if (!(ft_bufappend(token->value, &c, sizeof(char))))
		return (ERROR);
	ctx->i += 1;
	ctx->op_state = next;
	token->type = next;
	return (SUCCESS);
}

/*
** 3. If the previous character was used as part of an operator and the current
** character cannot be used with the current characters to form an operator,
** the operator containing the previous character shall be delimited.
*/

int		rule_3(t_token *token, t_list **tokens, t_lctx *ctx)
{
	t_list	*node;

	if (!(node = ft_lstnew(token, sizeof(t_token))))
		return (ERROR);
	ft_lstpushback(tokens, node);
	ctx->op_state = FALSE;
	return (SUCCESS);
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

int		rule_4(char c, char *input, t_token *token, t_lctx *ctx)
{
	size_t	end;

	end = 0;
	if (NONE(token->type) && ERR(create_new_tok(c, token, ctx, WORD)))
		return (ERROR);
	if (c == '\\')
		end = 2;
	else if (c == '"')
	{
		if (NONE(find_next('"', &input[ctx->i], &end)))
		{
			ctx->stop = TRUE;
			ctx->missing = '"';
			return (NIL); 
		}
		end += 1;
		ctx->in_word = TRUE;
	}
	ft_bufappend(token->value, &input[ctx->i], end);
	ctx->i += end;
	return (SUCCESS);
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

int		rule_5(char c, t_token *token, t_list **tokens, t_lctx *ctx)
{
	(void)c;
	(void)token;
	(void)tokens;
	(void)ctx;
	// {
	// 	if (NONE((ctx.j = find_sub_end(c, &input[1], &j))))
	// 		return (ERROR);
	// 	ctx.i += ctx.j;
	// }
	return (SUCCESS);
}

/*
** 6: If the current character is not quoted and can be used as the first
** character of a new operator, the current token (if any) shall be delimited.
** The current character shall be used as the beginning of the next (operator)
** token.
*/

int		rule_6(char c, t_token *token, t_list **tokens, t_lctx *ctx)
{
	t_list	*node;

	node = NULL;
	if (ctx->in_word && ERR(push_token(token, node, tokens, ctx)))
		return (ERROR);
	ctx->i += 1;
	ctx->op_state = next_op_state(c, START);
	if (ERR(create_new_tok(c, token, ctx, ctx->op_state)))
		return (ERROR);
	return (SUCCESS);
}

/*
** 7. If the current character is an unquoted <newline>, the current token shall
** be delimited.
*/

int		rule_7(char c, t_token *token, t_list **tokens, t_lctx *ctx)
{
	(void)c;
	(void)token;
	(void)tokens;
	(void)ctx;
	// {
	// 	ctx.i += 1;
	// 	ctx.in_word = FALSE;
	// }
	return (SUCCESS);
}

/*
** 8. If the current character is an unquoted <blank>, any token containing the
** previous character is delimited and the current character shall be discarded.
*/

int		rule_8(t_token *token, t_list **tokens, t_lctx *ctx)
{
	int		io_here;
	size_t	i;
	t_list	*node;

	ctx->i += 1;
	node = NULL;
	if (token->type == WORD)
	{
		i = 0;
		io_here = TRUE;
		while ((io_here == TRUE) && i < token->value->current)
		{
			if (!ft_isdigit(((char*)token->value->buf)[i]))
				io_here = FALSE;
		}
		if (io_here == TRUE)
			token->type = IO_HERE;
		push_token(token, node, tokens, ctx);
	}
	return (SUCCESS);
}

/*
** 9. If the previous character was part of a word, the current character shall
** be appended to that word.
*/

int		rule_9(char c, t_token *token, t_list **tokens, t_lctx *ctx)
{
	(void)c;
	(void)tokens;
	ctx->i += 1;
	if (!(ft_bufappend(token->value, &c, sizeof(char))))
		return (ERROR);
	return (SUCCESS);
}

/*
** 10. If the current character is a '#', it and all subsequent characters up to,
** but excluding, the next <newline> shall be discarded as a comment.
** The <newline> that ends the line is not considered part of the comment.
*/

int		rule_10(char c, t_token *token, t_list **tokens, t_lctx *ctx)
{
	(void)c;
	(void)token;
	(void)tokens;
	(void)ctx;
	// {
		// if (NONE((ctx.j = find_sub_end(c, &input[1], &j))))
			// return (ERROR);
		// ctx.i += ctx.j;
	// }
	return (SUCCESS);
}

/*
** 11. The current character is used as the start of a new word.
*/

// TODO: define at exit funcs to clean up any state

int		rule_11(char c, t_token *token, t_lctx *ctx)
{
	char	*buf;

	(void)c;
	ctx->i += 1;
	token->type = WORD;
	ctx->in_word = TRUE;
	if (!(buf = ft_memalloc(sizeof(char) * 10)))
		return (ERROR);
	ft_memcpy(buf, &c, sizeof(char));
	if (!(token->value = ft_bufnew(buf, sizeof(char), sizeof(char) * 10)))
		return (ERROR);
	return (SUCCESS);
}

int		lexical_analysis(char *input, t_list **tokens)
{
	t_lctx	ctx;
	t_token	token;
	char	c;
	int		status;

	if (!input)
		return (NIL);
	init_lexer_ctx(&ctx);
	while (!ctx.stop)
	{
		c = input[ctx.i];
		if (!c)
			status = rule_1(&token, &ctx, tokens);
		else if (ctx.op_state && !escaped(input, ctx.i)
			&& rule_2(c, &token, &ctx))
			ctx.i += 1;
		else if (ctx.op_state)
			status = rule_3(&token, tokens, &ctx);
		else if (!escaped(input, ctx.i) && (c == '\\' || c == '\'' || c == '"'))
			status = rule_4(c, input, &token, &ctx);
		else if (!escaped(input, ctx.i) && (c == '$' || c == '`'))
			status = rule_5(c, &token, tokens, &ctx);
		else if (can_form_op(c))
			status = rule_6(c, &token, tokens, &ctx);
		else if (c == '\n' && !escaped(input, ctx.i))
			status = rule_7(c, &token, tokens, &ctx);
		else if (c == ' ' && !escaped(input, ctx.i))
			status = rule_8(&token, tokens, &ctx);
		else if (ctx.in_word)
			status = rule_9(c, &token, tokens, &ctx);
		else if (c == '#')
			status = rule_10(c, &token, tokens, &ctx);
		else
			status = rule_11(c, &token, &ctx);
		if (ERR(status))
			return (ERROR);
	}
	return (SUCCESS);
}
