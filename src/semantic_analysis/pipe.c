/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 15:11:58 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/02 17:21:49 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		pipe_sequence(t_pipe *pipe, t_ast_node *root, int bg)
{
	int					position;
	t_pipe_child		*pipe_child;
	t_pipe				*tmp;
	t_simple_command	*s_command;

	position = 0;
	if (IS_A("pipe_sequence | linebreak command", root->rhs))
	{
		if (!(pipe_child = ft_memalloc(sizeof(t_pipe_child)))
			|| !(tmp = ft_memalloc(sizeof(t_pipe))))
			return (ERROR);
		pipe_child->pipe = tmp;
		pipe->left = pipe_child;
		pipe->bg = bg;
		position = 3;
		if (ERR(pipe_sequence(tmp, root->val[0], FALSE)))
			return (ERROR);
	}
	if (!IS_A("command", root->rhs)
		|| !(pipe_child = ft_memalloc(sizeof(t_pipe_child)))
		|| !(s_command = ft_memalloc(sizeof(t_simple_command))))
		return (ERROR);
	pipe_child->simple_command = s_command ;
	pipe_child->type = EXEC_SIMPLE_COMMAND;
	pipe->type = position ? EXEC_PIPE : EXEC_NONE;
	if (position)
		pipe->right = pipe_child;
	else
		pipe->left = pipe_child;
	return (command(s_command, root->val[position], bg));
}

int		pipeline(t_pipe *pipe, t_ast_node *root, int bg)
{
	int		position;

	position = 0;
	if (IS_A("Bang pipe_sequence", root->rhs))
	{
		pipe->bang = TRUE;
		position = 1;
	}
	else if (!IS_A("pipe_sequence", root->rhs))
		return (ERROR);
	return pipe_sequence(pipe, root->val[position], bg);
}
