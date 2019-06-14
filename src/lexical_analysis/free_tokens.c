/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tokens.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/13 16:16:55 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/13 16:34:07 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	free_tokens(t_list **tokens)
{
	t_list		*next;
	t_ast_node	*ast_node;
	t_stack		*stack_node;

	if (*tokens)
	{
		next = (*tokens)->next;
		stack_node = (t_stack*)(*tokens)->content;
		ast_node = (t_ast_node*)stack_node->item.token;
		free(ast_node->lhs);
		free(ast_node->rhs);
		ft_freearr((char**)ast_node->val, TRUE);
		free(stack_node);
		free(*tokens);
		*tokens = NULL;
		free_tokens(&next);
	}
}
