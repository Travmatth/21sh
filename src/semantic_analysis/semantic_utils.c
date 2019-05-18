/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 16:04:05 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/18 16:14:21 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	free_ast_node(t_ast_node *root)
{
	int		i;

	if (IS_TERMINAL(root->type))
		free(root->val[0]);
	else
	{
		i = -1;
		while (root->val && root->val[++i])
			free_ast_node((t_ast_node*)root->val[i]);
	}
	free(root->val);
	free(root->lhs);
	free(root->rhs);
	free(root);
}

void	free_ast(t_ast *ast)
{
	free_ast_node((t_ast_node*)ast->root->val[0]);
	free(ast->root->val);
	free(ast->root);
}
