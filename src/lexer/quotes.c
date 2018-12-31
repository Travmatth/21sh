/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 21:44:04 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/30 18:27:11 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		find_subst(char *input, int x, t_lctx *ctx)
{
	int		i;
	int		tmp;
	int		next;
	t_list	*internal_missing;

	i = 1;
	internal_missing = NULL;
	tmp = 0;
	if (input[x + i] == '(')
	{
		push_missing_symbol(CMD_SUB, &internal_missing);
		i += 1;
		tmp = 1;
	}
	else if (input[x + i] == '{')
	{
		push_missing_symbol(BRACE_SUB, &internal_missing);
		i += 1;
	}
	else
	{
		push_missing_symbol(VAR_SUB, &internal_missing);
		i += 1;
	}
	if (tmp && input[x + i] && input[x + i] == '(')
	{
		push_missing_symbol(MATH_SUB, &internal_missing);
		i += 1;
	}
	while ((next = next_missing_symbol(internal_missing)) && input[x + i])
	{
		if (input[x + i] == '\\')
			i += 2;
		else if (input[x + i] == '$' && input[x + i + 1] && (
			input[x + i + 1] == '(' || input[x + i + 1] == '{'))
		{
			if (!OK((tmp = find_subst(input, x + i, ctx))))
				return (tmp);
			i += tmp + 1;
		}
		else if (next == BRACE_SUB && input[x + i] == '}')
		{
			pop_missing_symbol(&internal_missing);
			i += 1;
		}
		else if ((next == CMD_SUB || next == MATH_SUB) && input[x + i] == ')')
		{
			pop_missing_symbol(&internal_missing);
			i += 1;
		}
		else if (next == VAR_SUB && !ft_isalnum(input[x + i])
		{
			pop_missing_symbol(&internal_missing);
			i += 1;
		}
		else if (input[x + i] == '`')
		{
			if (!OK((tmp = find_bquote(input, x + i, ctx))))
				return (tmp);
			i += tmp + 1;
		}
		else if (input[x + i] == '\'')
		{
			if (!OK((tmp = find_quote(input, x + i, ctx))))
				return (tmp);
			i += tmp + 1;
		}
		else if (input[x + i] == '"')
		{
			if (!OK((tmp = find_dquote(input, x + i, ctx))))
				return (tmp);
			i += tmp + 1;
		}
		else
			i += 1;
	}
	if (next && !input[x + i])
	{
		ft_lstmerge(&ctx->missing, internal_missing);
		return (NIL);
	}
	return (i);
}

int		find_quote(char *input, int x, t_lctx *ctx)
{
	int		i;
	int		tmp;

	i = 1;
	push_missing_symbol(QUOTE, &ctx->missing);
	while (input[x + i])
	{
		if (input[x + i] == '\\')
			i += 2;
		else if (input[x + i] == '\'')
		{
			pop_missing_symbol(&ctx->missing);
			break ;
		}
		else if (input[x + i] == '$' && (
			input[x + i + 1] == '(' || input[x + i + 1] == '{'))
		{
			if (!OK((tmp = find_subst(input, x + i, ctx))))
				return (tmp);
			i += tmp;
		}
		else
			i += 1;
	}
	if (input[x + i] != '\'')
		return (NIL);
	return (i);
}

int		find_dquote(char *input, int x, t_lctx *ctx)
{
	int		i;
	int		tmp;

	i = 1;
	push_missing_symbol(DQUOTE, &ctx->missing);
	while (input[x + i])
	{
		if (input[x + i + 1] && input[x + i + 2] && input[x + i] == '\\')
			i += 2;
		else if (input[x + i] == '"')
		{
			pop_missing_symbol(&ctx->missing);
			break ;
		}
		else if (input[x + i] == '$' && (
			input[x + i + 1] == '(' || input[x + i + 1] == '{'))
		{
			if (!OK((tmp = find_subst(input, x + i, ctx))))
				return (tmp);
			i += tmp;
		}
		else
			i += 1;
	}
	if (input[x + i] != '"')
		return (NIL);
	return (i);
}

int		find_bquote(char *input, int x, t_lctx *ctx)
{
	int		i;
	int		tmp;

	i = 1;
	push_missing_symbol(BQUOTE, &ctx->missing);
	while (input[x + i])
	{
		if (input[x + i] == '\\')
			i += 2;
		else if (input[x + i] == '`')
		{
			pop_missing_symbol(&ctx->missing);
			break ;
		}
		else if (input[x + i] == '$' && (
			input[x + i + 1] == '(' || input[x + i + 1] == '{'))
		{
			if (!OK((tmp = find_subst(input, x + i, ctx))))
				return (tmp);
			i += tmp;
		}
		else
			i += 1;
	}
	return (i);
}
