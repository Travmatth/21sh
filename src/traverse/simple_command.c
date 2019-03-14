/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 15:15:29 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/13 18:15:39 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		process_prefix(t_ectx *e_ctx, char ***command, t_ast_node *root)
{
	(void)e_ctx;
	(void)command;
	(void)root;
	return (SUCCESS);
}

int		process_suffix(t_ectx *e_ctx, char ***command, t_ast_node *root)
{
	(void)e_ctx;
	(void)command;
	if (!ft_strcmp("io_redirect", root->rhs))
	{
		// implement
	}
	else if (!ft_strcmp("cmd_suffix io_redirect", root->rhs))
	{
		// implement
	}
	else if (!ft_strcmp("WORD", root->rhs))
	{
		// implement
	}
	else if (!ft_strcmp("cmd_suffix WORD", root->rhs))
	{
		// implement
	}
	return (SUCCESS);
}

int		process_cmd(t_ectx *e_ctx, char ***command, t_ast_node *root)
{
	(void)e_ctx;
	if (!*command && !(*command = (char**)ft_memalloc(sizeof(char*) * 2)))
		return (ERROR);
	if (!((*command)[0] = ft_strdup(((t_ast_node*)root->val[0])->val[0])))
		return (ERROR);
	return (SUCCESS);
}

int		exec_simple_command(t_ectx *e_ctx, t_ast_node *root)
{
	char	**command;

	command = NULL;
	if (!ft_strcmp("cmd_prefix cmd_word cmd_suffix", root->rhs))
	{
		// not implemented
	}
	else if (!ft_strcmp("cmd_prefix cmd_word", root->rhs))
	{
		// not implemented
	}
	else if (!ft_strcmp("cmd_prefix", root->rhs))
	{
		// not implemented
	}
	else if (!ft_strcmp("cmd_name cmd_suffix", root->rhs))
	{
		if (ERR(process_cmd(e_ctx, &command, root->val[0])))
			return (ERROR);
		if (ERR(process_suffix(e_ctx, &command, root->val[0])))
			return (ERROR);
	}
	else if (!ft_strcmp("cmd_name", root->rhs))
	{
		if (ERR(process_cmd(e_ctx, &command, root->val[0])))
			return (ERROR);
	}
	return (SUCCESS);
}
