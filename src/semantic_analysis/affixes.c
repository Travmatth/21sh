/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   affixes.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 15:15:29 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/05 13:02:18 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		io_file(t_simple_command *cmd, int io_num, t_ast_node *root)
{
	t_redir	*redir;
	char	*file;
	int		status;

	status = create_redir(&redir, root, &file);
	if (OK(status) && IS_A("< filename", root->rhs))
		status = simple_redir(redir, IO(io_num, STDIN), file, PARSE_LT);
	else if (OK(status) && IS_A("LESSAND filename", root->rhs))
		status = dup_redir(redir, IO(io_num, STDIN), file, PARSE_LESSAND);
	else if (OK(status) && IS_A("> filename", root->rhs))
		status = simple_redir(redir, IO(io_num, STDOUT), file, PARSE_LT);
	else if (OK(status) && IS_A("GREATAND filename", root->rhs))
		status = dup_redir(redir, IO(io_num, STDOUT), file, PARSE_GREATAND);
	else if (OK(status) && IS_A("DGREAT filename", root->rhs))
		status = simple_redir(redir, IO(io_num, STDOUT), file, PARSE_DGREAT);
	else if (OK(status) && IS_A("LESSGREAT filename", root->rhs))
		status = simple_redir(redir, IO(io_num, STDIN), file, PARSE_LESSGREAT);
	else if (OK(status) && IS_A("CLOBBER filename", root->rhs))
		status = simple_redir(redir, IO(io_num, STDOUT), file, PARSE_CLOBBER);
	else
		return (ERROR);
	if (!OK(status) && ERR(push_redir(cmd, redir)))
		return (status);
	return (SUCCESS);
}

int		io_number(int *io_num, t_ast_node *root)
{
	char	*io;

	io = (char*)root->val[0];
	if (IS_A("0", io))
		*io_num = 0;
	else
	{
		if (NONE((*io_num = ft_atoi(io))))
			return (ERROR);
	}
	return (SUCCESS);
}

int		io_here(t_simple_command *cmd, int io_num, t_ast_node *root)
{
	int		status;
	t_redir	*redir;
	char	*here_end;
	int		type;

	status = create_redir(&redir, (t_ast_node*)root->val[1], &here_end);
	if (OK(status) && IS_A("DLESS here_end", root->rhs))
		type = PARSE_DLESS;
	else if (OK(status) && IS_A("DLESSDASH here_end", root->rhs))
		type = PARSE_DLESSDASH;
	if (OK(status))
		status = simple_redir(redir, IO(io_num, STDIN), here_end, type);
	if OK(status)
		push_redir(cmd, redir);
	return (status);
}

int		io_redirect(t_simple_command *cmd, t_ast_node *root)
{
	int		io_num;

	io_num = ERROR;
	if (IS_A("io_file", root->rhs))
		return (io_file(cmd, io_num, root->val[0]));
	else if (IS_A("IO_NUMBER io_file", root->rhs))
	{
		if (ERR(io_number(&io_num, root->val[0])))
			return (ERROR);
		return (io_file(cmd, io_num, root->val[1]));
	}
	else if (IS_A("io_here", root->rhs))
		return (io_here(cmd, io_num, root->val[0]));
	else if (IS_A("IO_NUMBER io_here", root->rhs))
	{
		if (ERR(io_number(&io_num, root->val[0])))
			return (ERROR);
		return (io_here(cmd, io_num, root->val[1]));
	}
	return (SUCCESS);
}

int		prefix(t_simple_command *cmd, t_ast_node *root)
{
	if (!IS_A("io_redirect", root->rhs))
		return (io_redirect(cmd, root->val[0]));
	else if (!IS_A("cmd_prefix io_redirect", root->rhs))
		return (ERR(prefix(cmd, root->val[0]))
			? ERROR : io_redirect(cmd, root->val[1]));
	else if (!IS_A("ASSIGNMENT_WORD", root->rhs))
		ft_putendl("Error: ASSIGNMENT_WORD syntax not implemented");
	else if (!IS_A("cmd_prefix ASSIGNMENT_WORD", root->rhs))
	{
		if (ERR(prefix(cmd, root->val[0])))
			return (ERROR);
		ft_putendl("Error: ASSIGNMENT_WORD syntax not implemented");
	}
	return (SUCCESS);
}

int		suffix(t_simple_command *cmd, t_ast_node *root)
{
	int		status;
	char	**tmp;
	char	**fields;

	if (IS_A("io_redirect", root->rhs))
		return (io_redirect(cmd, root->val[0]));
	else if (IS_A("cmd_suffix io_redirect", root->rhs))
		return (ERR(suffix(cmd, root->val[0]))
			? ERROR : io_redirect(cmd, root->val[1]));
	else if (IS_A("WORD", root->rhs))
	{
		if (!OK((status = full_word_expansion(&fields, (char*)root->val[0]))))
			return (status);
		if (!(tmp = ft_strjoinarrs(cmd->command, fields)))
			return (ERROR);
		free(cmd->command);
		cmd->command = tmp;
	}
	else if (IS_A("cmd_suffix WORD", root->rhs))
	{
		if (ERR(suffix(cmd, root->val[0])))
			return (ERROR);
		if (!OK((status = full_word_expansion(&fields, (char*)root->val[1]))))
			return (status);
		if (!(tmp = ft_strjoinarrs(cmd->command, fields)))
			return (ERROR);
		free(cmd->command);
		cmd->command = tmp;
	}
	return (SUCCESS);
}
