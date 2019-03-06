/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_rules_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/27 12:45:22 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/05 11:17:43 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** 6: If the current character is not quoted and can be used as the first
** character of a new operator, the current token (if any) shall be delimited.
** The current character shall be used as the beginning of the next (operator)
** token.
*/

void	rule_6(char c, t_token *token, t_list **tokens, t_lctx *ctx)
{
	size_t	i;
	t_list	*node;
	int		io_here;

	ctx->i += 1;
	node = NULL;
	if (token->type == LEXER_WORD && token->value->current)
	{
		i = 0;
		io_here = TRUE;
		while ((io_here == TRUE) && i < token->value->current)
		{
			if (!ft_isdigit(((char*)token->value->buf)[i]))
				io_here = FALSE;
			i += 1;
		}
		if (io_here == TRUE)
			token->type = IO_HERE;
	}
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

void	rule_7(char c, t_token *token, t_list **tokens, t_lctx *ctx)
{
	(void)c;
	(void)token;
	(void)tokens;
	(void)ctx;
	// {
	// 	ctx.i += 1;
	// 	ctx.in_word = FALSE;
	// }
	ctx->status = SUCCESS;
}

/*
** 8. If the current character is an unquoted <blank>, any token containing the
** previous character is delimited and the current character shall be discarded.
*/

void	rule_8(t_token *token, t_list **tokens, t_lctx *ctx)
{
	size_t	i;
	t_list	*node;
	int		io_here;

	ctx->i += 1;
	node = NULL;
	if (token->type == LEXER_WORD && token->value->current)
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
	}
	if (token->type && ERR(push_token(token, node, tokens, ctx)))
		ctx->status = ERROR;
	ctx->status = SUCCESS;
}

/*
** 9. If the previous character was part of a word, the current character shall
** be appended to that word.
*/

void	rule_9(char c, t_token *token, t_list **tokens, t_lctx *ctx)
{
	(void)tokens;
	ctx->i += 1;
	if (!(ft_bufappend(token->value, &c, sizeof(char))))
		ctx->status = ERROR;
	ctx->status = SUCCESS;
}

/*
** 10. If the current character is a '#', it and all subsequent characters
** up to, but excluding, the next <newline> shall be discarded as a comment.
** The <newline> that ends the line is not considered part of the comment.
*/

void	rule_10(char c, t_token *token, t_list **tokens, t_lctx *ctx)
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
	ctx->status = SUCCESS;
}
