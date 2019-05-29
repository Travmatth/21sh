/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/15 16:18:58 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/29 13:27:54 by tmatthew         ###   ########.fr       */
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
** After a token has been delimited, but before applying the grammatical rules
** in Shell Grammar, a resulting word that is identified to be the command name
** word of a simple command shall be examined to determine whether it is an
** unquoted, valid alias name. However, reserved words in correct grammatical
** context shall not be candidates for alias substitution. A valid alias name
** (see XBD Alias Name) shall be one that has been defined by the alias utility
** and not subsequently undefined using unalias. Implementations also may
** provide predefined valid aliases that are in effect when the shell is
** invoked. To prevent infinite loops in recursive aliasing, if the shell is not
** currently processing an aliasof the same name, the word shall be replaced by
** the value of the alias; otherwise, it shall not be replaced.
**
** If the value of the alias replacing the word ends in a <blank>, the shell
** shall check the next command word for alias substitution; this process shall
** continue until a word is found that is not a valid alias or an alias value
** does not end in a <blank>.
**
** When used as specified by this volume of POSIX.1-2017, alias definitions
** shall not be inherited by separate invocations of the shell or by the
** utility execution environments invoked by the shell; see Shell Execution
** Environment.
*/

int		substitute_alias(t_token *token)
{
	(void)token;
	return (SUCCESS);
}

/*
** Processing tokens involves applying certain rules described in
** `2.10.2 Shell Grammar Rules`. The token is inspected to determine whether
** it can be converted to a reserved word, or converted into an ASSIGNMENT_WORD
** type. If neither are true, the given lexical type of the token is converted
** to the equivalent syntactical type of the token so as to be able to be used
** in later syntactic analysis
** Since lexical and syntactic analysis use separate and unique enums
** to identify given tokens, conversion between the two types is necessary
*/

int		process_token(t_token *token)
{
	int		status;
	int		i;

	status = SUCCESS;
	if (!OK((status = substitute_alias(token)))
		|| !OK((status = process_reserved(token))))
		return (status);
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
** Once a token is delimited, it is processed by process_token and then
** embedded into an ast_node, which is then ast_node is embedded into a
** stack_token. The stack_token is pushed onto the list of stack_tokens
** which will later be processed in syntactical analysis.
*/

int		push_token(t_token *token, t_list *node, t_list **tokens, t_lctx *ctx)
{
	t_ast_node	*ast_node;
	t_stack		stack_node;
	int			status;

	if (!OK((status = process_token(token)))
		|| !(ast_node = (t_ast_node*)ft_memalloc(sizeof(t_ast_node)))
		|| !(ast_node->val = (void**)ft_memalloc(sizeof(void*) * 2)))
		return (OK(status) ? ERROR : status);
	ast_node->val[0] = token->value->buf;
	ast_node->type = token->type;
	stack_node.type = STACK_TOKEN;
	stack_node.item.token = ast_node;
	if (!(node = ft_lstnew(&stack_node, sizeof(t_stack))))
		return (ERROR);
	ft_lstpushback(tokens, node);
	ctx->in_word = FALSE;
	ctx->op_state = FALSE;
	free(token->value);
	token->type = NIL;
	token->value = NULL;
	return (SUCCESS);
}
