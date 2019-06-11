/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   missing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/19 15:32:58 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/23 14:40:53 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Returns next closing character sequence needed to correctly close
** given command
*/

int		next_missing_symbol(t_list *missing)
{
	t_list	*node;
	int		type;

	if (!(node = ft_lstpeekhead(missing)))
		return (NIL);
	type = *(short*)node->content;
	return (type);
}

/*
** When given command has elements that are not properly closed, the sequence
** of quotes or command substitutions are saved so as to allow correct sequence
** of closing characters needed.
*/

int		push_missing_symbol(short type, t_list **missing)
{
	t_list	*node;

	if (!(node = ft_lstnew(&type, sizeof(short))))
		return (ERROR);
	ft_lstpushfront(missing, node);
	return (SUCCESS);
}

/*
** When needed closing character detected, it's corresponding opening sequence
** can be popped from the stack. When stack is empty command is correctly
** closed.
*/

int		pop_missing_symbol(t_list **missing, short *type)
{
	t_list	*node;

	node = ft_lsthead(missing);
	if (type)
		*type = *(short*)node->content;
	free(node->content);
	free(node);
	return (SUCCESS);
}

void	print_missing_sym(t_list *elem)
{
	short	sym;

	sym = *(short*)elem->content;
	if (sym == QUOTE)
		ft_putstr("QUOTE ");
	else if (sym == DQUOTE)
		ft_putstr("DQUOTE ");
	else if (sym == BRACE_SUB)
		ft_putstr("BRACE_SUB ");
	else if (sym == MATH_SUB)
		ft_putstr("MATH_SUB ");
	else if (sym == CMD_SUB)
		ft_putstr("CMD_SUB ");
	else if (sym == BQUOTE)
		ft_putstr("BQUOTE_SUB ");
	else if (sym == BACKSLASH_ESC)
		ft_putstr("BACKSLASH ");
}

/*
** Delete given missing node
*/

void	del_missing(void *content, size_t len)
{
	(void)len;
	free(content);
}
