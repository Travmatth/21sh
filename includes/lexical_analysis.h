/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analysis.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 14:08:15 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/11 17:24:58 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXICAL_ANALYSIS_H
# define LEXICAL_ANALYSIS_H

# include "quote_management.h"
# include "reserved_word.h"
# include "operator_dfa.h"

# define ERR_UNCLOSED_STR "Lex Warning: Unrecognized string is not closed %s"

# define LINE 0
# define TMP 1

/*
** Used in lex_switch to determine current character in given input
*/

# define IS_QUOTED(c, ctx) ((c == '$' || c == '`') || PARENS(ctx->input, ctx->i))
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

/*
** src/lexical_analysis/lexer.c
*/

int				init_lexer_ctx(char *input, t_lctx *ctx, t_token *token);
void			lex_switch(char c
						, t_token *token
						, t_list **tokens
						, t_lctx *ctx);
int				lexical_analysis(char *input, t_list **tokens);

/*
** src/lexical_analysis/lexer_rules_1_4.c
*/

void			rule_1(t_token *token, t_lctx *ctx, t_list **tokens);
int				rule_2(char c, t_token *token, t_lctx *ctx);
void			rule_3(t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_4(char *input, t_token *token, t_lctx *ctx);

/*
** src/lexical_analysis/lexer_rules_5_8.c
*/

void			rule_5(char c, t_token *token, t_lctx *ctx);
void			rule_6(char c, t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_7(t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_8(t_token *token, t_list **tokens, t_lctx *ctx);

/*
** src/lexical_analysis/lexer_rules_9_11.c
*/

void			rule_9(char c, t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_10(t_lctx *ctx);
void			rule_11(char c, t_lctx *ctx, t_token *token);

/*
** src/lexical_analysis/lexer_utils.c
*/

void			clear_current_tokens(t_list **tokens, t_list **missing);
int				create_new_tok(t_token *token, t_lctx *ctx, int type);
int				append_to_tok(char c, t_token *token);
void			del_token(void *content, size_t len);
void			detect_io_number(t_token *token);

/*
** src/lexical_analysis/missing.c
*/

int				prep_missing_terminal(struct termios tty[2]
							, char **line
							, char next[2]
							, int *len);
int				restore_missing_terminal(t_list **tokens
							, char *input
							, char *line
							, char *tmp);
void			add_missing_char(char **line, char *tmp, int *len, char c);
int				manage_backspace(char *line, int *len);
int				manage_missing_closures(char *input
										, t_list **tokens
										, t_list **missing);

/*
** src/lexical_analysis/missing_utils.c
*/

int				next_missing_symbol(t_list *missing);
int				push_missing_symbol(short type, t_list **missing);
int				pop_missing_symbol(t_list **missing, short *type);
void			print_missing_sym(t_list *elem);
void			del_missing(void *content, size_t len);

/*
** src/lexical_analysis/operator_dfa.c
*/

int				can_form_op(char c);
int				next_op_state(char c, int current);

/*
** src/lexical_analysis/process_token.c
*/

int				substitute_alias(t_token *token);
int				process_token(t_token *token, t_lctx *ctx);
int				push_token(t_token *token
							, t_list *node
							, t_list **tokens
							, t_lctx *ctx);

/*
** src/lexical_analysis/reserved_word.c
*/

int				process_reserved(t_token *token, t_lctx *ctx);
int				lex_quote(char **str, int start, int end);
#endif
