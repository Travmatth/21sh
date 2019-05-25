/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/23 14:38:23 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/25 15:39:19 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Determine whether the current character index in the given string is
** preceded by an escape character `\`
*/

int		escaped(char *input, size_t i)
{
	if (i == 0)
		return (FALSE);
	else if (input[i - 1] == '\\')
		return (TRUE);
	return (FALSE);
}

void	free_prods(void)
{
	size_t		i;
	size_t		j;
	t_prod		*prod;

	i = 0;
	while (TRUE)
	{
		prod = &g_prods[i++];
		if (!prod->lhs)
			break ;
		free(prod->lhs);
		j = 0;
		while (prod->rhs && prod->rhs[j])
			free(prod->rhs[j++]);
		if (prod->rhs)
			free(prod->rhs);
	}
	free(g_prods);
}

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
