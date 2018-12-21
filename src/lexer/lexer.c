/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/01 14:37:15 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/20 16:28:59 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

# define END 1
# define BEGIN 0

// if we encounter another opening candidate
// we need to restart search w/ that sequence

/*
** find the next token in the complete_command
*/

int		tokenize_switch(char *input, int i, int n, size_t *offset)
{
	int		s;

	if (!input[1])
		return (NIL);
	s = 0;
	if ((!is_quote(&input[n], i) && (s = find_ws(input, offset)))
		|| IS_SNGL(input, s, &input[n], offset)
		|| IS_DBL(input, s, &input[n], offset))
		return (SUCCESS);
	if (s)
		return (ERROR);
	return (NIL);
}

int		find_sub_end(char c, char *input, size_t *j)
{
	(void)c;
	(void)input;
	(void)j;
	return (SUCCESS);
}

int		escaped(char *input, size_t i)
{
	if (i == 0)
		return (FALSE);
	else if (input[i - 1] == '\\')
		return (TRUE);
	return (FALSE);
}

/*
** given compete_cmd with op starting at i, determine whether operator is valid
*/

int		can_form_op(char *input, t_lctx *ctx, int position)
{
	(void)input;
	(void)ctx;
	(void)position;
	return (NIL);
}

void	init_lexer_ctx(t_lctx *ctx)
{
	ctx->i = 0;
	ctx->j = 0;
	ctx->stop = 0;
	ctx->in_op = 0;
	ctx->in_word = 0;
}

/*
** 1. If the end of input is recognized
** the current token shall be delimited.
** If there is no current token, the end-of-input
** indicator shall be returned as the token.
**
** 4. If the current character is backslash, single-quote,
** or double-quote ( '\', '", or ' )' and it is not quoted,
** it shall affect quoting for subsequent characters up to
** the end of the quoted text. The rules for quoting are as
** described in Quoting. During token recognition no substitutions
** shall be actually performed, and the result token shall
** contain exactly the characters that appear in the input
** (except for <newline> joining), unmodified, including any
** embedded or enclosing quotes or substitution operators,
** between the quote mark and the end of the quoted text.
** The token shall not be delimited by the end of the quoted field.
**
** 5: If the current character is an unquoted '$' or '`',
** the shell shall identify the start of any candidates for
** parameter expansion ( Parameter Expansion),
** command substitution ( Command Substitution),
** or arithmetic expansion ( Arithmetic Expansion)
** from their introductory unquoted character sequences:
** '$' or "${", "$(" or '`', and "$((", respectively.
** The shell shall read sufficient input to determine the end
** of the unit to be expanded (as explained in the cited sections).
** While processing the characters, if instances of expansions or
** quoting are found nested within the substitution, the shell
** shall recursively process them in the manner specified for the
** construct that is found. The characters found from the beginning
** of the substitution to its end, allowing for any recursion
** necessary to recognize embedded constructs, shall be included
** unmodified in the result token, including any embedded or
** enclosing substitution operators or quotes. The token shall not
** be delimited by the end of the substitution.
**
** 7. If the current character is an unquoted <newline>,
** the current token shall be delimited.
**
** 8. If the current character is an unquoted <blank>,
** any token containing the previous character is delimited
** and the current character shall be discarded.
**
** 9. If the previous character was part of a word,
** the current character shall be appended to that word.
**
** 10. If the current character is a '#', it and all subsequent
** characters up to, but excluding, the next <newline> shall be
** discarded as a comment. The <newline> that ends the line is
** not considered part of the comment.
**
** 11. The current character is used as the start of a new word.
*/

int		lexical_analysis(char *input, t_list *tokens)
{
	t_lctx	ctx;
	t_list	*node;
	t_token	token;
	char	c;

	if (!input)
		return (NIL);
	init_lexer_ctx(&ctx);
	while (!ctx.stop)
	{
		c = input[ctx.i];
		// rule 1
		if (!c)
		{
			if (!token.type)
			{
				token.type = EOI;
				token.value = NULL;
			}
			ctx.stop = TRUE;
		}
		else if (ctx.in_op)
			process_operator(c, token, node, tokens, ctx);
		// rule 4
		else if (!escaped(input, ctx.i) && (c == '\\' || c == '\'' || c == '"'))
		{
			if (NONE((ctx.j = find_next(c, &input[1], &ctx.j))))
				return (ERROR);
			ctx.i += ctx.j;
		}
		// rule 5
		else if (!escaped(input, ctx.i) && (c == '$' || c == '`'))
		{
			if (NONE((ctx.j = find_sub_end(c, &input[1], &j))))
				return (ERROR);
			ctx.i += ctx.j;
		}
		else if (can_form_op(input, &ctx, BEGIN))
		{
			ctx.i += 1;
			ctx.in_op += 1;
		}
		// rule 7
		else if (c == '\n' && !espaced(input, ctx.i))
		{
			ctx.i += 1;
			ctx.in_word = FALSE;
		}
		// rule 8
		else if (c == ' ' && !espaced(input, ctx.i))
		{
			ctx.i += 1;
			ctx.in_word = FALSE;
		}
		// rule 9
		else if (ctx.in_word)
			ctx.i += 1;
		// rule 10
		else if (c == '#')
		{
			if (NONE((ctx.j = find_sub_end(c, &input[1], &j))))
				return (ERROR);
			ctx.i += ctx.j;
		}
		// rule 11
		if (!ft_lstnew(&token, sizeof(t_token)))
			return (ERROR);
		ft_lstpushback(&tokens, node);
	}
	return (SUCCESS);
}
