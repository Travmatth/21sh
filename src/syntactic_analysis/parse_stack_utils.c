/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_stack_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 19:23:40 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/17 18:01:58 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Create syntactic parse token that may hold either syntactic parse state
** or lexical token
*/

t_list		*create_stack_token(int type, t_ast_node *token, int state)
{
	t_stack	stack_token;

	stack_token.type = type;
	if (type == STACK_STATE)
		stack_token.item.state = state;
	else if (type == STACK_TOKEN)
		stack_token.item.token = token;
	return (ft_lstnew((void*)&stack_token, sizeof(t_stack)));
}

/*
** LR_1 stacks have end stack symbol as first element
*/

t_list		*create_end_stack_token(void)
{
	t_stack	stack_token;

	stack_token.type = STACK_END;
	stack_token.item.token = NULL;
	return (ft_lstnew((void*)&stack_token, sizeof(t_stack)));
}

/*
** Pop current lexical token off token stream to be used as current word
** in syntactic parsing
*/

t_ast_node	*pop_token(t_list **tokens)
{
	t_list		*top;
	t_stack		*node;
	t_ast_node	*ast_node;

	if (!(top = ft_lsthead(tokens)))
		return (NULL);
	node = (t_stack*)top->content;
	ast_node = (t_ast_node*)node->item.token;
	ft_dprintf(STDERR, "pop_token: buf: %p containing: %d\n", (void*)ast_node->val[0]);
	free(top);
	free(node);
	return (ast_node);
}
