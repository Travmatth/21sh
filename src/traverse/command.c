/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 15:15:29 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/23 17:04:16 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

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
		if (ERR(process_suffix(e_ctx, &command, root->val[1])))
			return (ERROR);
	}
	else if (!ft_strcmp("cmd_name", root->rhs))
	{
		if (ERR(process_cmd(e_ctx, &command, root->val[0])))
			return (ERROR);
	}
	e_ctx->exit_code = execute_cmd(command);
	reset_exec_ctx(e_ctx);
	return (SUCCESS);
}
