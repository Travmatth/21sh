/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   separator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 15:11:58 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/17 14:32:32 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Set whether given separator is a process background `&`
*/

int		separator_op_is_linebreak(int *is_bg, t_ast_node *root)
{
	*is_bg = IS_A(root->rhs, "&") ? TRUE : FALSE;
	return (SUCCESS);
}

/*
** Detect whether given separator is a process background `&`
*/

int		separator(int *is_bg, t_ast_node *root)
{
	return (separator_op_is_linebreak(is_bg, (t_ast_node*)root->val[0]));
}
