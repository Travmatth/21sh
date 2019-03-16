/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_rules_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/27 12:45:22 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/15 15:58:42 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** 2.10.1.3 Shell Grammar Lexical Conventions
** If the string consists solely of digits and the delimiter character is
** one of '<' or '>', the token identifier IO_NUMBER shall be returned.
*/

void	detect_io_number(t_token *token)
{
	size_t	i;
	int		is_io_num;
	char	*str;

	i = 0;
	is_io_num = TRUE;
	str = token->value->buf;
	while (i < token->value->current)
	{
		if (str[i] < '0' || str[i] > '9')
		{
			is_io_num = FALSE;
			break ;
		}
		i += 1;
	}
	if (is_io_num)
		token->type = IO_NUMBER;
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

void	rule_10(t_lctx *ctx)
{
	while (ctx->input[ctx->i] && ctx->input[ctx->i] != '\n')
		ctx->i += 1;
	ctx->status = SUCCESS;
}
