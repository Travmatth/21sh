/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 14:37:15 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/15 17:11:43 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Determine whether the current character index in the given string is
** preceded by an escape character `\`
*/

int		escaped(char *input, size_t i)
{
	if (i == 0)
		return (FALSE);
	else if (input[i - 1] == '\\')
		return (TRUE);
	return (FALSE);
}

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
** Create a new blank to which lexical parsing can alter to store next
** token to be created.
*/

int		create_new_tok(t_token *token, t_lctx *ctx, int type)
{
	char	*buf;

	token->type = type;
	if (type == LEXER_WORD)
		ctx->in_word = TRUE;
	if (!(buf = ft_memalloc(sizeof(char) * (type == LEXER_WORD ? 10 : 4))))
		return (ERROR);
	if (!(token->value = ft_bufnew(buf, 0, sizeof(char) * 10)))
		return (ERROR);
	return (SUCCESS);
}

/*
** Append given character from command to the current token
*/

int		append_to_tok(char c, t_token *token)
{
	if (!ft_bufappend(token->value, &c, sizeof(char)))
		return (ERROR);
	return (SUCCESS);
}

/*
** Returns next closing character sequence needed to correctly close
** given command
*/

int		next_missing_symbol(t_list *missing)
{
	t_list	*node;
	int		type;

	if (!(node = ft_lstpeekhead(missing)))
		return (NIL);
	type = *(short*)node->content;
	return (type);
}

/*
** When given command has elements that are not properly closed, the sequence
** of quotes or command substitutions are saved so as to allow correct sequence
** of closing characters needed.
*/

int		push_missing_symbol(short type, t_list **missing)
{
	t_list	*node;

	if (!(node = ft_lstnew(&type, sizeof(short))))
		return (ERROR);
	ft_lstpushfront(missing, node);
	return (SUCCESS);
}

/*
** When needed closing character detected, it's corresponding opening sequence
** can be popped from the stack. When stack is empty command is correctly
** closed.
*/

int		pop_missing_symbol(t_list **missing)
{
	t_list	*node;

	node = ft_lsthead(missing);
	free(node->content);
	free(node);
	return (SUCCESS);
}
