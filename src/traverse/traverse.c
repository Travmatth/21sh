/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   traverse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 15:11:58 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/14 13:09:19 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		separator_op_is_linebreak(int *is_bg, t_ast_node *root)
{
	if (root->type == PARSE_SEPARATOR_OP && !ft_strcmp(root->rhs, "&"))
	{
		*is_bg = TRUE;
		return (SUCCESS);
	}
	*is_bg = FALSE;
	return (ERROR);
}

int		exec_separator(int *is_bg, t_ast_node *root)
{
	if (!ft_strcmp("separator_op linebreak", root->rhs))
		separator_op_is_linebreak(is_bg, (t_ast_node*)root->val[0]);
	return (SUCCESS);
}

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

int		exec_and_or(t_ectx *e_ctx, t_ast_node *root)
{
	if (ft_strcmp("pipeline", root->rhs))
	{
		if (((t_ast_node*)root->val[1])->type == PARSE_AND_IF)
		{
			exec_and_or(e_ctx, root->val[0]);
			if (EXIT_OK(e_ctx->exit_code))
				return (exec_pipeline(e_ctx, root->val[2]));
			return (SUCCESS);
		}
		else
		{
			exec_and_or(e_ctx, root->val[0]);
			if (!EXIT_OK(e_ctx->exit_code))
				return (exec_pipeline(e_ctx, root->val[2]));
			return (SUCCESS);
		}
	}
	else
		return (exec_pipeline(e_ctx, root->val[0]));
}

int		exec_list(t_ectx *e_ctx, t_ast_node *root)
{
	int		is_bg;
	int		position;

	is_bg = FALSE;
	position = 0;
	if (!ft_strcmp("list separator_op and_or", root->rhs)
		&& OK(exec_list(e_ctx, root->val[0])))
	{
		position = 2;
		separator_op_is_linebreak(&is_bg, (t_ast_node*)root->val[0]);
	}
	if (!is_bg)
		exec_and_or(e_ctx, root->val[position]);
	else
	{
		// how do we execute in the background?
		// place as member in e_ctx struct?
	}
	return (SUCCESS);
}

int		exec_complete_command(t_ectx *e_ctx, t_ast_node *root)
{
	if (!ft_strcmp("list separator", root->rhs)
		&& ERR(exec_separator(&e_ctx->is_bg, root->val[1])))
		return (ERROR);
	return (exec_list(e_ctx, root->val[0]));
}

int		traverse_ast(t_ast *ast)
{
	t_ectx	e_ctx;

	set_exec_ctx(&e_ctx);
	if (!ft_strcmp("complete_command $end", ast->root->rhs))
		return (exec_complete_command(&e_ctx, ast->root->val[0]));
	return (NIL);
}
