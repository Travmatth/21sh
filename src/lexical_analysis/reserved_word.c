/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reserved_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/22 15:30:49 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 22:47:32 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

/*
** Array specifying reserved tokens and their equivalent types
*/

t_reserved_conversion	g_reserved_conversion[RESERVED_WORD_CONVERSIONS] =
{
	{ "if", RESERVED_IF },
	{ "then", RESERVED_THEN },
	{ "else", RESERVED_ELSE },
	{ "elif", RESERVED_ELIF },
	{ "fi", RESERVED_FI },
	{ "do", RESERVED_DO },
	{ "done", RESERVED_DONE },
	{ "case", RESERVED_CASE },
	{ "esac", RESERVED_ESAC },
	{ "while", RESERVED_WHILE },
	{ "until", RESERVED_UNTIL },
	{ "for", RESERVED_FOR },
	{ "{", RESERVED_LBRACE },
	{ "}", RESERVED_RBRACE },
	{ "!", RESERVED_BANG },
	{ "in", RESERVED_IN }
};

/*
** Determine whether a given token constitutes a reserved word
*/

int						process_reserved(t_token *token, t_lctx *ctx)
{
	int		i;
	int		status;
	char	*contents;

	i = -1;
	status = SUCCESS;
	contents = (char*)token->value->buf;
	while (++i < RESERVED_WORD_CONVERSIONS)
	{
		if (IS_A(g_reserved_conversion[i].reserved, contents))
		{
			if ((g_reserved_conversion[i].symbol == RESERVED_LBRACE
				|| g_reserved_conversion[i].symbol == RESERVED_RBRACE)
				&& !first_word(ctx->input, ctx->i - 1))
				break ;
			token->type = g_reserved_conversion[i].symbol;
			ft_printf(RESERVED_LEX_ERR, contents);
			status = NIL;
			break ;
		}
	}
	return (status);
}
