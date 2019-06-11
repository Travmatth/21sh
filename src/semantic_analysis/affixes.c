/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   affixes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 15:15:29 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/25 15:59:34 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** io_here denote heredocs
*/

int		io_here(t_simple *cmd, int io_num, t_ast_node *root)
{
	int		status;
	t_redir	*redir;
	char	*here_end;
	int		type;

	status = create_redir(&redir, root, &here_end);
	if (OK(status) && IS_A("DLESS here_end", root->rhs))
		type = PARSE_DLESS;
	else if (OK(status) && IS_A("DLESSDASH here_end", root->rhs))
		type = PARSE_DLESSDASH;
	if (OK(status))
		status = process_redir(redir, IO(io_num, STDIN), here_end, type);
	if (OK(status) && OK((status = push_redir(cmd, redir))))
		status = process_heredoc(redir);
	return (status);
}

/*
** Redirections specify alterations to the default input output of a command
*/

int		io_redirect(t_simple *cmd, t_ast_node *root)
{
	int		status;
	int		io_num;

	status = NIL;
	io_num = ERROR;
	if (IS_A("io_file", root->rhs))
		return (io_file(cmd, io_num, root->val[0]));
	else if (IS_A("IO_NUMBER io_file", root->rhs))
	{
		if (!OK((status = io_number(&io_num, root->val[0]))))
			return (status);
		return (io_file(cmd, io_num, root->val[1]));
	}
	else if (IS_A("io_here", root->rhs))
		return (io_here(cmd, io_num, root->val[0]));
	else if (IS_A("IO_NUMBER io_here", root->rhs))
	{
		if (!OK((status = io_number(&io_num, root->val[0]))))
			return (status);
		return (io_here(cmd, io_num, root->val[1]));
	}
	return (status);
}

/*
** Prefixes denote either variable assigments (not implemented in 21sh)
** or io redirections to be performed on the given command
*/

int		prefix(t_simple *cmd, t_ast_node *root)
{
	int		status;

	if (IS_A("io_redirect", root->rhs))
		return (io_redirect(cmd, root->val[0]));
	else if (IS_A("cmd_prefix io_redirect", root->rhs))
		return (!OK((status = prefix(cmd, root->val[0])))
			? status : io_redirect(cmd, root->val[1]));
	else if (IS_A("ASSIGNMENT_WORD", root->rhs))
		ft_putendl("Error: ASSIGNMENT_WORD syntax not implemented");
	else if (IS_A("cmd_prefix ASSIGNMENT_WORD", root->rhs))
	{
		if (!OK((status = prefix(cmd, root->val[0]))))
			return (status);
		ft_putendl("Error: ASSIGNMENT_WORD syntax not implemented");
	}
	return (NIL);
}

/*
** Suffixes denote either arguments to be added to command to be executed,
** or redirections to be performed on the given command
*/

int		suffix_word(t_simple *cmd, t_ast_node *root, int position)
{
	char		**fields;
	char		**tmp;
	char		*word;
	int			status;

	if (!(word = ft_strdup((char*)((t_ast_node*)root->val[position])->val[0])))
		return (ERROR);
	if (!OK((status = full_word_expansion(&fields, word))))
		return (status);
	if (!(tmp = ft_strjoinarrs(cmd->command, fields)))
		return (ERROR);
	free(cmd->command);
	free(fields);
	cmd->command = tmp;
	return (SUCCESS);
}

int		suffix(t_simple *cmd, t_ast_node *root)
{
	int			status;

	if (IS_A("io_redirect", root->rhs))
		return (io_redirect(cmd, root->val[0]));
	else if (IS_A("cmd_suffix io_redirect", root->rhs))
		return (!OK((status = suffix(cmd, root->val[0])))
			? status : io_redirect(cmd, root->val[1]));
	else if (IS_A("WORD", root->rhs))
		return (suffix_word(cmd, root, 0));
	else if (IS_A("cmd_suffix WORD", root->rhs))
	{
		if (!OK((status = suffix(cmd, root->val[0]))))
			return (status);
		return (suffix_word(cmd, root, 1));
	}
	return (SUCCESS);
}
