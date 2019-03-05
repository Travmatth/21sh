/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/02 15:33:00 by tmatthew          #+#    #+#             */
/*   Updated: 2019/03/04 17:37:25 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef GRAMMAR_H
# define GRAMMAR_H

/*
** Operator DFA definitions
*/

# define START 0
# define AMPERSAND 1
# define AND_IF 2
# define PIPE 4
# define OR_IF 4
# define SEMICOLON 5
# define DSEMI 6
# define LESS 7
# define DLESS 8
# define GREAT 9
# define DGREAT 10
# define LESSAND 11
# define GREATAND 12
# define LESSGREAT 13
# define DLESSDASH 14
# define CLOBBER 15
# define ACCEPTING(x) ((x >= 1) && (x <= 15))
# define NOT_ERR(x) (x != 16)

/*
** Parsed token types
** 
*/

# define EOI -1
# define LEXER_WORD 1
# define IO_HERE 2

# define END 1
# define BEGIN 0

# define CMD_SUB 1
# define MATH_SUB 2
# define BRACE_SUB 3
# define VAR_SUB 4
# define BQUOTE 5
# define QUOTE 6
# define DQUOTE 7

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

/*
** used in src/parse/parse.c
** to hold tokens pushed onto parse stack
*/

typedef struct	s_handle
{
	char		*lhs;
	char		**rhs;
	char		**lookahed;
}				t_prod;

extern char		*g_parse_table[][53];

enum			e_stack_token_type
{
	STACK_END,
	STACK_TOKEN,
	STACK_STATE
};

enum			e_parse_sym
{
	PARSE_WORD = 23
};

typedef union	u_item
{
	t_token		*token;
	int			state;
}				t_item;

typedef struct	s_stack
{
	int			type;
	t_item		item;
}				t_stack;

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
void			rule_7(char c, t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_8(t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_9(char c, t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_10(char c, t_token *token, t_list **tokens, t_lctx *ctx);

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
int				identify_substitutions(char c, t_token *token, t_lctx *ctx, t_list **missing);
#endif
