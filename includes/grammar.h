/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/02 15:33:00 by tmatthew          #+#    #+#             */
/*   Updated: 2018/12/27 14:38:49 by tmatthew         ###   ########.fr       */
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

# define EOI -1
# define WORD 1
# define IO_HERE 2

# define END 1
# define BEGIN 0

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
** lexer/operator_dfa.c
*/

int				can_form_op(char c);
int				next_op_state(char c, int current);

/*
** lexer/operator_dfa.c
*/

int				remove_slash(char elem, size_t i, char *str, int *stop);
void			expand_command(char **command);
int				find_ws(char *cmd, size_t *offset);
int				find_next(char c, char *cmd, size_t *offset);
int				escaped(char *input, size_t i);
int				init_lexer_ctx(char *input, t_lctx *ctx);
int				create_new_tok(char c, t_token *token, t_lctx *ctx, int type);
int				push_token(t_token *token, t_list *node
					, t_list **tokens, t_lctx *ctx);

/*
** lexer/lexer_rules_1.c
*/

void			rule_1(t_token *token, t_lctx *ctx, t_list **tokens);
int				rule_2(char c, t_token *token, t_lctx *ctx);
void			rule_3(t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_4(char c, char *input, t_token *token, t_lctx *ctx);
void			rule_5(char c, t_token *token, t_list **tokens, t_lctx *ctx);

/*
** lexer/lexer_rules_2.c
*/

void			rule_6(char c, t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_7(char c, t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_8(t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_9(char c, t_token *token, t_list **tokens, t_lctx *ctx);
void			rule_10(char c, t_token *token, t_list **tokens, t_lctx *ctx);
#endif
