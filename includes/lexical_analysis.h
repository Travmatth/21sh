/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analysis.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 14:08:15 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/01 15:18:31 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXICAL_ANALYSIS_H
# define LEXICAL_ANALYSIS_H

/*
** Used by quote management functions to give context pecific functionality to
** functions as they parse different quote types - lexical analysis needs to
** store unclosed quotes, word expansion needs to execute closed quoted strings
*/

t_list		*g_missing;
typedef	int	(*t_quote)(char **str, int start, int end);

# define NEXT_BRACE(s, i) (s[i + 1] && s[i + 1] == '{')
# define NEXT_PAREN(s, i) (s[i + 1] && s[i + 1] == '(')
# define TWO_PARENS(s, i) (NEXT_PAREN(s, i) && NEXT_PAREN(s, i + 1))

# define SNGL_QUOTE(s, i) (s[i] == '\'')
# define DBL_QUOTE(s, i) (s[i] == '"')
# define PARAM_EXP(s, i) (s[i] == '$' && NEXT_BRACE(s, i))
# define ARITH_EXP(s, i) (s[i] == '$' && TWO_PARENS(s, i))
# define CMD_SUB(s, i) (s[i] == '$' && NEXT_BRACE(s, i))
# define BACKTICK(s, i) (s[i] == '`')

/*
** Operator DFA symbols used to match input to specific bash operator
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

/*
** Used within the lexing to track types of quotes/substitutions
** in given command and whether all are properly closed
*/

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

/*
** src/utils/quote_management.c
*/

int		quote(char **str, int start, int *end, t_quote f);
int		backtick(char **str, int start, int *end, t_quote f);
int		dbl_quote(char **str, int start, int *end, t_quote f);
int		param_exp(char **str, int start, int *end, t_quote f);
int		command_sub(char **str, int start, int *end, t_quote f);
int		arith_exp(char **str, int start, int *end, t_quote f);

/*
** src/lexical_analysis/lexer.c
*/

void	rule_11(char c, t_lctx *ctx, t_token *token);
void	lex_switch(char c, t_token *token, t_list **tokens, t_lctx *ctx);
int		lexical_analysis(char *input, t_list **tokens, t_list **missing);

/*
** src/lexical_analysis/lexer_rules_1.c
*/

void	rule_1(t_token *token, t_lctx *ctx, t_list **tokens);
int		rule_2(char c, t_token *token, t_lctx *ctx);
void	rule_3(t_token *token, t_list **tokens, t_lctx *ctx);
void	rule_4(char c, char *input, t_token *token, t_lctx *ctx);
void	rule_5(char c, t_token *token, t_lctx *ctx);

/*
** src/lexical_analysis/lexer_rules_2.c
*/

void	detect_io_number(t_token *token);
void	rule_6(char c, t_token *token, t_list **tokens, t_lctx *ctx);
void	rule_7(t_token *token, t_list **tokens, t_lctx *ctx);
void	rule_8(t_token *token, t_list **tokens, t_lctx *ctx);
void	rule_9(char c, t_token *token, t_list **tokens, t_lctx *ctx);
void	rule_10(t_lctx *ctx);

/*
** src/lexical_analysis/lexer_utils.c
*/

int		escaped(char *input, size_t i);
int		init_lexer_ctx(char *input, t_lctx *ctx, t_token *token);
int		append_to_tok(char c, t_token *token);
int		create_new_tok(t_token *token, t_lctx *ctx, int type);
int		convert_token(t_token *token);
int		push_token(t_token *token, t_list *node, t_list **tokens, t_lctx *ctx);
int		next_missing_symbol(t_list *missing);
int		push_missing_symbol(short type, t_list **missing);
int		pop_missing_symbol(t_list **missing);

/*
** src/lexical_analysis/operator_dfa.c
*/

int		can_form_op(char c);
int		next_op_state(char c, int current);

/*
** src/lexical_analysis/quotes.c
*/

int		find_subst(char *input, int x, t_lctx *ctx);
int		find_quote(char *input, int x, t_lctx *ctx);
int		find_dquote(char *input, int x, t_lctx *ctx);
int		find_bquote(char *input, int x, t_lctx *ctx);

/*
** src/lexical_analysis/reserved_dfa.c
*/

int				process_reserved(char c, t_token *token, t_lctx *ctx);
#endif
