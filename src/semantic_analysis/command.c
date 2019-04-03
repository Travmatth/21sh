/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 15:15:29 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/02 17:39:14 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		cmd_name(t_simple_command *command, t_ast_node *root)
{
	char	**tmp;

	if (!(tmp = push_pointer_back(command->command, root->val[0])))
		return (ERROR);
	free(command->command);
	command->command = tmp;
	return (SUCCESS);
}

int		simple_command(t_simple_command *command, t_ast_node *root, int bg)
{
	char	**argv;

	argv = NULL;
	if (IS_A("cmd_prefix cmd_word cmd_suffix", root->rhs))
	{
		// not implemented
	}
	else if (IS_A("cmd_prefix cmd_word", root->rhs))
	{
		// not implemented
	}
	else if (IS_A("cmd_prefix", root->rhs))
	{
		// not implemented
	}
	else if (IS_A("cmd_name cmd_suffix", root->rhs))
	{
		if (ERR(cmd_name(command, root->val[0])))
			return (ERROR);
		if (ERR(suffix(command, root->val[1])))
			return (ERROR);
	}
	else if (IS_A("cmd_name", root->rhs))
	{
		if (ERR(cmd_name(command, root->val[0])))
			return (ERROR);
	}
	(void)bg;
	// TODO
	// verify_command();
	// t_ctx->exit_code = execute_cmd(command);
	// TODO
	return (SUCCESS);
}

int		command(t_simple_command *cmd, t_ast_node *root, int bg)
{
	if (IS_A("simple_command", root->rhs))
		return (simple_command(cmd, root->val[0], bg));
	else if (IS_A("compound_command", root->rhs))
	{
		ft_putendl("Error: compound command not implemented");
		return (NIL);
	}
	else if (IS_A("compound_command redirect_list", root->rhs))
	{
		ft_putendl("Error: compound command not implemented");
		return (NIL);
	}
	else if (IS_A("function_definition", root->rhs))
	{
		ft_putendl("Error: compound command not implemented");
		return (NIL);
	}
	else
		return (ERROR);
}
