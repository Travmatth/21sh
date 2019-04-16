/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 15:11:58 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/15 18:13:34 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** push given pointer to back of given array
*/

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
		ft_freearr(tmp, TRUE);
		return (NULL);
	}
	return (tmp);
}

/*
** push given pointer to front of given array
*/

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

/*
** Process and_or ast_nodes into and_or nodes in the t_program struct
*/

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
	if (IS_A("and_or AND_IF linebreak pipeline", root->rhs))
		op->type = EXEC_AND;
	else if (IS_A("and_or AND_IF linebreak pipeline", root->rhs))
		op->type = EXEC_OR;
	else
		return (NIL);
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

/*
** Process commands seprated by separators (;/&) as consecutive indexes
** in the final t_program struct
*/

int		list(t_program *p, t_ast_node *root, int last_is_bg)
{
	int			status;
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
		if (!OK((status = separator_op_is_linebreak(&is_bg, root->val[1]))))
			return (status);
		if (!OK((status = list(p, root->val[0], is_bg))))
			return (status);
		return (and_or(op, root->val[0], last_is_bg));
	}
	else if (IS_A("and_or", root->rhs))
		return (and_or(op, root->val[0], last_is_bg));
	else
		return (NIL);
}

/*
** Semantic analysis descends the given ast and processes child nodes to
** construct a t_program struct. A program struct contains an array of pipe,
** logical and simple nodes, where pipe nodes may contain logical, pipe, or 
** simple nodes and logical nodes may contain logical or simple nodes
*/

int		semantic_analysis(t_ast *ast, t_program *program)
{
	int			status;
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
			&& !OK((status = separator(&is_bg, root->val[1]))))
			return (status);
		return (list(program, root->val[0], is_bg));
	}
	return (NIL);
}
