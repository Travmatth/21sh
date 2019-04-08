/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 15:11:58 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/06 16:01:51 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		separator_op_is_linebreak(int *is_bg, t_ast_node *root)
{
	if (root->type == PARSE_SEPARATOR_OP && IS_A(root->rhs, "&"))
	{
		*is_bg = TRUE;
		return (SUCCESS);
	}
	*is_bg = FALSE;
	return (ERROR);
}

int		separator(int *is_bg, t_ast_node *root)
{
	if (IS_A("separator_op linebreak", root->rhs))
		separator_op_is_linebreak(is_bg, (t_ast_node*)root->val[0]);
	return (SUCCESS);
}
