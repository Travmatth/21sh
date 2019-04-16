/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analysis.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 14:08:15 by tmatthew          #+#    #+#             */
/*   Updated: 2019/04/15 16:37:06 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXICAL_ANALYSIS_H
# define LEXICAL_ANALYSIS_H

# include "quote_management.h"
# include "reserved_word.h"
# include "operator_dfa.h"

# define ERR_UNCLOSED_STR "Lex Warning: Unrecognized string is not closed %s"

/*
** Used in lex_switch to determine current character in given input
*/

# define IS_QUOTED(c) ((c == '$' || c == '`'))
# define IS_QUOTE_CHAR(c) (c == '\\' || c == '\'' || c == '"')

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
int		push_token(t_token *token, t_list *node, t_list **tokens, t_lctx *ctx);
int		next_missing_symbol(t_list *missing);
int		push_missing_symbol(short type, t_list **missing);
int		pop_missing_symbol(t_list **missing);

/*
** src/lexical_analysis/process_token.c
*/

int		push_token(t_token *token, t_list *node, t_list **tokens, t_lctx *ctx);
#endif
