/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/16 16:04:05 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/13 18:54:13 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Process given redirection in order to push correct t_redir struct onto
** the t_simple struct
*/

int		io_file(t_simple *cmd, int io_num, t_ast_node *root)
{
	t_redir	*redir;
	char	*file;
	int		status;

	if (!OK((status = create_redir(&redir, root, &file))))
		return (status);
	if (IS_A("< filename", root->rhs))
		status = process_redir(redir, IO(io_num, STDIN), file, PARSE_LT);
	else if (IS_A("LESSAND filename", root->rhs))
		status = process_redir(redir, IO(io_num, STDIN), file, PARSE_LESSAND);
	else if (IS_A("> filename", root->rhs))
		status = process_redir(redir, IO(io_num, STDOUT), file, PARSE_GT);
	else if (IS_A("GREATAND filename", root->rhs))
		status = process_redir(redir, IO(io_num, STDOUT), file, PARSE_GREATAND);
	else if (IS_A("DGREAT filename", root->rhs))
		status = process_redir(redir, IO(io_num, STDOUT), file, PARSE_DGREAT);
	else if (IS_A("LESSGREAT filename", root->rhs))
		status = process_redir(redir, IO(io_num, STDIN), file, PARSE_LESSGREAT);
	else if (IS_A("CLOBBER filename", root->rhs))
		status = process_redir(redir, IO(io_num, STDOUT), file, PARSE_CLOBBER);
	if (OK(status))
		push_redir(cmd, redir);
	return (SUCCESS);
}

/*
** redirections may optionally specify a number, the fd
** to be used by the given redirection
*/

int		io_number(int *io_num, t_ast_node *root)
{
	char	*io;

	io = (char*)root->val[0];
	if (ERR(ft_safeatoi(io, io_num)))
		return (NIL);
	return (SUCCESS);
}

void	free_ast_node(t_ast_node *root)
{
	int		i;

	if (!root)
		return ;
	if (IS_TERMINAL(root->type))
		free(root->val[0]);
	else
	{
		i = -1;
		while (root->val && root->val[++i])
			free_ast_node((t_ast_node*)root->val[i]);
	}
	free(root->val);
	free(root->lhs);
	free(root->rhs);
	free(root);
}

void	free_ast(t_ast *ast)
{
	if (ast && ast->root)
	{
		free_ast_node((t_ast_node*)ast->root->val[0]);
		free(ast->root->val);
		free(ast->root);
	}
}
