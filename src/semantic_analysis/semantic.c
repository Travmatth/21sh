/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semantic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 15:11:58 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/16 16:18:03 by tmatthew         ###   ########.fr       */
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

int		and_or(t_exec_node *container, t_ast_node *root, int is_bg)
{
	int			position;
	int			status;
	t_operator	*op;
	t_exec_node	*left;
	t_exec_node	*right;

	position = 0;
	if (!IS_A("pipeline", root->rhs))
	{
		if (!(op = ft_memalloc(sizeof(t_operator)))
			|| !(left = ft_memalloc(sizeof(t_exec_node)))
			|| !(right  = ft_memalloc(sizeof(t_exec_node))))
			return (ERROR);
		container->type = CONTAINS("AND_IF", root->rhs) ? EXEC_AND : EXEC_OR;
		container->operator = op;
		op->left = left;
		op->right = right;
		op->bg = is_bg;
		op->type = container->type;
		container = right;
		position = 3;
		if (!OK((status = and_or(left, root->val[0], FALSE))))
			return (status );
	}
	return (pipeline(container, root->val[position], position ? FALSE : is_bg));
}

/*
** Process commands seprated by separators (;/&) as consecutive indexes
** in the final t_program struct
*/

int		list(t_program *p, t_ast_node *root, int last_is_bg)
{
	int			status;
	int			is_bg;
	t_exec_node	*child;
	void		**tmp;
	int			position;

	if (!(child = ft_memalloc(sizeof(t_exec_node)))
		|| !(tmp = push_pointer_front((void**)p->commands, (void*)child)))
		return (ERROR);
	free(p->commands);
	p->commands = (t_exec_node**)tmp;
	position = 0;
	if (IS_A("list separator_op and_or", root->rhs))
	{
		if (!OK((status = separator_op_is_linebreak(&is_bg, root->val[1])))
			|| !OK((status = list(p, root->val[0], is_bg))))
			return (status);
		position = 2;
	}
	return (and_or(child, root->val[position], last_is_bg));
}

/*
** Semantic analysis descends the given ast and processes child nodes to
** construct a t_program struct. A program struct contains an array of pipe,
** logical and simple nodes, where pipe nodes may contain logical, pipe, or 
** simple nodes and logical nodes may contain logical or simple nodes
*/

int		semantic_analysis(t_ast *ast, t_program *p)
{
	int			status;
	t_ast_node	*root;
	int			is_bg;

	if (!p->commands && !(p->commands = ft_memalloc(sizeof(t_operator*))))
		return (ERROR);
	is_bg = FALSE;
	root = (t_ast_node*)ast->root->val[0];
	status = SUCCESS;
	if (IS_A("list separator", root->rhs))
		status = separator(&is_bg, root->val[1]);
	status = OK(status) && list(p, root->val[0], is_bg);
	free_ast(ast);
	return (status);
}
