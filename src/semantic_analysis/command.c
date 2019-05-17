/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 15:15:29 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/16 20:31:55 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Process word token to be used as basis of t_simple. Word is subjected
** to full expansion before appending to command
*/

int		cmd_name(t_simple *simple, t_ast_node *root)
{
	char	*name;
	int		status;
	char	**tmp;
	char	**fields;

	if (!(name = ft_strdup((char*)((t_ast_node*)root->val[0])->val[0])))
		return (ERROR);
	if (!OK((status = full_word_expansion(&fields, name)))
		|| !(tmp = ft_strjoinarrs(simple->command, fields)))
		return (OK(status) ? ERROR : status);
	free(fields);
	free(simple->command);
	simple->command = tmp;
	return (SUCCESS);
}

/*
** A simple command is one of 5 possible forms, each containing a possible
** prefix, suffix, word, or name. To simplify their processing, we determine
** the possible position of each of these symbols
*/

void	simple_positions(char *form, int position[3])
{
	position[PREFIX] = ERROR;
	position[COMMAND] = ERROR;
	position[SUFFIX] = ERROR;
	if (IS_A("cmd_prefix cmd_word cmd_suffix", form))
	{
		position[PREFIX] = 0;
		position[COMMAND] = 1;
		position[SUFFIX] = 2;
	}
	else if (IS_A("cmd_prefix cmd_word", form))
	{
		position[PREFIX] = 0;
		position[COMMAND] = 1;
	}
	else if (IS_A("cmd_prefix", form))
		position[PREFIX] = 0;
	else if (IS_A("cmd_name cmd_suffix", form))
	{
		position[COMMAND] = 0;
		position[SUFFIX] = 1;
	}
	else if (IS_A("cmd_name", form))
		position[COMMAND] = 0;
}

/*
** Process given simple command to extract command name, arguements and
** any redirections. Builds a t_simple struct to later be executed
*/

int		simple_command(t_simple *simple, t_ast_node *root)
{
	int		status;
	int		position[3];

	simple_positions(root->rhs, position);
	if (CONTAINS("cmd_prefix", root->rhs)
		&& !OK((status = prefix(simple, root->val[position[PREFIX]]))))
		return (status);
	else if (CONTAINS("cmd_word", root->rhs)
		&& !OK((status = cmd_name(simple, root->val[position[COMMAND]]))))
			return (status);
	else if (CONTAINS("cmd_name", root->rhs)
		&& !OK((status = cmd_name(simple, root->val[position[COMMAND]]))))
			return (status);
	if (CONTAINS("cmd_suffix", root->rhs)
		&& !OK((status = suffix(simple, root->val[position[SUFFIX]]))))
		return (status);
	return (verify_command(simple));
}

/*
** Differentiate simple commands to be processed from
** unimplemented compound commands to be gracefully exited from
*/

int		command(t_exec_node *container, t_ast_node *root, int bg, int bang)
{
	t_simple	*simple;

	if (!IS_A("simple_command", root->rhs))
	{
		ft_printf("Error: %s not implemented", root->rhs);
		return (NIL);
	}
	if (!(simple = ft_memalloc(sizeof(t_simple))))
		return (ERROR);
	container->type = EXEC_SIMPLE_COMMAND;
	container->simple_command = simple;
	simple->bang = bang;
	simple->bg = bg;
	return (simple_command(simple, root->val[0]));
}
