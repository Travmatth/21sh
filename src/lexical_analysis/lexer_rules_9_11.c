/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_rules_9_11.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 14:43:48 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/23 14:44:27 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

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
	while (ctx->input[ctx->i] && ctx->input[ctx->i + 1] != '\n')
		ctx->i += 1;
	ctx->status = SUCCESS;
}

/*
** 11. The current character is used as the start of a new word.
*/

void	rule_11(char c, t_lctx *ctx, t_token *token)
{
	ctx->status = create_new_tok(token, ctx, LEXER_WORD);
	if (OK(ctx->status))
		ctx->status = append_to_tok(c, token);
}
