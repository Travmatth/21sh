/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntactic_analysis.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 23:42:59 by tmatthew          #+#    #+#             */
/*   Updated: 2019/05/25 15:47:02 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYNTACTIC_ANALYSIS_H
# define SYNTACTIC_ANALYSIS_H

# include "lexical_analysis.h"
# include "parse_constructs.h"

/*
** src/syntactic_analysis/expansions.c
*/

int			remove_slash(char elem, size_t i, char *str, int *stop);
void		expand_command(char **command);
int			normalize_tokens(char **args);

/*
** src/syntactic_analysis/parse.c
*/

int			reduce(int state, t_list **stack, t_ast_node *word);
int			shift(int state
				, t_list **stack
				, t_ast_node **word
				, t_list **tokens);
int			accept_ast(t_list **stack, t_ast *ast, t_ast_node *word);
int			syntactic_analysis(t_list **tokens, t_ast *ast);

/*
** src/syntactic_analysis/parse_stack_utils.c
*/

t_list		*create_stack_token(int type, t_ast_node *token, int state);
t_list		*create_end_stack_token(void);
t_ast_node	*pop_token(t_list **tokens);

/*
** src/syntactic_analysis/parse_table.c
*/

/*
** src/syntactic_analysis/parse_utils.c
*/

int			peek_state(t_list **stack, int *state);
int			assign_type(char *lhs, t_ast_node *node);
int			reduce_symbol(t_prod *handle
						, t_list **tmp
						, t_stack *sym
						, int *i);
void		del_stack_node(void *contents, size_t size);
int			unrecognized_syntax(void);
#endif
