/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 15:11:58 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/23 17:06:46 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		exec_command(t_ectx *e_ctx, t_ast_node *root)
{
	if (!ft_strcmp("simple_command", root->rhs))
		return (exec_simple_command(e_ctx, root->val[0]));
	else
	{
		ft_printf("Commands beyond simple commands not implemented\n");
	}
	return (SUCCESS);
}

int		exec_pipe_seq(t_ectx *e_ctx, t_ast_node *root)
{
	int		position;

	position = 0;
	if (ft_strcmp("command", root->rhs))
	{
		position = 3;
		if (ERR(exec_pipe_seq(e_ctx, root->val[0])))
			return (ERROR);
	}
	return (exec_command(e_ctx, root->val[position]));
}

int		exec_pipeline(t_ectx *e_ctx, t_ast_node *root)
{
	int		position;

	position = 0;
	if (((t_ast_node*)root->val[0])->type == PARSE_BANG)
	{
		ft_printf("Bang operator not implemented");
		position = 1;
	}
	return (exec_pipe_seq(e_ctx, root->val[position]));
}
