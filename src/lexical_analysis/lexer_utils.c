/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 14:37:15 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/13 18:47:55 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

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

int		append_to_tok(char c, t_token *token)
{
	if (!ft_bufappend(token->value, &c, sizeof(char)))
		return (ERROR);
	return (SUCCESS);
}

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

t_token_cnv	g_tok_cnv[TOKEN_CONVERSIONS] =
{
	{ EOI, PARSE_END },
	{ LEXER_WORD, PARSE_WORD },
	{ IO_NUMBER, PARSE_IO_NUMBER },
	{ L_PAREN, PARSE_L_PAREN },
	{ R_PAREN, PARSE_R_PAREN },
	{ AMPERSAND, PARSE_AMPERSAND },
	{ AND_IF, PARSE_AND_IF },
	{ PIPE, PARSE_PIPE },
	{ OR_IF, PARSE_OR_IF },
	{ SEMICOLON, PARSE_SEMICOLON },
	{ LESS, PARSE_LT },
	{ DLESS, PARSE_DLESS },
	{ GREAT, PARSE_GT },
	{ DGREAT, PARSE_DGREAT },
	{ LESSAND, PARSE_LESSAND },
	{ GREATAND, PARSE_GREATAND },
	{ LESSGREAT, PARSE_LESSGREAT },
	{ DLESSDASH, PARSE_DLESSDASH },
	{ L_BRACE, PARSE_LBRACE },
	{ R_BRACE, PARSE_RBRACE },
	{ CLOBBER, PARSE_CLOBBER }
};

int		convert_token(t_token *token)
{
	int		i;

	i = -1;
	while (++i < TOKEN_CONVERSIONS)
	{
		if (token->type == g_tok_cnv[i].lex_tok)
		{
			token->type = g_tok_cnv[i].parse_tok;
			return (SUCCESS);
		}
	}
	return (ERROR);
}

int		push_token(t_token *token, t_list *node, t_list **tokens, t_lctx *ctx)
{
	t_ast_node	*ast_node;
	t_stack		*stack_node;

	if (ERR(convert_token(token))
		|| !(ast_node = (t_ast_node*)ft_memalloc(sizeof(t_ast_node)))
		|| !(ast_node->val = (void**)ft_memalloc(sizeof(void*) * 2))
		|| !(ast_node->val[0] = ft_strdup((char*)token->value->buf)))
		return (ERROR);
	ast_node->type = token->type;
	if (!(stack_node = (t_stack*)ft_memalloc(sizeof(t_stack))))
		return (ERROR);
	stack_node->type = STACK_TOKEN;
	stack_node->item.token = ast_node;
	if (!(node = ft_lstnew(stack_node, sizeof(t_stack))))
		return (ERROR);
	ft_lstpushback(tokens, node);
	ctx->in_word = FALSE;
	ctx->op_state = FALSE;
	token->type = NIL;
	token->value = NULL;
	return (SUCCESS);
}

int		next_missing_symbol(t_list *missing)
{
	t_list	*node;
	int		type;

	if (!(node = ft_lstpeekhead(missing)))
		return (NIL);
	type = *(short*)node->content;
	return (type);
}

int		push_missing_symbol(short type, t_list **missing)
{
	t_list	*node;

	if (!(node = ft_lstnew(&type, sizeof(short))))
		return (ERROR);
	ft_lstpushfront(missing, node);
	return (SUCCESS);
}

int		pop_missing_symbol(t_list **missing)
{
	t_list	*node;

	node = ft_lsthead(missing);
	free(node->content);
	free(node);
	return (SUCCESS);
}
