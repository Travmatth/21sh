/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 15:11:58 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/21 16:23:28 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Process pipe sequence into t_pipe struct, setting both sides of pipe
** if a command is in the form `command_1 | command_2`
*/

int		pipe_sequence(t_exec_node *container
					, t_ast_node *root
					, int bg
					, int bang)
{
	int			status;
	int			i;
	t_pipe		*pipe;
	t_exec_node	*left;
	t_exec_node	*right;

	i = 0;
	if (IS_A("pipe_sequence | linebreak command", root->rhs))
	{
		if (!(pipe = ft_memalloc(sizeof(t_pipe)))
			|| !(left = ft_memalloc(sizeof(t_exec_node)))
			|| !(right = ft_memalloc(sizeof(t_exec_node))))
			return (ERROR);
		container->type = EXEC_PIPE;
		container->pipe = pipe;
		pipe->bang = bang;
		pipe->left = left;
		pipe->right = right;
		pipe->bg = bg;
		i = 3;
		container = right;
		if (!OK((status = pipe_sequence(left, root->val[0], FALSE, FALSE))))
			return (status);
	}
	return (command(container, root->val[i], bg, i ? bang : FALSE));
}

/*
** Process pipeline into t_pipe struct
*/

int		pipeline(t_exec_node *container, t_ast_node *root, int bg)
{
	int		position;
	int		bang;
	int		contains_bang;

	contains_bang = IS_A("Bang pipe_sequence", root->rhs);
	bang = contains_bang ? TRUE : FALSE;
	position = contains_bang ? 1 : 0;
	return (pipe_sequence(container, root->val[position], bg, bang));
}
