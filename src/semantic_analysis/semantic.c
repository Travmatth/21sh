/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 15:11:58 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/02 17:39:45 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

char	**push_pointer_back(char **pointers, t_ast_node *node)
{
	int		i;
	char	**tmp;
	char	*word;

	if (!node)
		return (NULL);
	i = 0;
	while (pointers && pointers[i])
		i += 1;
	if (!(tmp = ft_memalloc(sizeof(void*) * (i + 2))))
		return (NULL);
	i = -1;
	while (pointers && pointers[++i])
		tmp[i] = pointers[i];
	word = (char*)node->val[0];
	if (!(tmp[i == -1 ? 0 : i] = ft_strdup(word)))
	{
		ft_freearr(tmp);
		return (NULL);
	}
	return (tmp);
}

void	**push_pointer_front(void **pointers, void *ptr)
{
	int		i;
	void	**tmp;

	if (!pointers || !ptr)
		return (NULL);
	i = 0;
	while (pointers[i])
		i += 1;
	if (!(tmp = ft_memalloc(sizeof(void*) * (i + 2))))
		return (NULL);
	tmp[0] = ptr;
	i = 0;
	while (pointers[i])
	{
		tmp[i + 1] = pointers[i];
		i += 1;
	}
	return (tmp);
}

int		and_or(t_operator *op, t_ast_node *root, int is_bg)
{
	t_operator	*tmp;
	t_op_child	*op_child;
	t_pipe		*pipe;

	if (!(op_child = ft_memalloc(sizeof(t_op_child)))
		|| !(pipe = ft_memalloc(sizeof(t_pipe))))
		return (ERROR);
	if (IS_A("pipeline", root->rhs))
	{
		op_child->type = EXEC_PIPE;
		op_child->pipe = pipe;
		op->type = EXEC_NONE;
		op->left = op_child;
		return (pipeline(pipe, root->val[0], is_bg));
	}
	if (!(tmp = ft_memalloc(sizeof(t_operator))))
		return (ERROR);
	if (!IS_A("and_or AND_IF linebreak pipeline", root->rhs))
		op->type = EXEC_AND;
	else if (!IS_A("and_or AND_IF linebreak pipeline", root->rhs))
		op->type = EXEC_OR;
	else
		return (ERROR);
	op->bg = is_bg;
	op_child->type = EXEC_OP;
	op_child->operator = tmp;
	op->left = op_child;
	if (ERR(and_or(tmp, root->val[0], FALSE)))
		return (ERROR);
	if (!(op_child = ft_memalloc(sizeof(t_op_child)))
		|| !(tmp = ft_memalloc(sizeof(t_operator))))
		return (ERROR);
	op_child->type = EXEC_PIPE;
	op_child->pipe = pipe;
	op->right = op_child;
	return (pipeline(pipe, root->val[3], FALSE));
}

int		list(t_program *p, t_ast_node *root, int last_is_bg)
{
	int			is_bg;
	t_operator	*op;
	t_operator	**tmp;

	is_bg = FALSE;
	if (!(op = ft_memalloc(sizeof(t_operator)))
		|| !(tmp = (t_operator**)push_pointer_front((void**)p->commands, (void*)op)))
		return (ERROR);
	free(p->commands);
	p->commands = tmp;
	if (IS_A("list separator_op and_or", root->rhs))
	{
		if (ERR(separator_op_is_linebreak(&is_bg, root->val[1])))
			return (ERROR);
		if (ERR(list(p, root->val[0], is_bg)))
			return (ERROR);
		return (and_or(op, root->val[0], last_is_bg));
	}
	else if (IS_A("and_or", root->rhs))
		return (and_or(op, root->val[0], last_is_bg));
	else
		return (ERROR);
	return (SUCCESS);
}

int		semantic_analysis(t_ast *ast, t_program *program)
{
	t_ast_node	*root;
	int			is_bg;

	is_bg = FALSE;
	if (!program->commands
		&& !(program->commands = ft_memalloc(sizeof(t_operator*))))
		return (ERROR);
	if (IS_A("complete_command $end", ast->root->rhs))
	{
		root = (t_ast_node*)ast->root->val[0];
		if (IS_A("list separator", root->rhs)
			&& ERR(separator(&is_bg, root->val[1])))
			return (ERROR);
		return (list(program, root->val[0], is_bg));
	}
	return (NIL);
}