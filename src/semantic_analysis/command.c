/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 15:15:29 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/15 18:07:30 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Create a new char* array by joing two arrays into one
*/

char	**ft_strjoinarrs(char **arr_1, char **arr_2)
{
	int		i;
	int		arr_1_len;
	char	**new;

	i = 0;
	arr_1_len = 0;
	while (arr_1 && arr_1[arr_1_len])
		arr_1_len += 1;
	while (arr_2[i])
		i += 1;
	if (!(new = ft_memalloc(sizeof(char*) * (arr_1_len + i + 1))))
		return (NULL);
	i = -1;
	while (arr_1 && arr_1[++i])
		new[i] = arr_1[i];
	arr_1_len = i >= 0 ? i : 0;
	i = -1;
	while (arr_2[++i])
		new[arr_1_len + i] = arr_2[i];
	return (new);
}

/*
** Process word token to be used as basis of t_simple_command. Word is subjected
** to full expansion before appending to command
*/

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

/*
** Process given simple command to extract command name, arguements and
** any redirections. Builds a t_simple_command struct to later be executed
*/

int		simple_command(t_simple_command *command, t_ast_node *root, int bg)
{
	int		status;
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
		if (!OK((status = cmd_name(command, root->val[0]))))
			return (status);
		if (!OK((status = suffix(command, root->val[1]))))
			return (status);
	}
	else if (IS_A("cmd_name", root->rhs))
	{
		if (!OK((status = cmd_name(command, root->val[0]))))
			return (status);
	}
	status = verify_command(&command->command[0]);
	command->is_builtin = status == BUILTIN ? TRUE : FALSE;
	command->bg = bg;
	return (status);
}

/*
** Differentiate simple commands to be processed from
** unimplemented compound commands to be gracefully exited from
*/

int		command(t_simple_command *cmd, t_ast_node *root, int bg)
{
	if (IS_A("simple_command", root->rhs))
		return (simple_command(cmd, root->val[0], bg));
	else if (IS_A("compound_command", root->rhs)
		|| IS_A("compound_command redirect_list", root->rhs)
		|| IS_A("function_definition", root->rhs))
	{
		ft_printf("Error: %s not implemented", root->rhs);
		return (NIL);
	}
	else
		return (ERROR);
}
