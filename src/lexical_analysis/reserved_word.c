/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reserved_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/22 15:30:49 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 16:31:34 by tmatthew         ###   ########.fr       */
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

/*
** When rule 4 of lex_switch determines that a given command is not properly
** closed the opening sequence should be saved so that it may the necessary
** closing characters can be later determined
*/

int						lex_quote(char **str, int start, int end)
{
	int		status;
	short	type;

	if (NONE((status = ERR(end) ? NIL : SUCCESS)))
	{
		if (SNGL_QUOTE((*str), start))
			type = QUOTE;
		else if (DBL_QUOTE((*str), start))
			type = DQUOTE;
		else if (EPARAM_EXP((*str), start))
			type = BRACE_SUB;
		else if (ARITH_EXP((*str), start))
			type = MATH_SUB;
		else if (CMD_SUB((*str), start))
			type = CMD_SUB;
		else if (BACKTICK((*str), start))
			type = BQUOTE;
		else if (BACKSLASH((*str), start))
			type = BACKSLASH_ESC;
		else if (!(type = NIL))
			ft_printf(ERR_UNCLOSED_STR, *str);
		if (ERR(push_missing_symbol(type, &g_missing)))
			status = ERROR;
	}
	return (status);
}
