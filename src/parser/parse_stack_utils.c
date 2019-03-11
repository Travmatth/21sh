/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_stack_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/20 19:23:40 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/11 00:53:17 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

t_list	*create_stack_token(int type, t_ast_node *token, int state)
{
	t_stack	*stack_token;

	if (!(stack_token = (t_stack*)ft_memalloc(sizeof(t_stack))))
		return (NULL);
	stack_token->type = type;
	if (type == STACK_STATE)
		stack_token->item.state = state;
	else if (type == STACK_TOKEN)
	{
		stack_token->item.token = token;
	}
	return (ft_lstnew((void*)stack_token, sizeof(t_stack)));
}

t_list	*create_end_stack_token(void)
{
	t_stack	*stack_token;

	if (!(stack_token = (t_stack*)ft_memalloc(sizeof(t_stack))))
		return (NULL);
	stack_token->type = STACK_END;
	return (ft_lstnew((void*)stack_token, sizeof(t_stack)));
}

t_ast_node	*pop_token(t_list **tokens)
{
	t_list	*top;

	if (!(top = ft_lsthead(tokens)))
		return (NULL);
	return (t_ast_node*)((t_stack*)top->content)->item.token;
}

