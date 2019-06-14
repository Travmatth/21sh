/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexical_analysis.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 14:08:15 by tmatthew          #+#    #+#             */
/*   Updated: 2019/06/13 16:26:15 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXICAL_ANALYSIS_H
# define LEXICAL_ANALYSIS_H

# include "quote_management.h"
# include "reserved_word.h"
# include "operator_dfa.h"
# include "lexical_constructs.h"

int		init_lexer_ctx(char *input, t_lctx *ctx, t_token *token);
void	lex_switch(char c, t_token *token, t_list **tokens, t_lctx *ctx);
int		lexical_analysis(char *input, t_list **tokens);

/*
** src/lexical_analysis/lexer_rules_1_4.c
*/

void	rule_1(t_token *token, t_lctx *ctx, t_list **tokens);
int		rule_2(char c, t_token *token, t_lctx *ctx);
void	rule_3(t_token *token, t_list **tokens, t_lctx *ctx);
void	rule_4(char *input, t_token *token, t_lctx *ctx);

/*
** src/lexical_analysis/lexer_rules_5_8.c
*/

void	rule_5(char c, t_token *token, t_lctx *ctx);
void	rule_6(char c, t_token *token, t_list **tokens, t_lctx *ctx);
void	rule_7(t_token *token, t_list **tokens, t_lctx *ctx);
void	rule_8(t_token *token, t_list **tokens, t_lctx *ctx);

/*
** src/lexical_analysis/lexer_rules_9_11.c
*/

void	rule_9(char c, t_token *token, t_list **tokens, t_lctx *ctx);
void	rule_10(t_lctx *ctx);
void	rule_11(char c, t_lctx *ctx, t_token *token);

/*
** src/lexical_analysis/lexer_utils.c
*/

void	clear_current_tokens(t_list **tokens, t_list **missing);
int		create_new_tok(t_token *token, t_lctx *ctx, int type);
int		append_to_tok(char c, t_token *token);
void	del_token(void *content, size_t len);
void	detect_io_number(t_token *token);

/*
** src/lexical_analysis/operator_dfa.c
*/

int		can_form_op(char c);
int		next_op_state(char c, int current);

/*
** src/lexical_analysis/process_token.c
*/

int		substitute_alias(t_token *token);
int		process_token(t_token *token, t_lctx *ctx);
int		push_token(t_token *token, t_list *node, t_list **tokens, t_lctx *ctx);

/*
** src/lexical_analysis/reserved_word.c
*/

int		process_reserved(t_token *token, t_lctx *ctx);

/*
** src/lexical_analysis/free_tokens.c
*/

void	free_tokens(t_list **tokens);
#endif
