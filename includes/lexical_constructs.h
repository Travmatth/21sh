/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_constructs.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/11 22:37:08 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 22:38:24 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXICAL_CONSTRUCTS_H
# define LEXICAL_CONSTRUCTS_H

# ifndef LIBFT_H
#  include "../libftprintf/srcs/includes/ft_printf.h"
# endif

# define ERR_UNCLOSED_STR "Lex Warning: Unrecognized string is not closed %s"

# define LINE 0
# define TMP 1

/*
** Used in lex_switch to determine current character in given input
*/

# define IS_EXP(c) ((c == '$' || c == '`'))
# define IS_QUOTED(c, ctx) (IS_EXP(c) || PARENS(ctx->input, ctx->i))
# define QUOTE_CHAR(c) (c == '\\' || c == '\'' || c == '"')

/*
** Central state struct of lexical analysis, state of lexer when
** iterating through given string to generate lex tokens
*/

typedef struct	s_lctx
{
	char		*input;
	int			status;
	short		i;
	short		j;
	short		stop;
	short		op_state;
	short		in_word;
	t_list		*missing;
}				t_lctx;

/*
** Discrete unit of output produced by the lexer, represents
** one token of the bash grammar
*/

typedef struct	s_token
{
	int			type;
	t_buf		*value;
}				t_token;

/*
** Used by lexer when pushing recognized tokens to convert lexical token
** type to equivalent syntactic parse token type
*/

typedef struct	s_token_cnv
{
	int			lex_tok;
	int			parse_tok;
}				t_token_cnv;
#endif
