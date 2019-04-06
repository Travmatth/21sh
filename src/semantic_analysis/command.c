/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 15:15:29 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/05 13:25:52 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	**ft_strjoinarrs(char **arr_1, char **arr_2)
{
	int		i;
	int		total;
	char	**new;

	i = 0;
	total = 0;
	while (arr_1[i])
		total += 1;
	while (arr_2[i])
		total += 1;
	if (!(new = ft_memalloc(sizeof(char*) * (total + 1))))
		return (NULL);
	i = -1;
	while (arr_1[++i])
		new[i] = arr_1[i];
	total = i;
	i = -1;
	while (arr_2[++i])
		new[total + i] = arr_1[i];
	return (new);
}

int		cmd_name(t_simple_command *command, t_ast_node *root)
{
	char	*name;
	int		status;
	char	**tmp;
	char	**fields;

	name = (char*)((t_ast_node*)root->val[0])->val[0];
	if (!OK((status = full_word_expansion(&fields, name))))
		return (status);
	if (!(tmp = ft_strjoinarrs(command->command, fields)))
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
