/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 21:44:04 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/30 14:01:08 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		find_subst(char *input, int x, t_lctx *ctx)
{
	int		i;
	int		j;
	int		next;

	i = 1;
	if (input[x + i] == '(')
	{
		push_missing_symbol(CMD_SUB, &ctx->missing);
		i += 1;
	}
	else if (input[x + i] == '{')
	{
		push_missing_symbol(BRACE_SUB, &ctx->missing);
		i += 1;
	}
	if (input[x + i] == '(' && input[x + i + 1] && input[x + i + 1] == '(')
	{
		push_missing_symbol(MATH_SUB, &ctx->missing);
		i += 1;
	}
	while ((next = next_missing_symbol(ctx->missing)) && input[x + i])
	{
		if (input[x + i] == '\\')
			i += 2;
		else if (input[x + i] == '$' && (
			input[x + i + 1] == '(' || input[x + i + 1] == '{'))
		{
			if (!OK((j = find_subst(input, x + i, ctx))))
				return (j);
			i += j;
		}
		else if (next == BRACE_SUB && input[x + i] == '}')
			pop_missing_symbol(&ctx->missing);
		else if ((next = CMD_SUB || next == MATH_SUB) && input[x + i] == ')')
			pop_missing_symbol(&ctx->missing);
		else if (input[x + i] == '`')
		{
			if (!OK((j = find_bquote(input, x + i, ctx))))
				return (j);
			i += j;
		}
		else if (input[x + i] == '\'')
		{
			if (!OK((j = find_quote(input, x + i, ctx))))
				return (j);
			i += j;
		}
		else if (input[x + i] == '"')
		{
			if (!OK((j = find_dquote(input, x + i, ctx))))
				return (j);
			i += j;
		}
		else
			i += 1;
	}
	if (next && !input[x + i])
		return (ERROR);
	return (i);
}

int		find_quote(char *input, int x, t_lctx *ctx)
{
	int		i;
	int		j;

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
			if (!OK((j = find_subst(input, x + i, ctx))))
				return (j);
			i += j;
		}
		else
			i += 1;
	}
	return (i);
}

int		find_dquote(char *input, int x, t_lctx *ctx)
{
	int		i;
	int		j;

	i = 1;
	push_missing_symbol(DQUOTE, &ctx->missing);
	while (input[x + i])
	{
		if (input[x + i] == '\\')
			i += 2;
		else if (input[x + i] == '"')
		{
			pop_missing_symbol(&ctx->missing);
			break ;
		}
		else if (input[x + i] == '$' && (
			input[x + i + 1] == '(' || input[x + i + 1] == '{'))
		{
			if (!OK((j = find_subst(input, x + i, ctx))))
				return (j);
			i += j;
		}
		else
			i += 1;
	}
	return (i);
}

int		find_bquote(char *input, int x, t_lctx *ctx)
{
	int		i;
	int		j;

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
			if (!OK((j = find_subst(input, x + i, ctx))))
				return (j);
			i += j;
		}
		else
			i += 1;
	}
	return (i);
}
