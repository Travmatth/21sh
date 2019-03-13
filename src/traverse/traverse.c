/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 15:11:58 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/12 18:25:29 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		exec_separator(int *is_bg, t_ast_node *root)
{
	if (!ft_strcmp("separator_op linebreak", root->rhs))
	{
		if (((t_ast_node*)root->val[0])->type == PARSE_SEPARATOR_OP
			&& (!ft_strcmp(((t_ast_node*)root->val[0])->rhs, "&")))
		{
			*is_bg = TRUE;
			return (SUCCESS);
		}
	}
	*is_bg = FALSE;
	return (SUCCESS);
}

int		exec_complete_command(t_ectx *e_ctx, t_ast_node *root)
{
	if (!ft_strcmp("list separator", root->rhs)
		&& ERR(exec_separator(&e_ctx->is_bg, root->val[1])))
		return (ERROR);
	return (SUCCESS);
}

int		traverse_ast(t_ast *ast)
{
	t_ectx	e_ctx;

	reset_exec_ctx(&e_ctx);
	if (!ft_strcmp("complete_command $end", ast->root->rhs))
		return (exec_complete_command(&e_ctx, ast->root->val[0]));
	return (NIL);
}
