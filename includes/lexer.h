/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 14:08:15 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/26 17:00:04 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "lexer.h"

/*
** Operator DFA definitions
*/

enum	e_parse_ops
{
	EOI = -1,
	START = 0,
	AMPERSAND = 1,
	AND_IF = 2,
	PIPE = 3,
	OR_IF = 4,
	SEMICOLON = 5,
	DSEMI = 6,
	LESS = 7,
	DLESS = 8,
	GREAT = 9,
	DGREAT = 10,
	LESSAND = 11,
	GREATAND = 12,
	LESSGREAT = 13,
	DLESSDASH = 14,
	CLOBBER = 15,
	LEXER_WORD = 17,
	IO_NUMBER = 18
};

# define TOKEN_CONVERSIONS 17
# define ACCEPTING(x) ((x >= 1) && (x <= 15))
# define NOT_ERR(x) (x != 16)


# define _IS_Q(c) ((c == '$' || c == '`'))
# define _NEXT(ctx) (ctx->input[ctx->i + 1])
# define IS_NL(x) (_NEXT(x) == '\n')
# define NEXT_(ctx) (_NEXT(ctx) == ' ' || _NEXT(ctx) == '\t' || IS_NL(ctx))
# define IS_QUOTED(c, ctx) (_IS_Q(c) && _NEXT(ctx) && !NEXT_(ctx))

enum			e_missing_sym
{
	CMD_SUB = 1,
	MATH_SUB = 2,
	BRACE_SUB = 3,
	VAR_SUB = 4,
	BQUOTE = 5,
	QUOTE = 6,
	DQUOTE = 7
};

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

typedef struct	s_token
{
	int			type;
	t_buf		*value;
}				t_token;

typedef struct	s_token_cnv
{
	int			lex_tok;
	int			parse_tok;
}				t_token_cnv;

/*
** lexer/quotes.c
*/

int				find_subst(char *input, int x, t_lctx *ctx);
int				find_quote(char *input, int x, t_lctx *ctx);
int				find_dquote(char *input, int x, t_lctx *ctx);
int				find_bquote(char *input, int x, t_lctx *ctx);

/*
** lexer/operator_dfa.c
*/

int				can_form_op(char c);
int				next_op_state(char c, int current);

/*
** lexer/operator_dfa.c
*/

/*
** lexer/lexer_rules_1.c
*/

void			rule_1(t_token *token, t_lctx *ctx, t_list **tokens);
int				rule_2(char c, t_token *token, t_lctx *ctx);
void			rule_3(t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_4(char c, char *input, t_token *token, t_lctx *ctx);
void			rule_5(char c, t_token *token, t_lctx *ctx);

/*
** lexer/lexer_rules_2.c
*/

void			rule_6(char c, t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_7(t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_8(t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_9(char c, t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_10(t_lctx *ctx);

/*
** lexer/lexer_utils.c
*/

int				next_missing_symbol(t_list *missing);
int				push_missing_symbol(short type, t_list **missing);
int				pop_missing_symbol(t_list **missing);
void			expand_command(char **command);
int				escaped(char *input, size_t i);
int				init_lexer_ctx(char *input, t_lctx *ctx, t_token *token);
int				create_new_tok(t_token *token, t_lctx *ctx, int type);
int				append_to_tok(char c, t_token *token);
int				push_token(t_token *token
					, t_list *node, t_list **tokens, t_lctx *ctx);
int				find_closing_chars(t_list **missing
					, t_token *token, t_lctx *ctx);
int				identify_substitutions(char c
					, t_token *token
					, t_lctx *ctx
					, t_list **missing);
#endif
