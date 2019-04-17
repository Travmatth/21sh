/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/15 16:18:58 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/16 13:26:51 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** List of equivalent syntactic token type for given lexical token type
*/

t_token_cnv	g_tok_cnv[OP_TOKEN_CONVERSIONS] =
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
	{ CLOBBER, PARSE_CLOBBER }
};

/*
** Since lexical and syntactic analysis use separate and unique enums
** to identify given tokens, conversion between the two types is necessary
*/

int		convert_token(t_token *token)
{
	int		i;

	i = -1;
	while (++i <= OP_TOKEN_CONVERSIONS)
	{
		if (token->type == g_tok_cnv[i].lex_tok)
		{
			token->type = g_tok_cnv[i].parse_tok;
			return (SUCCESS);
		}
	}
	return (ERROR);
}

/*
** 21sh does not implement variable assignment, so returning NIL
** gracefully fails current command execution and allows shell
** to accept next command to execute
*/

int		process_assignment(t_token *token)
{
	char	*equals;
	char	*contents;

	contents = (char*)token->value->buf;
	equals = ft_strchr(contents, '=');
	if (equals)
	{
		ft_putendl("Lexical Error: variable assignment not implemented");
		return (NIL);
	}
	else
		return (SUCCESS);
}

/*
** Processing tokens involves applying certain rules described in
** `2.10.2 Shell Grammar Rules`. The token is inspected to determine whether
** it can be converted to a reserved word, or converted into an ASSIGNMENT_WORD
** type. If neither are true, the given lexical type of the token is converted
** to the equivalent syntactical type of the token so as to be able to be used
** in later syntactic analysis
*/

int		process_token(t_token *token)
{
	int		status;

	status = SUCCESS;
	if (!OK((status = process_reserved(token)))
		|| !OK((status = process_assignment(token)))
		|| !OK((status = convert_token(token))))
		return (status);
	return (status);
}

/*
** Once a token is delimited, it is processed by process_token and then
** embedded into an ast_node, which is then ast_node is embedded into a
** stack_token. The stack_token is pushed onto the list of stack_tokens
** which will later be processed in syntactical analysis. 
*/

int		push_token(t_token *token, t_list *node, t_list **tokens, t_lctx *ctx)
{
	t_ast_node	*ast_node;
	t_stack		*stack_node;
	int			status;

	if (!OK((status = process_token(token)))
		|| !(ast_node = (t_ast_node*)ft_memalloc(sizeof(t_ast_node)))
		|| !(ast_node->val = (void**)ft_memalloc(sizeof(void*) * 2))
		|| !(ast_node->val[0] = ft_strdup((char*)token->value->buf)))
		return (OK(status) ? ERROR : status);
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
